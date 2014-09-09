project(ff_bq)
add_definitions(-std=c++11)
#add_definitions(-g)
add_definitions(-O2)
add_definitions(-DUSING_FF_NONBLOCKING_QUEUE)
add_definitions(-DNDEBUG)

include_directories(${PROJECT_SOURCE_DIR}/../ff/include)
message(${PROJECT_SOURCE_DIR}/../ff/include)

set(LIBS_LINK_LIBS ff pthread)
link_directories(${PROJECT_SOURCE_DIR}/../bin)

add_executable(ff_bq main.cpp bq.cpp record.cpp)
target_link_libraries(ff_bq boost_program_options ${LIBS_LINK_LIBS})

install(TARGETS ff_bq RUNTIME DESTINATION bin)
