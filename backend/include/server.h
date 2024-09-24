#ifndef SERVER_H
#define SERVER_H

typedef struct {
    void* addr_handle;
    int addr_handle_len;
} UDP_Client;

typedef struct {
    int sockfd;
    int running;
    void (*handle)(void*, UDP_Client, char*, int);
} UDP_Server;

UDP_Server* UDP_Open(int port);
void UDP_SetHandle(UDP_Server* server, void (*handle)(UDP_Server*, UDP_Client, char*, int));
void UDP_Listen(UDP_Server* server);
int UDP_Send(UDP_Server* server, UDP_Client client, char* data, int length);
void UDP_Close(UDP_Server* server);

#endif