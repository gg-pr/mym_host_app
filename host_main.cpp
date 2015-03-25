/*
 * host_main.c
 *
 *  Created on: Mar 14, 2015
 *      Author: GURU
 */
/*
 * March 2015
 * This is the file which gets called during library init from
 * the host process.
 */
/*
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include "pthread.h"
#include "ue_msg_types.h"
*/
#include <pthread.h>
#include <stdlib.h>
#include <iostream>
//#include <tchar.h>
#include <windows.h>
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <unistd.h>
#include "ue_msg_types.h"

using namespace std;


#define   TRUE  1
#define   FALSE 0
//#define   NULL 0

/*
#ifdef _WIN32
#if !defined(_WIN64) && !defined(__MINGW64_VERSION_MAJOR)
struct timespec {
    int tv_sec;
    long tv_nsec;
};
#endif
#endif
*/

static int conn_exist = TRUE;
int client_sockfd;
int connfd;
FILE *f;
HANDLE host_pipe;

/*
 * This function is called by GUI module when user/admin
 * wishes to close the GUI thereby ending communication
 * channel with the Modem.
 */
int
ue_close_sock()
{
    closesocket(connfd);
    fclose(f);

    /*
     * Send Connection close message to Modem and terminate the receive thread
     */
    conn_exist = FALSE;
    return 0;
}

int open_socket()
{
	struct sockaddr_in serv_addr;
    client_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(client_sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(client_sockfd, 10);
    return 0;
}

void * client_recv(void *id)
{
    int n = 0;
    char recvBuff[1024], parseBuff[1024];
    long fsize;
    wchar_t recv_data[1024];
    size_t orig_size=0;
	size_t converted_char_len=0;


    memset(recvBuff, '0', sizeof(recvBuff));

    /*if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf ("\n Error: Could not create socket\n");
		return 1;
	}

	memset (&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000);

	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <=0) {
		printf ("\n inet_pton error occured\n");
		return 1;
	}

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
       printf("\n Error : Connect Failed \n");
       return 1;
    }
*/
    //while ((n = read(client_sockfd, recvBuff, sizeof(recvBuff)-1)) > 0) {
	while ((n = recv(client_sockfd, recvBuff, (sizeof(recvBuff)-1), 0)) > 0) {
  //      f = fopen("testrxd.enc", "wb");

    //    fwrite(recvBuff, n, 1, f);
     //   fclose(f);

//        (void)system("openssl aes-256-cbc -d -a -in testrxd.enc -out testrxd.json -pass pass:\"helloamar\"");

    /*    f = fopen("testrxd.json", "rb");
        fseek(f, 0, SEEK_END);
        fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        fread(parseBuff, fsize, 1, f);

        //parse_json(parseBuff);

        fclose(f);
*/
    }

    if(n < 0) {
        //printf("\n Read error \n");
		cout<<"Read error";
    }

    /*We have now received the buffer, send it to Named pipe connecting GUI */ 
    mbstowcs_s(&converted_char_len, recv_data, orig_size, recvBuff, _TRUNCATE);

	host_write_to_pipe(0,static_cast (wchar_t )recv_data,host_pipe);
    return NULL;
}

/*
 * Remove/Modify this function if this is made as Library
 */
int main (int argc, char *argv[])
{

    int ret_val = 0;

	//char sendBuff[1025];
	char *sendBuff;

//	time_t ticks;
	long fsize;
    pthread_t recv_tid;
    pthread_attr_t attr;
    int *params[2];
	payload *p= NULL;

	/*
     * Create named pipe towards GUI application
     */
	host_create_pipe(&host_pipe);

	/*
	 * Start the state machine for this Modem interface
	 *
	 */

    ret_val = pthread_attr_init(&attr);
    if(ret_val != 0){
        cout<<"\nAttr init failed on host";
    }

	ret_val = pthread_create(&recv_tid, 0, client_recv, params[0]);
	if(ret_val != 0){
		cout<<"\nreceive thread creation failed on host";
	}

	int temp = 100;
	p = new payload;
	p->msg = new msg_t;
    p->bulk_msg_count = 1;
    p->msg->msg_id = UE_MODEM_INIT_MSG;
	p->msg->msg_len = 4;
	p->msg->data = new int;
	//p->msg->data) = 100;
	p->msg->data = &temp;
	p->msg->next = NULL;

	host_write_to_pipe(0, reinterpret_cast (wchar_t *) p, host_pipe);
	
	while (1) {

//		(void)system("openssl aes-256-cbc -a -in test.json -out test.enc -pass pass:\"helloamar\"");

		f = fopen("test.enc", "rb");
		fseek(f, 0, SEEK_END);
		fsize = ftell(f);
		fseek(f, 0, SEEK_SET);
		
        sendBuff = new char[fsize+1];

		if (sendBuff == NULL) {
			cout<<"\n Error mallocing for json data\n";
			return -1;
		}
		//memset(sendBuff, '0', sizeof(sendBuff));

		connfd = accept(client_sockfd, (struct sockaddr*)NULL, NULL);
        /*accept_sock = accept(sock,NULL, NULL);
        if (accept_sock == INVALID_SOCKET) {
            printf("\nAccept function failed with the error");
        }*/

		cout<<"Connection accepted\n";

		fread(sendBuff, fsize, 1, f);
		//sendBuff[fsize] = 0;

		//snprintf(sendBuff,sizeof(sendBuff), "Hello Amarnath!\n");
		send(connfd, sendBuff, fsize,0);
		//write(connfd, sendBuff, fsize);
        //write(accept_sock, sendBuff, fsize);

		//sleep(1);
	}
}

