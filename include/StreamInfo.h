#pragma once

#include <string>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/dict.h>
#include <libavutil/pixdesc.h>
}

class StreamInfo
{
public:

    explicit StreamInfo();

    void getMediaInfo(AVFormatContext* formatContext);
    void getCodecInfo(AVFormatContext* formatContext);

    void close(AVFormatContext* formatContext);

private:

    //AVFormatContext* formatContext_;
};