#pragma once

#include <cstdint>

extern "C"
{
#include <libavutil/frame.h>
#include <libavutil/rational.h>
}

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

class VideoRenderer
{
public:
    VideoRenderer();
    ~VideoRenderer();

    bool initialize(int width, int height);
    void render(AVFrame* frame, AVRational timeBase);
    bool processEvents();
    void close();

private:
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    SDL_Texture* texture_;

    int width_;
    int height_;
    double firstPresentationTime_;

    uint64_t startTime_;
    bool clockStarted_;
};