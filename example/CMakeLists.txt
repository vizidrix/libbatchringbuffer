set(TITLE example_c)

#set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -pthread -D_REENTRANT -fPIC" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -pthread -D_REENTRANT -fPIC" )

set(SOURCES example.c)

add_executable(${TITLE} ${SOURCES})
target_link_libraries( ${TITLE} batchringbuffer )