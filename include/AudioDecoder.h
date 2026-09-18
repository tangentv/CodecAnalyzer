#include <iostream>

#pragma once

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
}

class AudioDecoder
{
public:
    AudioDecoder();
    ~AudioDecoder();

    bool open(AVCodecParameters* codecParameters);
    AVFrame* decode(AVPacket* packet);
    void close();

private:
    const AVCodec* codec_;
    AVCodecContext* codecContext_;
    AVFrame* frame_;
};