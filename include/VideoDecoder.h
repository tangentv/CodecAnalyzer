#pragma once

#include <iostream>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
#include <libavutil/pixdesc.h>
}

class VideoDecoder
{
public:

    VideoDecoder();
    ~VideoDecoder();

    bool open(AVCodecParameters* codecParameters);

    //bool decode(AVPacket* packet);
    AVFrame* decode(AVPacket* packet);

    void close();

    // AVFrame* getFrame() const;

private:

    const AVCodec* codec_;
    AVCodecContext* codecContext_;
    AVFrame* frame_;
    AVFrame* outputFrame_;
};