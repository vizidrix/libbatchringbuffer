set(TITLE example_c)

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99" )
set(SOURCES example.c)

add_executable(${TITLE} ${SOURCES})
target_link_libraries( ${TITLE} batchringbuffer )