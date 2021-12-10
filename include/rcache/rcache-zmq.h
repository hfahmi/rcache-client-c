/*************************************************************************************
 * Author: Husni Fahmi
 * Email: husnifahmi@outlook.com
 * Date: 2021-11-30
 * Filename: rcache-zmq.h
 * Description: header file for rcache client which contains
 * functions related to zeromq socket communication with rcache-server.
 * 
*************************************************************************************/

#ifndef RCACHE_ZMQ_H_
#define RCACHE_ZMQ_H_

#ifdef __cplusplus
extern "C"
{
#endif

    int init_zmq();
    void *rcache_connect(char *rcache_server, int port);
    int rcache_send_cmd(void *conn, char cmd, char *key, char *value);
    int rcache_recv_rep(void *conn, char *recv_buf, int buf_length, int timeout);

#ifdef __cplusplus
}
#endif

#endif /* RCACHE_ZMQ_H_ */
