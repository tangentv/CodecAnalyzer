#include <iostream>
#include <iomanip>

#include "Demuxer.h"
#include "MediaAnalyzer.h"
#include "VideoDecoder.h"
#include "VideoRenderer.h"

/*
libavformat → opens MP4/MKV
libavcodec → decodes H.264/AAC
libavutil → utilities
libswscale → pixel conversion (later)
*/
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/dict.h>
#include <libavutil/pixdesc.h> 
}

int main(int argc, char* argv[]){
if (argc < 2)
    {
        std::cerr<< "Usage: codec_analyzer <media_file>\n";
        return 1;
    }

    const char* filename = argv[1];

    MediaAnalyzer mediaAnalyzer(filename);
    if(!mediaAnalyzer.analyze()){
        std::cerr << "Failed to analyze media file\n";
        return 1;
    }

    Demuxer demuxer;

    if (!demuxer.open(filename))
    {
        std::cerr<< "Failed to open media file\n";
        return 1;
    }

    int videoStreamIndex = demuxer.getVideoStreamIndex();
    if (videoStreamIndex == -1)
    {
        std::cerr << "No video stream found\n";
        return 1;
    }
    AVStream* videoStream = demuxer.getFormatContext()->streams[videoStreamIndex];

    VideoDecoder videoDecoder;

    AVPacket* packet = av_packet_alloc();

    if (!packet)
    {
        std::cerr << "Failed to allocate packet\n";
        return 1;
    }

    if (!videoDecoder.open(videoStream->codecpar))
    {
        std::cerr << "Failed to open video decoder\n";
        return 1; 
    }

    VideoRenderer renderer;

    if (!renderer.initialize(
            videoStream->codecpar->width,
            videoStream->codecpar->height))
    {
        std::cerr << "Failed to initialize video renderer\n";
        return 1;
    }

    int packetNumber = 0;

    while (demuxer.readPacket(packet))
    {
        if(!renderer.processEvents()){
            break;
        }

        const char* type = "Unknown";
        if (packet->stream_index == demuxer.getVideoStreamIndex())
        {
            //bool frameReceived = false;
            type = "VIDEO";
            AVFrame* frame = videoDecoder.decode(packet);
            if (frame)
            {
              renderer.render(frame, videoStream->time_base);
            }
        }
        else if(packet->stream_index == demuxer.getAudioStreamIndex()){
            type = "AUDIO";
            // We process/identify the audio packet.
            // AudioDecoder will be added later.
        }
        /*
        PTS = Presentation Timestamp ----> When should this frame be displayed?
        DTS = Decode Timestamp ----> When should this packet/frame be decoded?
        */
        if (packet->flags & AV_PKT_FLAG_KEY)
        {
            std::cout<< "  KEY_FRAME";
        }
        ++packetNumber;
        av_packet_unref(packet);
        if (!renderer.processEvents())
        {
            break;
        }
    }
    av_packet_free(&packet);

    return 0;
}