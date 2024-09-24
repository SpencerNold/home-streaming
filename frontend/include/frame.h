#ifndef FRAME_H
#define FRAME_H

#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>

typedef struct {
    int width;
    int height;
    AVCodecContext* avctx;
    AVPacket* packet;
    AVFrame* frame;
    AVFrame* display;
    struct SwsContext* swsctx;
} FLV_Context;

typedef struct {
    char* data;
    int length;
} FLV_Frame;

FLV_Context* FLV_AllocContext(int width, int height, int format);
int FLV_DecodeFrame(FLV_Context* context, char* data, int length);
void FLV_ScaleFrame(FLV_Context* context);
void FLV_FreeContext(FLV_Context* context);

#endif