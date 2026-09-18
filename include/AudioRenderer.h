#pragma once

#include <cstdint>
#include <iostream>
#include <SDL2/SDL.h>

extern "C"
{
#include <libavutil/frame.h>
#include <libswresample/swresample.h>
}

class AudioRenderer
{
public:
    AudioRenderer();
    ~AudioRenderer();

    bool initialize(int sampleRate, int channels);

    void play(AVFrame* frame);

    void close();

private:
    SDL_AudioDeviceID audioDevice_;
    int sampleRate_;
    int channels_;
    SwrContext* swrContext_;
};