#include "server.h"
#include "flvformat.h"

void handle(void*, UDP_Client, char*, int);

int main() {
    // FLV_Stream* stream = FLV_OpenStream("backend/movies/flv/movie.flv");

    UDP_Server* server = UDP_Open(8192);
    UDP_SetHandle(server, handle);
    UDP_Listen(server);
    UDP_Close(server);
}

void handle(void* server, UDP_Client client, char* data, int length) {
    
}