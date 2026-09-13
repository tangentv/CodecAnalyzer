#include "StreamInfo.h"

#include <iostream>
#include <iomanip>

StreamInfo::StreamInfo()
{
}

void StreamInfo::printMediaInfo(AVFormatContext* formatContext, std::string filename_)
{
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "          MEDIA INFORMATION             \n";
    std::cout << "========================================\n";

    std::cout << "File       : "<< filename_<< '\n';
    std::cout << "Format     : "<< formatContext->iformat->long_name<< '\n';

    if (formatContext->duration != AV_NOPTS_VALUE)    {
        double duration =static_cast<double>(formatContext->duration)/ AV_TIME_BASE;
        std::cout << "Duration   : "<< duration<< " seconds\n";
    }

    if (formatContext->bit_rate > 0)
    {
        std::cout << "Bitrate    : "<< formatContext->bit_rate<< " bps\n";
    }
    std::cout << "Streams    : "<< formatContext->nb_streams<< "\n\n";
}



void StreamInfo::printStreamInfo(AVFormatContext* formatContext)
{
for (unsigned int i = 0;i < formatContext->nb_streams;++i)
    {
        AVStream* stream = formatContext->streams[i];
        AVCodecParameters* codecParams = stream->codecpar;
        std::cout << "----------------------------------------\n";
        std::cout << "Stream #" << i << '\n';
        std::cout << "Type       : "<< av_get_media_type_string(codecParams->codec_type)<< '\n';
        std::cout << "Codec      : "<< avcodec_get_name(codecParams->codec_id)<< '\n';
        if (codecParams->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            std::cout << "Resolution : "<< codecParams->width<< " x "<< codecParams->height<< '\n';
            const char* pixelFormat =av_get_pix_fmt_name(static_cast<AVPixelFormat>(codecParams->format));
            if (pixelFormat)
            {
                std::cout << "Pixel Fmt  : "<< pixelFormat<< '\n';
            }

            if (stream->avg_frame_rate.den != 0)
            {
                double fps =av_q2d(stream->avg_frame_rate);
                std::cout << "Frame Rate : "<< std::fixed<< std::setprecision(2)<< fps<< " FPS\n";
            }

            if (codecParams->bit_rate > 0)
            {
                std::cout << "Bitrate    : "<< codecParams->bit_rate<< " bps\n";
            }
        }

        if (codecParams->codec_type ==  AVMEDIA_TYPE_AUDIO)
        {
            std::cout << "Sample Rate: "<< codecParams->sample_rate<< " Hz\n";
            std::cout << "Channels   : "<< codecParams->ch_layout.nb_channels<< '\n';

            if (codecParams->bit_rate > 0)
            {
                std::cout << "Bitrate    : "<< codecParams->bit_rate<< " bps\n";
            }
        }
        std::cout << '\n';
    }
}

void StreamInfo::close(AVFormatContext* formatContext)
{
    avformat_close_input(&formatContext);
}