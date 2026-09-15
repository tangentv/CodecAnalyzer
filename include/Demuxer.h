#pragma once

#include <iostream>
#include <string>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/dict.h>
#include <libavutil/pixdesc.h>
}


class Demuxer
{
public:

    Demuxer();
    ~Demuxer();

    bool open(const std::string& filename);

    bool readPacket(AVPacket* packet);

    void close();

    AVFormatContext* getFormatContext();

    int getVideoStreamIndex() const;

    int getAudioStreamIndex() const;

private:

    AVFormatContext* formatContext_ = nullptr;

    int videoStreamIndex_;
    int audioStreamIndex_;
};