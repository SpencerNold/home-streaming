#ifndef CLIENT_H
#define CLIENT_H

typedef struct {
    int sockfd;
    int running;
    void (*handle)(void*, char*, int);
} UDP_Client;

UDP_Client* UDP_Open(const char* host, int port);
void UDP_SetHandle(UDP_Client* client, void (*handle)(void*, char*, int));
void UDP_Listen(UDP_Client* client);
int UDP_Send(UDP_Client* client, char* message, int length);
void UDP_Close(UDP_Client* client);

#endif