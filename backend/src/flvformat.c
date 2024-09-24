#include "flvformat.h"

#include <stdlib.h>

FLV_Stream* FLV_OpenStream(const char* name) {
    AVFormatContext* fmtctx = NULL;
    int res = avformat_open_input(&fmtctx, name, NULL, NULL);
    if (res < 0) {
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
        avformat_close_input(&fmtctx);
        return NULL;
    }
    AVCodecContext* avctx = avcodec_alloc_context3(codec);
    if (avctx == NULL) {
        avformat_close_input(&fmtctx);
        return NULL;
    }
    if (avcodec_parameters_to_context(avctx, codecpar) < 0) {
        avformat_close_input(&fmtctx);
        avcodec_free_context(&avctx);
        return NULL;
    }
    if (avcodec_open2(avctx, codec, NULL) < 0) {
        avformat_close_input(&fmtctx);
        avcodec_free_context(&avctx);
        return NULL;
    }
    AVPacket* packet = av_packet_alloc();
    if (packet == NULL) {
        avformat_close_input(&fmtctx);
        avcodec_free_context(&avctx);
        return NULL;
    }
    FLV_Stream* stream = (FLV_Stream*) malloc(sizeof(FLV_Stream));
    if (stream == NULL) {
        avformat_close_input(&fmtctx);
        avcodec_free_context(&avctx);
        av_packet_free(&packet);
        return NULL;
    }
    stream->fmtctx = fmtctx;
    stream->avctx = avctx;
    stream->packet = packet;
    return stream;
}

void FLV_ReadPacket(FLV_Stream* stream, FLV_Packet* packet) {
    packet->error = 0;
    if (av_read_frame(stream->fmtctx, stream->packet) < 0) {
        packet->error = 1;
    }
    packet->data = stream->packet->data;
    packet->length = stream->packet->size;
    packet->type = stream->packet->stream_index;
}

void FLV_CloseStream(FLV_Stream* stream) {
    avformat_close_input(&stream->fmtctx);
    avcodec_free_context(&stream->avctx);
    av_packet_free(&stream->packet);
    free(stream);
}