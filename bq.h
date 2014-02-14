#ifndef FF_BENCH_BQ_H_
#define FF_BENCH_BQ_H_


void 	serial(int msg_num);

void	parallel(int msg_num, int thrd_num, bool ff_lock);

#endif
