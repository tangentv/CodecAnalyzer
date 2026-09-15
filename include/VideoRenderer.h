#pragma once

extern "C"
{
#include <libavutil/frame.h>
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
    void render(AVFrame* frame);
    bool processEvents();
    void close();

private:
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    SDL_Texture* texture_;

    int width_;
    int height_;
};