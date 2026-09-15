# CodecAnalyzer
It is a mini-Media Info. It reads a video and audio file and prints details about the streams, including the exact codec used ( e.g. H.264, HEVC, AAC), profile, level, bitrate, chroma and subsampling


Build dteps:

1. cd build
2. cmake ..
3. cmake --build . -j.   OR     cmake --build build

## Screenshots

### Codec Analysis

![Codec Analysis](screenshots/codec-analyzer-output.png)


![Display of the first frame on the screen](screenshots/Display%20of%20the%20first%20frame%20on%20the%20screen.png)
              

* The basec FFmpeg flow is :

                Input file
                    │
                    ▼
                avformat_open_input()
                    │
                    ▼
                AVFormatContext
                    │
                    ├── Container information
                    │
                    └── Streams
                        │
                        ├── Video → AVCodecParameters
                        │
                        ├── Audio → AVCodecParameters
                        │
                        └── Subtitle / Data