
#include "bq.h"
#include <vector>
#include <memory>
#include <mutex>
#include <random>
#include <ff.h>

#define MSG_LEN 1024
#define MQ_LEN 4096
#define Q_N 4

#define RECORD_LOCKS
#ifdef RECORD_LOCKS
#include "record.h"
#endif


class Msg {
public:
    Msg():content(MSG_LEN) {
        for(int i = 0; i < MSG_LEN; ++i)
        {
            content[i] = '0' + i%10;
        }
    }
protected:
    std::vector<char> 	content;
};
typedef std::shared_ptr<Msg> Msg_ptr;

template<class LOCK_T>
class BQ {
public:
    BQ()
        : lock_in()
        , lock_out()
        , pq_in(nullptr)
        , pq_out(nullptr)
        , s_in(0)
        , s_out(0) {
        pq_in = new Msg_ptr[MQ_LEN];
        pq_out = new Msg_ptr[MQ_LEN];
    }

    virtual ~BQ()
    {
	lock_out.lock();
	lock_in.lock();
        delete[] pq_in;
        delete[] pq_out;
	lock_in.unlock();
	lock_out.unlock();
    }

    bool		pop_msg(Msg_ptr & msg)
    {
        //lock_out.lock();
        if (s_out == 0)
        {
            lock_in.lock();
            MQ_t t = pq_in;
            pq_in = pq_out;
            pq_out = t;
            size_t ts = s_in;
            s_in = s_out;
            s_out = ts;
            lock_in.unlock();
        }
        if(s_out == 0) {
            //lock_out.unlock();
            return false;
        }
        s_out --;
        msg = pq_out[s_out];
        //lock_out.unlock();
        return true;
    }

    LOCK_T & 	in_lock() {
        return lock_in;
    }
    LOCK_T &	out_lock() {
        return lock_out;
    }


    bool		push_msg(Msg_ptr &msg)
    {
        //lock_in.lock();
        if(s_in == MQ_LEN)
        {
            lock_in.unlock();
            lock_out.lock();
            lock_in.lock();
            MQ_t t = pq_in;
            pq_in = pq_out;
            pq_out = t;
            size_t ts = s_in;
            s_in = s_out;
            s_out = ts;
            lock_out.unlock();
        }
        if(s_in == MQ_LEN)
        {
            //lock_in.unlock();
            return false;
        }
        pq_in[s_in] = msg;
        s_in ++;
        //lock_in.unlock();
        return true;

    }
protected:
    typedef Msg_ptr * MQ_t;

    LOCK_T	lock_in;
    LOCK_T	lock_out;
    MQ_t		pq_in;
    MQ_t		pq_out;
    size_t	s_in;
    size_t	s_out;
};
typedef Msg_ptr	*	MQ_t;

void serial(int task_num)
{
    MQ_t p = new Msg_ptr[MQ_LEN];

    for(int i = 0; i < task_num; ++i)
    {

    }
}
#define MIN_NUM 50000
#define MAX_NUM 100000

void parallel_bq(int msg_num, int thrd_num, BQ<ff::mutex> & bq)
{
    int iMsg_num = 0;

    ff::paragroup pg;
    while(iMsg_num < msg_num)
    {
        std::random_device rd;
        std::default_random_engine dre(rd());
        std::uniform_int_distribution<int> dist(MIN_NUM, MAX_NUM);
        int m = dist(dre);

        ff::para<> p;
        p([&bq, m]() {
            bq.in_lock().lock();
#ifdef RECORD_LOCKS
            RecordLocks::record(&(bq.in_lock()), 1);
#endif
            for(int i = 0; i < m; i ++)
            {
                Msg_ptr msg = std::make_shared<Msg>();
                bq.push_msg(msg);
            }
            bq.in_lock().unlock();
        }, bq.in_lock().id());
        pg.add(p);

        iMsg_num += m;

        std::mt19937 e2(rd());
        std::normal_distribution<> ndist(m, 2);
        int n = ndist(e2);

        ff::para<> q;
        q([&bq, n]() {
            bq.out_lock().lock();
#ifdef RECORD_LOCKS
            RecordLocks::record(&(bq.out_lock()), 0);
#endif
            for(int i = 0; i<n; ++i)
            {
                Msg_ptr msg;
                bq.pop_msg(msg);
            }
            bq.out_lock().unlock();
        }, bq.out_lock().id());
        pg.add(q);

    }
    ff::ff_wait(ff::all(pg));
}

void parallel_bq(int msg_num, int thrd_num, BQ<std::mutex> & bq)
{
    int iMsg_num = 0;

    ff::paragroup pg;
    while(iMsg_num < msg_num)
    {
        std::random_device rd;
        std::default_random_engine dre(rd());
        std::uniform_int_distribution<int> dist(MIN_NUM, MAX_NUM);
        int m = dist(dre);

        ff::para<> p;
        p([&bq, m]() {
            bq.in_lock().lock();
#ifdef RECORD_LOCKS
            RecordLocks::record(&(bq.in_lock()), 1);
#endif
            for(int i = 0; i < m; i ++)
            {
                Msg_ptr msg = std::make_shared<Msg>();
                bq.push_msg(msg);
            }
            bq.in_lock().unlock();
        });
        pg.add(p);

        iMsg_num += m;

        std::mt19937 e2(rd());
        std::normal_distribution<> ndist(m, 2);
        int n = ndist(e2);

        ff::para<> q;
        q([&bq, n]() {
            bq.out_lock().lock();
#ifdef RECORD_LOCKS
            RecordLocks::record(&(bq.out_lock()), o);
#endif
            for(int i = 0; i<n; ++i)
            {
                Msg_ptr msg;
                bq.pop_msg(msg);
            }
            bq.out_lock().unlock();
        });
        pg.add(q);

    }
    ff::ff_wait(ff::all(pg));
}
void parallel(int msg_num, int thrd_num, bool ff_lock)
{
  int sub = msg_num/Q_N;
  ff::paragroup pg;
  for(int i = 0; i <Q_N; ++i)
  {
    ff::para<> p;
    p([sub, thrd_num, ff_lock](){
    if(ff_lock)
    {
        BQ<ff::mutex> bq;
        parallel_bq(sub, thrd_num, bq);
    }
    else
    {
        BQ<std::mutex> bq;
        parallel_bq(sub, thrd_num, bq);
    }
    });
    pg.add(p);
  }
  ff::ff_wait(ff::all(pg));
}


