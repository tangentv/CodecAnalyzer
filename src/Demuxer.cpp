#include "Demuxer.h"

Demuxer::Demuxer(): formatContext_(nullptr),
                    videoStreamIndex_(-1),
                    audioStreamIndex_(-1)
{
}

Demuxer::~Demuxer()
{
    close();
}

bool Demuxer::open(const std::string &filename)
{
    int inputStream = avformat_open_input(&formatContext_,filename.c_str(),nullptr,nullptr);
    if (inputStream < 0)
    {
        char errorBuffer[AV_ERROR_MAX_STRING_SIZE];
        av_strerror(inputStream,errorBuffer,sizeof(errorBuffer));
        std::cerr << "Could not open file: "
                  << errorBuffer
                  << '\n';

        return false;
    }

    inputStream = avformat_find_stream_info(formatContext_,nullptr);

    if (inputStream < 0)
    {
        std::cerr<< "Could not find stream information\n";
        close();
        return false;
    }

    // Find video and audio streams
    for (unsigned int i = 0;i < formatContext_->nb_streams;++i)
    {
        AVCodecParameters* codecParams = formatContext_->streams[i]->codecpar;

        if (codecParams->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            if (videoStreamIndex_ == -1)
            {
                videoStreamIndex_ = static_cast<int>(i);
            }
        }
        else if (codecParams->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            if (audioStreamIndex_ == -1)
            {
                audioStreamIndex_ = static_cast<int>(i);
            }
        }
    }

    std::cout << "Video stream index: "<< videoStreamIndex_<< '\n';

    std::cout << "Audio stream index: "<< audioStreamIndex_<< '\n';
    return true;
}

bool Demuxer::readPacket(AVPacket *packet)
{
    if (!formatContext_ || !packet)
    {
        return false;
    }

    int nextFramOfStream = av_read_frame(formatContext_,packet
    );

    if (nextFramOfStream < 0)
    {
        return false;
    }

    return true;
}

void Demuxer::close()
{
    if (formatContext_)
    {
        avformat_close_input(&formatContext_);
    }

    videoStreamIndex_ = -1;
    audioStreamIndex_ = -1;
}

AVFormatContext *Demuxer::getFormatContext()
{
    return formatContext_;
}

int Demuxer::getVideoStreamIndex() const
{
    return videoStreamIndex_;
}

int Demuxer::getAudioStreamIndex() const
{
    return audioStreamIndex_;
}
