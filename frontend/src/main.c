#include "window.h"
#include "client.h"
#include "thread.h"
#include "packet.h"
#include "frame.h"

#include <stdio.h>

void* connect(void*);
void handle(void*, BUF_Buffer*);

#include "flv.h"
int main() {
    //PROC_StartNewThread(connect);

    FLV* flv = flv_open();
    PACKET packet;

    FLV_Context* context = FLV_AllocContext(1920, 1080, 21);
    if (context == NULL) {
        return 1;
    }

    WIN_Window* window = WIN_Create("title", 1920, 1080);
    if (window == NULL) {
        FLV_FreeContext(context);
        return 2;
    }

    while (window->running) {
        WIN_PrepareRender(window);
        WIN_PollEvents(window);

        flv_read_packet(flv, &packet);
        if (packet.type == flv->videoStreamIndex && packet.error == 0) {
            FLV_DecodeFrame(context, packet.data, packet.length);
            FLV_ScaleFrame(context);
            WIN_FillTexture(window, context);
        }

        WIN_DoRender(window);
    }
    WIN_Destroy(window);
    FLV_FreeContext(context);
}

void* connect(void* ptr) {
    UDP_Client* client = UDP_Open("127.0.0.1", 8192);
    UDP_SetHandle(client, handle);
    UDP_Listen(client);
    UDP_Close(client);
    return NULL;
}

void handle(void* client, BUF_Buffer* buffer) {
    
}