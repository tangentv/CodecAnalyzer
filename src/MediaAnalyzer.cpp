#include "MediaAnalyzer.h"

#include <iostream>
#include <iomanip>

MediaAnalyzer::MediaAnalyzer(const std::string& filename)
    : filename_(filename)
{    
    formatContext = nullptr;
    streamInfo = new StreamInfo();
}

bool MediaAnalyzer::analyze(){

    AVFormatContext* formatContext = nullptr;

    int ret = avformat_open_input(&formatContext,filename_.c_str(),nullptr,nullptr);

     if (ret < 0) {
        char errorBuffer[AV_ERROR_MAX_STRING_SIZE];
        av_strerror(ret,errorBuffer,sizeof(errorBuffer));
        std::cerr << "Could not open file: "<< errorBuffer << '\n';
        return false;
    }

    ret = avformat_find_stream_info(formatContext,nullptr);

    if (ret < 0) {
        std::cerr << "Could not find stream information\n";
        closeAnalyzer();
        return false;
    }

    streamInfo->getCodecInfo(formatContext);
    streamInfo->getMediaInfo(formatContext);

    return true;

}

void MediaAnalyzer::closeAnalyzer()
{
    avformat_close_input(&formatContext);
}