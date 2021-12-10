# include_directories(../)

add_library(rcache-zmq-c 
    rcache-zmq.c
)

include_directories(${PROJECT_SOURCE_DIR})
target_link_libraries(rcache-zmq-c zmq)

