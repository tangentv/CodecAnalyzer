#include <iostream>
#include "stdio.h"
#include "MediaAnalyzer.h"

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

    if (argc < 2) {
        std::cerr << "Usage: codec_analyzer <media_file>\n";
        return 1;
    }

    const char* filename = argv[1];

    MediaAnalyzer* mediaAnalyzer = new MediaAnalyzer(filename);
    bool result = mediaAnalyzer->analyze();

    std::cout << "\n========================================\n";
    return 0;
}