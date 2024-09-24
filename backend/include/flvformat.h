#ifndef FLVFORMAT_H
#define FLVFORMAT_H

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>

typedef struct {
    AVFormatContext* fmtctx;
    AVCodecContext* avctx;
    AVPacket* packet;
    int videoStreamIndex;
    int audioStreamIndex;
} FLV_Stream;

typedef struct {
    char* data;
    int length;
    int type;
    int error;
} FLV_Packet;

FLV_Stream* FLV_OpenStream(const char* name);
void FLV_ReadPacket(FLV_Stream* stream, FLV_Packet* packet);
void FLV_CloseStream(FLV_Stream* stream);

#endif