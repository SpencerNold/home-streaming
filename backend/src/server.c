#include "server.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

UDP_Server* UDP_Open(int port) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        return NULL;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (bind(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        return NULL;
    }
    UDP_Server* server = (UDP_Server*) malloc(sizeof(UDP_Server));
    if (server == NULL) {
        close(sockfd);
        return NULL;
    }
    server->handle = NULL;
    server->sockfd = sockfd;
    server->running = 1;
    return server;
}

void UDP_SetHandle(UDP_Server* server, void (*handle)(void*, UDP_Client, char*, int)) {
    server->handle = handle;
}

void UDP_Listen(UDP_Server* server) {
    int size = 2048;
    char buffer[size];
    int read;
    struct sockaddr_in client_addr;
    unsigned int client_addr_len = (unsigned int) sizeof(client_addr);
    while (server->running) {
        read = recvfrom(server->sockfd, buffer, size, 0, (struct sockaddr*) &client_addr, &client_addr_len);
        if (read < 0) {
            continue;
        }
        if (server->handle != NULL) {
            UDP_Client client;
            client.addr_handle = &client_addr;
            client.addr_handle_len = client_addr_len;
            server->handle(server, client, buffer, read);
        }
    }
}

int UDP_Send(UDP_Server* server, UDP_Client client, char* data, int length) {
    return sendto(server->sockfd, data, length, 0, (struct sockaddr*) client.addr_handle, client.addr_handle_len);
}

void UDP_Close(UDP_Server* server) {
    close(server->sockfd);
    free(server);
}