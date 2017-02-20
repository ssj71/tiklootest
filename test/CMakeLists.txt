#this is just a test, typically the tikloo folder will be next to your source or in a libs dir, but here you must make a symbolic link for it to work (or copy the dir)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g ")

add_subdirectory(tikloo)
include_directories( ${TIKLOO_INCLUDE_DIRS} )



add_executable(test_ui
    test.c
)
add_executable(unittest EXCLUDE_FROM_ALL
    unittest.c

)

target_link_libraries(test_ui tikloo ${TIKLOO_LDFLAGS} )
target_link_libraries(unittest tikloo ${TIKLOO_LDFLAGS} )
