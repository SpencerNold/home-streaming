#include "frame.h"

#include <libavutil/frame.h>
#include <libavutil/imgutils.h>

#include <stdlib.h>

FLV_Context* FLV_AllocContext(int width, int height, int format) {
    const AVCodec* codec = avcodec_find_decoder(format);
    if (codec == NULL) {
        return NULL;
    }
    AVCodecContext* avctx = avcodec_alloc_context3(codec);
    if (avctx == NULL) {
        return NULL;
    }
    AVPacket* packet = av_packet_alloc();
    if (packet == NULL) {
        avcodec_free_context(&avctx);
        return NULL;
    }
    if (avcodec_open2(avctx, codec, NULL) < 0) {
        avcodec_free_context(&avctx);
        av_packet_free(&packet);
        return NULL;
    }
    AVFrame* frame = av_frame_alloc();
    if (frame == NULL) {
        avcodec_free_context(&avctx);
        av_packet_free(&packet);
        return NULL;
    }
    AVFrame* display = av_frame_alloc();
    if (display == NULL) {
        avcodec_free_context(&avctx);
        av_packet_free(&packet);
        av_frame_free(&frame);
        return NULL;
    }
    int num_bytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, width, height, 1);
    uint8_t* buffer = (uint8_t*) av_malloc(num_bytes * sizeof(uint8_t));
    av_image_fill_arrays(display->data, display->linesize, buffer, AV_PIX_FMT_RGB24, width, height, 1);
    struct SwsContext* swsctx = sws_getContext(width, height, AV_PIX_FMT_YUV420P, width, height, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);
    if (swsctx == NULL) {
        avcodec_free_context(&avctx);
        av_packet_free(&packet);
        av_frame_free(&frame);
        av_frame_free(&display);
        return NULL;
    }
    FLV_Context* context = (FLV_Context*) malloc(sizeof(FLV_Context));
    if (context == NULL) {
        avcodec_free_context(&avctx);
        av_packet_free(&packet);
        av_frame_free(&frame);
        av_frame_free(&display);
        sws_freeContext(swsctx);
        return NULL;
    }
    context->avctx = avctx;
    context->packet = packet;
    context->frame = frame;
    context->display = display;
    context->swsctx = swsctx;
    context->width = width;
    context->height = height;
    return context;
}

int FLV_DecodeFrame(FLV_Context* context, char* data, int length) {
    context->packet->data = (uint8_t*) data;
    context->packet->size = length;
    if (avcodec_send_packet(context->avctx, context->packet) < 0) {
        return -1;
    }
    int res = avcodec_receive_frame(context->avctx, context->frame);
    if (res == AVERROR(EAGAIN) || res == AVERROR_EOF) {
        return 1;
    }
    if (res < 0) {
        return -2;
    }
    return 0;
}

void FLV_ScaleFrame(FLV_Context* context) {
    sws_scale(context->swsctx, (uint8_t const * const *) context->frame->data, context->frame->linesize, 0, context->height, context->display->data, context->display->linesize);
}

void FLV_FreeContext(FLV_Context* context) {
    avcodec_free_context(&context->avctx);
    av_packet_free(&context->packet);
    av_frame_free(&context->frame);
    av_frame_free(&context->display);
    sws_freeContext(context->swsctx);
    free(context);
}