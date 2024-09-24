#ifndef PACKET_H
#define PACKET_H

#include <inttypes.h>

typedef struct {
    uint8_t id;
    uint8_t* data;
    uint16_t length;
}  PAK_Packet;

void PAK_Send(void* client, PAK_Packet packet);

#endif