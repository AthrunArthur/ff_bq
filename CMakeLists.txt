project(ff_bq)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
add_definitions(-std=c++11)
#add_definitions(-g)
add_definitions(-O2)
add_definitions(-DUSING_FF_NONBLOCKING_QUEUE)
add_definitions(-DNDEBUG)

include_directories(${PROJECT_SOURCE_DIR}/../ff/include)
message(${PROJECT_SOURCE_DIR}/../ff/include)

set(LIBS_LINK_LIBS ff pthread)
link_directories(${PROJECT_SOURCE_DIR}/../bin)

if(CACHE_EVAL)
  add_definitions(-DCACHE_EVAL)
  set(LIBS_LINK_LIBS ff pthread papi)
  MESSAGE(STATUS "This is cache evaluation version...")
endif(CACHE_EVAL)

add_executable(ff_bq main.cpp bq.cpp record.cpp)
target_link_libraries(ff_bq boost_program_options ${LIBS_LINK_LIBS})

install(TARGETS ff_bq RUNTIME DESTINATION bin)
