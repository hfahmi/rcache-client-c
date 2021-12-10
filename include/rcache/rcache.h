/*************************************************************************************
// Author: Husni Fahmi
// Email: husnifahmi@outlook.com
// Date: 2021-11-29
// Filename: rcache.h
// Description: header file for RCACHE
//
*************************************************************************************/

#ifndef RCACHE_RCACHE_H_
#define RCACHE_RCACHE_H_

#define RCACHE_VER_01 0x01

#define RCACHE_CONNECT_HASH 0x11

/* RCACHE accepts or rejects conneciton request from client */
#define RCACHE_CONNECT_ACCEPT 0x01
#define RCACHE_CONNECT_REJECT 0x00

#define RCACHE_CMD_HASH_SET 0x21
#define RCACHE_CMD_HASH_GET 0x22
#define RCACHE_CMD_HASH_DEL 0x23

#define RCACHE_CMD_LIST_LPUSH 0x31
#define RCACHE_CMD_LIST_RPOP 0x32

typedef struct rcache_cmd_hash_t
{
    char rcache[6];
    char version;
    char command;
    char client_id[12];
    int32_t data_length;
    int32_t key_length;
    char *key;
    int32_t value_length;
    char *value;
} rcache_cmd_hash;

typedef struct rcache_data_t
{
    char rcache[6];
    char version;
    char command;
    char client_id[12];
    char session_id[12];
    int32_t data_length;
    char data[6400];
} rcache_data;

#endif // RCACHE_RCACHE_H_
