#ifndef BUFFER_H
#define BUFFER_H

typedef struct {
    char* buffer;
} BUF_Buffer;

void BUF_Update(BUF_Buffer* buffer, char* data, int length);

#endif