#include "packet.h"
#include "client.h"

#include <stdio.h>

void PAK_Send(void* client, PAK_Packet packet) {
    int size = packet.length + 3;
    uint8_t data[size];
    uint8_t* target = data;
    *(target++) = (uint8_t) (size >> 8);
    *(target++) = (uint8_t) (size >> 0);
    *(target++) = packet.id;
    int len = packet.length;
    while (len--)
        *target++ = *packet.data++;
    UDP_Send((UDP_Client*) client, (char*) data, size);
}

int PAK_Read(PAK_Packet* packet, char* message, int length) {
    if (length < 3) {
        return -1;
    }
    return 0;
}