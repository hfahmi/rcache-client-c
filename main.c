/**************************************************************************************
 * Author: Husni Fahmi
 * Email: husnifahmi@outlook.com
 * Date: 2021-11-30
 * Filename: main.cpp
 * Description: rcache client to perform INSERT, SEARCH, and DELETE item
 * from rcache-server.
 * rcache-client connects to rcache-server.
 * rcache-client sends command SET, GET, UNSET to rcache-server.
 *
 * v01: 2021-11-30
 *
 **************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/rcache/rcache.h"
#include "include/rcache/rcache-zmq.h"

int main(int argc, char **argv)
{
    // read_config()
    // connect to rcache-server
    // send command SET to rcache-server
    // send command GET to rcache-server
    // send command UNSET to rcache-server
    // close connection to rcache-server

    int rc;
    void *conn;

    int port = 21001;

    int timeout; // milliseconds
    int buf_length;
    char recv_buf[1024];

    buf_length = 512;

    rcache_data msg;

    char key[128];
    char value[512];

    char rcache_server[128];

    if (argc != 2)
    {
        printf("Usage: ./rcache-client-c <rcache-server-host>\n");
        exit(EXIT_FAILURE);
    }
    // strcpy(rcache_server, "172.17.226.250");
    strcpy(rcache_server, argv[1]);

    init_zmq();
    conn = rcache_connect(rcache_server, port);
    if (conn == NULL)
    {
        fprintf(stderr, "FATAL: Failed to connect to %s\n", rcache_server);
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    strcpy(key, "key1");
    strcpy(value, "thisisvalue1");

    rc = rcache_send_cmd(conn, RCACHE_CMD_HASH_SET, key, value);
    timeout = 100; // 100 milliseconds
    rc = rcache_recv_rep(conn, recv_buf, buf_length, timeout);

    return 0;
}
