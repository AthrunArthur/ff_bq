#include "record.h"

namespace RecordLocks{
  std::string g_strRecordFile = "locks.dat";
     std::map<LockAndThrd, int64_t, 
        std::function<bool (const LockAndThrd &, 
                            const LockAndThrd &)> > g_olockTimes(
                                [](const LockAndThrd & l1, const LockAndThrd & l2)
                                -> bool
                                {
                                    if(l1.lock < l2.lock)
                                        return true;
                                    else if(l1.lock == l2.lock && l1.thrd < l1.thrd)
                                        return true;
                                    else
                                        return false;
                                }
                            );

    void record(void * lock_ptr)
    {
        LockAndThrd lt;
        lt.lock = reinterpret_cast<intptr_t>(lock_ptr);
        lt.thrd = ff::rt::get_thrd_id();
        g_olockTimes[lt] ++;
    }
    
    void close_records()
    {
        std::fstream f(g_strRecordFile, std::ios::out);
        for (auto it = g_olockTimes.begin(); it != g_olockTimes.end(); ++it)
        {
            const LockAndThrd & lt = (*it).first;
            f<<lt.lock<<"\t"<<lt.thrd<<"\t"<<(*it).second<<std::endl;
        }
        f.close();
    }
    
}//end namespace RecordLocks
