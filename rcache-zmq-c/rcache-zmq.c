#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <zmq.h>

#include "include/rcache/rcache.h"
#include "include/rcache/rcache-zmq.h"

static int zmq_initialized = 0;
static void *context;
static void *rcache_worker;

static char rcache_worker_id[16];

static rcache_cmd_hash rcmd_hash;

char cmd_buf[8400];

//  Provide random number from 0..(num-1)
#define randof(num) (int)((float)(num)*random() / (RAND_MAX + 1.0))

int init_zmq()
{
    context = zmq_ctx_new();
    zmq_initialized = 1;

    return 0;
} // int init_zmq()

void *rcache_connect(char *rcache_server, int port)
{
    int rc;
    int seed;

    char tcp_str[128];

    if (zmq_initialized == 0)
    {
        rc = init_zmq();
        if (rc != 0)
        {
            // fprintf(stderr, "FATAL: Failed to initialize zeromq.\n");
            // fflush(stderr);
            return NULL;
        }
        zmq_initialized = 1;
    }

    seed = time(NULL);
    seed += 'C' + 'L' + 'E' + 'N' + 'T';
    srand(seed * getpid());

    // begin: Setup recv_responder server, zeromq
    // set random identity to make tracing easier
    sprintf(rcache_worker_id, "%04X-%04X-%02X",
            randof(0x10000), randof(0x10000), randof(0x100));
    zmq_setsockopt(rcache_worker, ZMQ_IDENTITY,
                   rcache_worker_id, strlen(rcache_worker_id));

    sprintf(tcp_str, "tcp://%s:%d", rcache_server, port);
    printf("tcp_str: %s\n", tcp_str);
    rc = zmq_connect(rcache_worker, tcp_str);
    printf("zmq_connect, rc: %d\n", rc);
    if (rc != 0)
    {
        // fprintf(stderr, "FATAL: Failed to connect to %s\n", rcache_server);
        // fflush(stderr);
        return NULL;
    }

    strncpy(rcmd_hash.rcache, "RCACHE", 6);
    strncpy(rcmd_hash.client_id, rcache_worker_id, 12);
    rcmd_hash.version = RCACHE_VER_01;

    // int max_key_size = 128;
    // rcmd_hash.key = (char *)malloc(sizeof(char) * max_key_size);

    // int max_value_length = 128;
    // rcmd_hash.key = (char *)malloc(sizeof(char) * max_key_size);

    return rcache_worker;
} // void *rcache_connect()

int rcache_send_cmd(void *conn, char cmd, char *key, char *value)
{
    int rc;
    int offset;
    int msg_length;
    int data_length;
    int key_length;
    int value_length;

    key_length = strlen(key) + 1;
    if (key_length > 128)
    {
        fprintf(stderr, "key length cannot be greater than 128 bytes\n");
        fflush(stderr);
        return -1;
    }

    value_length = strlen(value) + 1;
    if (value_length > 2048)
    {
        fprintf(stderr, "value length cannot be greater than 2048 bytes\n");
        fflush(stderr);
        return -1;
    }

    offset = 0;
    memcpy(&cmd_buf[offset], "RCACHE", 6);
    offset += 6;
    cmd_buf[offset] = RCACHE_VER_01;
    offset += 1;
    cmd_buf[offset] = cmd;
    offset += 1;

    // client_id
    memcpy(&cmd_buf[offset], rcache_worker_id, 12);
    offset += 12;

    memset(&cmd_buf[offset], 0, 4); // data_length
    offset += 4;

    memcpy(&cmd_buf[offset], (char *)&key_length, 4); // key_length
    offset += 4;

    strcpy(&cmd_buf[offset], key); // key
    offset += key_length;

    memcpy(&cmd_buf[offset], (char *)&value_length, 4); // value_length
    offset += 4;

    strcpy(&cmd_buf[offset], value); // key
    offset += value_length;

    data_length = 4 + 4 + key_length + 4 + value_length;

    memcpy(&cmd_buf[20], (char *)&data_length, 4); // data_length

    msg_length = 20 + data_length;
    printf("send command to rcache-server: %d.\n", msg_length);
    rc = zmq_send(conn, cmd_buf, msg_length, 0);
    if (rc == -1)
    {
        fprintf(stderr, "FATAL: zmq_send failed, %s, (1)", strerror(errno));
        fflush(stderr);
        return -1;
    }

    return 0;
} // int rcache_send_cmd(void *conn, char cmd, char *key, char *value)

int rcache_recv_rep(void *conn, char *recv_buf, int buf_length, int timeout)
{
    int rc;
    int num_iter;

    num_iter = timeout / 10; // poll every 10 milliseconds
    while (num_iter > 0)
    {
        // poll and zmq_recv
        zmq_pollitem_t items[] = {
            {conn, 0, ZMQ_POLLIN, 0}};

        // poll every 10 milliseconds
        rc = zmq_poll(items, 1, 10);
        if (rc == -1)
        {
            fprintf(stderr, "zmq_poll failed, %s, (1)", strerror(errno));
            fflush(stderr);
            return -1;
        }

        if (items[0].revents & ZMQ_POLLIN)
        {
            printf("receiving data from RCACHE server...\n");
            rc = zmq_recv(rcache_worker, recv_buf, 512, 0);
            recv_buf[rc] = '\0';
            printf("%s\n", recv_buf);

            int recv_length;
            recv_length = rc;
            printf("recv_length: %d\n", recv_length);
        }
    }

    return -1;
} // int rcache_recv_rep()
