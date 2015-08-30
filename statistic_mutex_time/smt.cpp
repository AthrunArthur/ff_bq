#include "smt.h"
#include <chrono>
#include <vector>
#include <map>
#include <iostream>
#include <cstdint>

typedef std::chrono::time_point<std::chrono::system_clock> time_point_t;
typedef std::chrono::duration<uint64_t> duration_t;
time_point_t last_start_time;

std::map<void *, time_point_t> mutex_last_hold_time;

duration_t task_exe_duration;
std::map<void *, duration_t> mutex_hold_duration;

void start_exe_task(){
  last_start_time = std::chrono::system_clock::now();
}

void pause_exe_task(){
  auto e = std::chrono::system_clock::now();
  task_exe_duration += std::chrono::duration_cast<
    std::chrono::duration<uint64_t>>(e - last_start_time);
}

void start_hold_mutex(void * pmutex){
  auto e = std::chrono::system_clock::now();
  mutex_last_hold_time.insert(std::make_pair(pmutex, e));
}

void end_hold_mutex(void * pmutex){
  auto e = std::chrono::system_clock::now();
  auto t = std::chrono::duration_cast<duration_t>(
      e - mutex_last_hold_time[pmutex]);
  if(mutex_hold_duration.find(pmutex) == mutex_hold_duration.end()){
    mutex_hold_duration.insert(std::make_pair(pmutex, t));
  }
  else{
    mutex_hold_duration[pmutex] += t;
  }
}

void print_results(){
  std::cout<<"\n***********************"<<std::endl;
  std::cout<<"task exe time: "<<task_exe_duration.count()<<std::endl;
  int i = 1;
  for(std::map<void *, duration_t>::iterator it = mutex_hold_duration.begin();
      it != mutex_hold_duration.end(); ++it){
    std::cout<<i<<"th mutex hold time: "<<it->second.count()<<std::endl;
    i++;
  }
}
