set(TITLE benchmark_c)

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -pg -D_REENTRANT -fPIC" )

set(SOURCES benchmark.c)

add_executable(${TITLE} ${SOURCES})

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # Mac OS X

target_link_libraries( ${TITLE} batchringbuffer )

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux") # Linux

target_link_libraries( ${TITLE} batchringbuffer -lrt )

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

