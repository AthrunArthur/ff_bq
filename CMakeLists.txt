project(ff_bq)
add_definitions(-std=c++11)
add_definitions(-DUSING_MIMO_QUEUE)
include_directories(${PROJECT_SOURCE_DIR}/../ff/include)
message(${PROJECT_SOURCE_DIR}/../ff/include)

set(LIBS_LINK_LIBS ff pthread)
link_directories(${PROJECT_SOURCE_DIR}/../bin)

add_executable(ff_bq main.cpp bq.cpp)
target_link_libraries(ff_bq boost_program_options ${LIBS_LINK_LIBS})

install(TARGETS ff_bq RUNTIME DESTINATION bin)
