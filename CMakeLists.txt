project(ff_bq)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
add_definitions(-std=c++11)
#add_definitions(-g)
add_definitions(-O2)
add_definitions(-DUSING_WORK_STEALING_QUEUE)
add_definitions(-DNDEBUG)

include_directories(${PROJECT_SOURCE_DIR}/../ff/include)
include_directories(${PROJECT_SOURCE_DIR}/../statistic_mutex_time/)
message(${PROJECT_SOURCE_DIR}/../ff/include)

set(LIBS_LINK_LIBS ff pthread smt)
link_directories(${PROJECT_SOURCE_DIR}/../bin)
link_directories(${PROJECT_SOURCE_DIR}/../statistic_mutex_time/build/)

if(CACHE_EVAL)
  add_definitions(-DCACHE_EVAL)
  set(LIBS_LINK_LIBS ff pthread papi)
  MESSAGE(STATUS "This is cache evaluation version...")
endif(CACHE_EVAL)

add_executable(ff_bq main.cpp bq.cpp record.cpp)
target_link_libraries(ff_bq boost_program_options ${LIBS_LINK_LIBS})

install(TARGETS ff_bq RUNTIME DESTINATION bin)
