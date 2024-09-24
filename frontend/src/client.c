#include "client.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct sockaddr_in server_addr;
unsigned int server_addr_len = 0;

UDP_Client* UDP_Open(const char* host, int port) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        return NULL;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(host);
    server_addr_len = (unsigned int) sizeof(server_addr);
    UDP_Client* client = (UDP_Client*) malloc(sizeof(UDP_Client));
    if (client == NULL) {
        close(sockfd);
        return NULL;
    }
    client->sockfd = sockfd;
    client->running = 1;
    return client;
}

void UDP_SetHandle(UDP_Client* client, void (*handle)(void*, BUF_Buffer*)) {
    client->handle = handle;
}

void UDP_Listen(UDP_Client* client) {
    int size = 2048;
    char message[size];
    memset(message, 0, size);
    BUF_Buffer buffer;
    while (client->running) {
        int read = recvfrom(client->sockfd, message, size, 0, (struct sockaddr*) &server_addr, &server_addr_len);
        if (read < 0) {
            break;
        }
        BUF_Update(&buffer, message, read);
        client->handle(client, &buffer);
    }
}

int UDP_Send(UDP_Client* client, char* message, int length) {
    return sendto(client->sockfd, (void*) message, length, 0, (struct sockaddr*) &server_addr, server_addr_len);
}

void UDP_Close(UDP_Client* client) {
    close(client->sockfd);
    free(client);
}