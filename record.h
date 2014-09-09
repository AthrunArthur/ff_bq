#ifndef RECORD_H_
#define RECORD_H_
#include <cstdint>
#include <fstream>
#include <string>
#include <map>
#include <ff.h>


namespace RecordLocks{

    extern std::string g_strRecordFile;
    
    struct LockAndThrd{
        intptr_t lock;
        int32_t   thrd;
    };
    
    extern std::map<LockAndThrd, int64_t, 
        std::function<bool (const LockAndThrd &, 
                            const LockAndThrd &)> > g_olockTimes;
    
    void record(void * lock_ptr);
    
    void close_records();
    
};//end namespace RecordLocks
#endif
