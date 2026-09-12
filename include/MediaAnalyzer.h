/*
avformat_open_input()
avformat_find_stream_info()
avformat_close_input()

avcodec_find_decoder()
avcodec_parameters_to_context()

av_dict_get()
av_get_media_type_string()
avcodec_get_name()
avcodec_profile_name()
av_get_pix_fmt_name()
*/

#pragma once

#include <string>
#include "StreamInfo.h"

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/dict.h>
#include <libavutil/pixdesc.h>
}

class MediaAnalyzer
{
public:

    explicit MediaAnalyzer(const std::string& filename);

    bool analyze();

    void closeAnalyzer();

private:

    std::string filename_;
    StreamInfo* streamInfo;
    AVFormatContext* formatContext;
};