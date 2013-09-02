set(TITLE bench_c)

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -pg -pthread -D_REENTRANT -fPIC" )

set(SOURCES bench.c)

add_executable(${TITLE} ${SOURCES})
target_link_libraries( ${TITLE} batchringbuffer )