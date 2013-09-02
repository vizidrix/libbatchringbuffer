set(TITLE libbatchringbuffer_test)

set(SOURCES test.cpp)

include_directories(${GTEST_INCLUDE_DIRS})

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # Mac OS X

set_source_files_properties( ${SOURCES}
                             PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pthread -D_REENTRANT -Wno-write-strings  -DBENCH" )

add_executable(${TITLE} ${SOURCES})
target_link_libraries(${TITLE} ${GTEST_BOTH_LIBRARIES} batchringbuffer ${CMAKE_THREAD_LIBS_INIT})
add_test(${TITLE} ${EXECUTABLE_OUTPUT_PATH}/${TITLE})

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux") # Linux

set_source_files_properties( ${SOURCES}
                             PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pthread -D_REENTRANT -Wno-write-strings -DBENCH" )

add_executable(${TITLE} ${SOURCES})
target_link_libraries(${TITLE} ${GTEST_BOTH_LIBRARIES} batchringbuffer ${CMAKE_THREAD_LIBS_INIT} -lrt)
add_test(${TITLE} ${EXECUTABLE_OUTPUT_PATH}/${TITLE})

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

