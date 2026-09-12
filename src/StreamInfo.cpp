#include "StreamInfo.h"

#include <iostream>
#include <iomanip>

StreamInfo::StreamInfo()
{
}

void StreamInfo::getMediaInfo(AVFormatContext* formatContext)
{
    std::cout << "Format : "<< formatContext->iformat->long_name<< '\n';
    std::cout << "Duration : "<< formatContext->duration / AV_TIME_BASE<< " seconds\n";
    std::cout << "Bitrate : "<< formatContext->bit_rate<< " bps\n";
    std::cout << "Streams : "<< formatContext->nb_streams<< "\n\n";
}



void StreamInfo::getCodecInfo(AVFormatContext* formatContext)
{
for (unsigned int i = 0;i < formatContext->nb_streams;++i)
    {
        AVStream* stream = formatContext->streams[i];
        AVCodecParameters* codecParams = stream->codecpar;

        std::cout << "--------------------------------------\n";

        std::cout << "Stream #" << i << '\n';
        std::cout << "Type : "<< av_get_media_type_string(codecParams->codec_type)<< '\n';
        std::cout << "Codec : "<< avcodec_get_name(codecParams->codec_id)<< '\n';
        std::cout << "Codec ID : "<< codecParams->codec_id<< '\n';
        std::cout << "Bitrate : "<< codecParams->bit_rate<< " bps\n";

        if (codecParams->codec_type ==AVMEDIA_TYPE_VIDEO)
        {
            std::cout << "Resolution : "<< codecParams->width<< " x "<< codecParams->height<< '\n';
            std::cout << "Pixel Format : "<< av_get_pix_fmt_name(static_cast<AVPixelFormat>(codecParams->format))<< '\n';// declares in pixdesc.h

            if (stream->avg_frame_rate.den != 0) {
                double fps = av_q2d(stream->avg_frame_rate);
                std::cout << "Frame Rate : "<< fps<< " fps\n";
            }
        }

        if (codecParams->codec_type ==AVMEDIA_TYPE_AUDIO)
        {
            std::cout << "Sample Rate : "<< codecParams->sample_rate<< " Hz\n";
            std::cout << "Channels : "<< codecParams->ch_layout.nb_channels<< '\n';
        }
    }
}

void StreamInfo::close(AVFormatContext* formatContext)
{
    avformat_close_input(&formatContext);
}