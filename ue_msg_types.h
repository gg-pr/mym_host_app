/*
 * ue_msg_types.h
 *
 *  Created on: Mar 14, 2015
 *      Author: GURU
 */
/*
 * This file defines the message types for all the communication
 *  between Host and Modem
 */
#ifndef UE_MSG_TYPES_H_
#define UE_MSG_TYPES_H_

#include "stdio.h"
#include<stdlib.h>
#include <iostream>
//#include <tchar.h>
#include <windows.h>
#include <Winsock2.h>
#include <ws2tcpip.h>

typedef enum ue_msg_header {
    UE_MODEM_INIT_MSG = 1,
    UE_GET_MODEM_INFO,
    UE_SEND_MODEM_INFO,

    UE_MAX_MSG
}ue_msg_header;

typedef struct msg_struct 
{
    ue_msg_header      msg_id;
    int                msg_len;
    void               *data;
    struct msg_struct  *next;
}msg_t;

typedef struct payload_t {
    int     bulk_msg_count;
    msg_t   *msg;
}payload;

int send_modem(wchar_t *sendbuf, int size);
int host_create_pipe();
void * pipe_client_recv(void *id);


#endif /* UE_MSG_TYPES_H_ */
