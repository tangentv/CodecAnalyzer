#include "VideoDecoder.h"

VideoDecoder::VideoDecoder(): codec_(nullptr),
      codecContext_(nullptr),
      frame_(nullptr),
      outputFrame_(nullptr)
{
}

VideoDecoder::~VideoDecoder()
{
    close();
}

bool VideoDecoder::open(AVCodecParameters *codecParameters)
{
    if (!codecParameters)
    {
        std::cerr<< "Invalid codec parameters\n";
        return false;
    }
    codec_ = avcodec_find_decoder(codecParameters->codec_id
    );

    if (!codec_)
    {
        std::cerr<< "Video decoder not found\n";
        return false;
    }

    std::cout<< "Video decoder: "<< codec_->name<< '\n';
    codecContext_ = avcodec_alloc_context3(codec_);

    if (!codecContext_)
    {
        std::cerr<< "Could not allocate codec context\n";
        return false;
    }
    int ret = avcodec_parameters_to_context(codecContext_,codecParameters
    );

    if (ret < 0)
    {
        std::cerr<< "Could not copy codec parameters\n";
        close();
        return false;
    }

    ret = avcodec_open2(codecContext_,codec_,nullptr);

    if (ret < 0)
    {
        std::cerr<< "Could not open video decoder\n";
        close();
        return false;
    }

    frame_ = av_frame_alloc();
    outputFrame_ = av_frame_alloc();

    if (!frame_)
    {
        std::cerr<< "Could not allocate AVFrame\n";
        close();
        return false;
    }

    return true;
}

AVFrame* VideoDecoder::decode(AVPacket *packet)
{
    //frameReceived = false;
    int ret = avcodec_send_packet(codecContext_,packet);

    if (ret < 0)
    {
        std::cerr<< "Error sending packet to decoder\n";
        return nullptr;
    }

    while (true)
    {
        ret = avcodec_receive_frame(codecContext_,frame_);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        {
            break;
        }

        if (ret < 0)
        {
            std::cerr<< "Error receiving decoded frame\n";
            return nullptr;
        }

        //frameReceived = true;
        av_frame_unref(outputFrame_);
        ret = av_frame_ref(outputFrame_, frame_);

        if (ret < 0)
        {
            std::cerr << "Could not reference decoded frame\n";
            return nullptr;
        }

        const char* pixelFormat = av_get_pix_fmt_name(static_cast<AVPixelFormat>(frame_->format));

        std::cout<< "Decoded video frame\n";
        std::cout<< "  Width     : "<< frame_->width<< '\n';
        std::cout<< "  Height    : "<< frame_->height<< '\n';
        std::cout<< "  Pixel Fmt : "<< (pixelFormat ? pixelFormat : "unknown")<< '\n';
        std::cout<< "  PTS       : "<< frame_->pts<< '\n';
        std::cout << '\n';

        std::cout << "  Data[0]   : "<< static_cast<void*>(frame_->data[0])<< '\n';
        std::cout << "  Data[1]   : "<< static_cast<void*>(frame_->data[1])<< '\n';
        std::cout << "  Data[2]   : "<< static_cast<void*>(frame_->data[2])<< '\n';
        std::cout << "  Linesize[0]: "<< frame_->linesize[0]<< '\n';

        return outputFrame_;
    }

    return nullptr;
}

void VideoDecoder::close()
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

// AVFrame *VideoDecoder::getFrame() const
// {
//     return frame_;
// }
