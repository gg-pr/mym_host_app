///// CLIENT PROGRAM /////
 
#include <iostream>
#include <windows.h>
using namespace std;


//#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include "ue_msg_types.h"
//#include <pthread.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "5000"

HANDLE pipe;

int send_modem(wchar_t *sendbuf, int size)
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    printf("Connection accepted\n");
    
        // Send an initial buffer
    char *chsendbuf = new char[2048], *encbuff = new char[2048];
/*    char *sslparam[] = {"aes-256-cbc", " -d", " -a", " -in", " chsendbuf",  
                      "-out", " chrxbuf.txt", " -pass", " pass: \"hello amar\""};*/
    wcstombs(chsendbuf, sendbuf, size);
    
    //execve("openssl", sslparam, NULL);
    
    iResult = send(ClientSocket, chsendbuf, size, 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    cout<<"Bytes Sent: %ld\n"<<iResult;

    
    // No longer need server socket
    closesocket(ListenSocket);

}
 
int pipe_close_host()
{
    CloseHandle(pipe);
}

void * pipe_client_recv(void *id)
{
    cout << "Reading data from pipe...\n";
     
    // The read operation will block until there is data to read
    wchar_t buffer[128];
    DWORD numBytesRead = 0;
    BOOL result = ReadFile(
                           pipe,
                           buffer, // the data from the pipe will be put here
                           127 * sizeof(wchar_t), // number of bytes allocated
                           &numBytesRead, // this will store number of bytes actually read
                           NULL // not using overlapped IO          
                   );
     
    if (result) {
        int size = (numBytesRead / sizeof(wchar_t)) + 1;
        buffer[numBytesRead / sizeof(wchar_t)] = '\0'; // null terminate the string
        wcout << "Number of bytes read: " << numBytesRead << endl;
        wcout << "Message: " << buffer << endl;
        cout<<"size of data passed = %d\n"<<size;
        send_modem(buffer, size);
    } else {
            wcout << "Failed to read data from the pipe." << endl;
    }
}

int pipe_create_bw_thread()
{
    cout<<"Connecting to pipe...\n" ;
        
    WaitNamedPipe(L"\\\\.\\pipe\\yodha_thd", 0xffffffff);
 
    pipe = CreateFile(
            L"\\\\.\\pipe\\yodha_thd",
            GENERIC_READ, // only need read access
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
     
    if (pipe == INVALID_HANDLE_VALUE) {
            cout << "Failed to connect to pipe.\n";
            // look up error code here using GetLastError()
            system("pause");
            return 1;
        }
     
    return 0;
}

//int main(int argc, const char **argv)
int pipe_create_host()
{   
    cout<<"Connecting to pipe...\n" ;
        
    WaitNamedPipe(L"\\\\.\\pipe\\yodha", 0xffffffff);
    pipe = CreateFile(
            L"\\\\.\\pipe\\yodha",
            GENERIC_READ, // only need read access
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
     
        if (pipe == INVALID_HANDLE_VALUE) {
            cout << "Failed to connect to pipe.\n";
            // look up error code here using GetLastError()
            system("pause");
            return 1;
 
		}
    return 0;
}