cmake_minimum_required(VERSION 3.0.0)
project(rcache-client-c VERSION 0.1.0)

include(CTest)
enable_testing()

# include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(rcache-zmq-c)

# include_directories("./include/rcache")

add_executable(rcache-client-c 
    main.c
)

# target_include_directories(rcache-client-c PUBLIC "./")
# target_include_directories(rcache-client-c PUBLIC "${PROJECT_SOURCE_DIR}/")
# target_link_libraries(rcache-client-c zmq)
target_link_libraries(rcache-client-c rcache-zmq-c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
