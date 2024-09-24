#ifndef FLV_H
#define FLV_H

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>

typedef struct {
    AVFormatContext* fmtctx;
    AVCodecContext* avctx;
    AVPacket* packet;
    int videoStreamIndex;
    int audioStreamIndex;
} FLV;

typedef struct {
    char* data;
    int length;
    int type;
    int error;
} PACKET;

FLV* flv_open() {
    AVFormatContext* fmtctx = NULL;
    int result = avformat_open_input(&fmtctx, "backend/movies/flv/movie.flv", NULL, NULL);
    if (result < 0) {
        return NULL;
    }
    if (avformat_find_stream_info(fmtctx, NULL) < 0) {
        avformat_close_input(&fmtctx);
        return NULL;
    }
    int videoStreamIndex;
    int audioStreamIndex;
    for (int i = 0; i < fmtctx->nb_streams; i++) {
        AVCodecParameters* codecpar = fmtctx->streams[i]->codecpar;
        if (codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
        }
        if (codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioStreamIndex = i;
        }
    }
    AVCodecParameters* codecpar = fmtctx->streams[videoStreamIndex]->codecpar;
    AVCodec* codec = avcodec_find_decoder(codecpar->codec_id);
    if (codec == NULL) {
        return NULL;
    }
    AVCodecContext* context = avcodec_alloc_context3(codec);
    if (context == NULL) {
        return NULL;
    }
    if (avcodec_parameters_to_context(context, codecpar) < 0) {
        return NULL;
    }
    if (avcodec_open2(context, codec, NULL) < 0) {
        return NULL;
    }
    AVPacket* packet = av_packet_alloc();
    if (packet == NULL) {
        return NULL;
    }
    FLV* flv = (FLV*) malloc(sizeof(FLV));
    flv->fmtctx = fmtctx;
    flv->avctx = context;
    flv->packet = packet;
    return flv;
}

void flv_read_packet(FLV* flv, PACKET* packet) {
    packet->error = 0;
    if (av_read_frame(flv->fmtctx, flv->packet) < 0) {
        packet->error = 1;
    }
    packet->data = flv->packet->data;
    packet->length = flv->packet->size;
    packet->type = flv->packet->stream_index;
}

#endif