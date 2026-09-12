cmake_minimum_required(VERSION 3.16)

project(codec)

set(CMAKE_CXX_STANDARD 17)

add_executable(codec
    src/main.cpp
    src/StreamInfo.cpp
    src/MediaAnalyzer.cpp
)

target_include_directories(codec PRIVATE
    /opt/homebrew/opt/ffmpeg/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_directories(codec PRIVATE
    /opt/homebrew/opt/ffmpeg/lib
)

target_link_libraries(codec PRIVATE
    avformat
    avcodec
    avutil
    swscale
    swresample
)

## added to check for memory leak in the code
target_compile_options(codec PRIVATE
    -fsanitize=address
    -fno-omit-frame-pointer
)

target_link_options(codec PRIVATE
    -fsanitize=address
)

# Copy media folder to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/media
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})