#include "VideoRenderer.h"

#include <iostream>

#include <SDL2/SDL.h>

VideoRenderer::VideoRenderer(): window_(nullptr),
      renderer_(nullptr),
      texture_(nullptr),
      width_(0),
      height_(0)
{
}

VideoRenderer::~VideoRenderer()
{
    close();
}

bool VideoRenderer::initialize(int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL initialization failed: "
                  << SDL_GetError()
                  << '\n';

        return false;
    }

    width_ = width;
    height_ = height;

    window_ = SDL_CreateWindow(
        "CodecAnalyzer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width_,
        height_,
        SDL_WINDOW_SHOWN
    );

    if (!window_)
    {
        std::cerr << "Failed to create SDL window: "
                  << SDL_GetError()
                  << '\n';

        SDL_Quit();
        return false;
    }

    renderer_ = SDL_CreateRenderer(
        window_,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (!renderer_)
    {
        std::cerr << "Failed to create SDL renderer: "
                  << SDL_GetError()
                  << '\n';

        close();
        return false;
    }

    texture_ = SDL_CreateTexture(
    renderer_,
    SDL_PIXELFORMAT_IYUV,
    SDL_TEXTUREACCESS_STREAMING,
    width_,
    height_
   );

    if (!texture_)
    {
        std::cerr << "Failed to create SDL texture: "
                << SDL_GetError()
                << '\n';

        close();
        return false;
    }

    std::cout << "SDL initialized successfully\n";
    std::cout << "SDL window created: "
              << width_ << "x" << height_
              << '\n';

    return true;
}

void VideoRenderer::render(AVFrame* frame)
{
    if (!frame || !texture_)
    {
        return;
    }

    SDL_UpdateYUVTexture(
        texture_,
        nullptr,

        frame->data[0],
        frame->linesize[0],

        frame->data[1],
        frame->linesize[1],

        frame->data[2],
        frame->linesize[2]
    );

    SDL_RenderClear(renderer_);

    SDL_RenderCopy(
        renderer_,
        texture_,
        nullptr,
        nullptr
    );

    SDL_RenderPresent(renderer_);
    
}

void VideoRenderer::close()
{
   if (texture_)
    {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }

    if (renderer_)
    {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }

    if (window_)
    {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }

    SDL_Quit();
}

bool VideoRenderer::processEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            return false;
        }
    }

    SDL_Delay(10);
    return true;
}
