#include "AudioDecoder.h"

AudioDecoder::AudioDecoder(): codec_(nullptr),
      codecContext_(nullptr),
      frame_(nullptr)
{
}

AudioDecoder::~AudioDecoder()
{
    close();
}

bool AudioDecoder::open(AVCodecParameters *codecParameters)
{
    if (!codecParameters)
    {
        return false;
    }

    codec_ = avcodec_find_decoder(
        codecParameters->codec_id
    );

    if (!codec_)
    {
        std::cerr << "Audio decoder not found\n";
        return false;
    }

    codecContext_ = avcodec_alloc_context3(codec_);

    if (!codecContext_)
    {
        std::cerr << "Failed to allocate audio codec context\n";
        return false;
    }

    int ret = avcodec_parameters_to_context(
        codecContext_,
        codecParameters
    );

    if (ret < 0)
    {
        std::cerr << "Failed to copy audio codec parameters\n";
        close();
        return false;
    }

    ret = avcodec_open2(
        codecContext_,
        codec_,
        nullptr
    );

    if (ret < 0)
    {
        std::cerr << "Failed to open audio decoder\n";
        close();
        return false;
    }

    frame_ = av_frame_alloc();

    if (!frame_)
    {
        std::cerr << "Failed to allocate audio frame\n";
        close();
        return false;
    }

    std::cout << "Audio decoder opened: "
              << codec_->name
              << '\n';

    return true;
}

AVFrame *AudioDecoder::decode(AVPacket *packet)
{
    if (!packet || !codecContext_)
    {
        return nullptr;
    }

    int ret = avcodec_send_packet(
        codecContext_,
        packet
    );

    if (ret < 0)
    {
        std::cerr << "Error sending audio packet to decoder\n";
        return nullptr;
    }

    while (true)
    {
        ret = avcodec_receive_frame(
            codecContext_,
            frame_
        );

        if (ret == AVERROR(EAGAIN) ||
            ret == AVERROR_EOF)
        {
            return nullptr;
        }

        if (ret < 0)
        {
            std::cerr << "Error receiving decoded audio frame\n";
            return nullptr;
        }

        const char* sampleFormat =
            av_get_sample_fmt_name(
                static_cast<AVSampleFormat>(frame_->format)
            );

        std::cout << "Decoded audio frame\n";

        std::cout << "  Sample Rate : "
                  << frame_->sample_rate
                  << '\n';

        std::cout << "  Channels    : "
                  << frame_->ch_layout.nb_channels
                  << '\n';

        std::cout << "  Sample Fmt  : "
                  << (sampleFormat ? sampleFormat : "unknown")
                  << '\n';

        std::cout << "  Samples     : "
                  << frame_->nb_samples
                  << '\n';

        std::cout << "  PTS         : "
                  << frame_->pts
                  << '\n';

        return frame_;
    }
}

void AudioDecoder::close()
{
    if (frame_)
    {
        av_frame_free(&frame_);
    }

    if (codecContext_)
    {
        avcodec_free_context(&codecContext_);
    }

    codec_ = nullptr;
}
