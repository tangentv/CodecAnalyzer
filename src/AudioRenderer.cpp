#include "AudioRenderer.h"

extern "C"
{
#include <libavutil/channel_layout.h>
#include <libavutil/samplefmt.h>
}

AudioRenderer::AudioRenderer()
                : audioDevice_(0),
      sampleRate_(0),
      channels_(0),
      swrContext_(nullptr)
{
}

AudioRenderer::~AudioRenderer()
{
    close();
}

bool AudioRenderer::initialize(int sampleRate, int channels)
{
    sampleRate_ = sampleRate;
    channels_ = channels;

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
    {
        std::cerr
            << "SDL audio initialization failed: "
            << SDL_GetError()
            << '\n';

        return false;
    }

    SDL_AudioSpec desiredSpec{};

    desiredSpec.freq = sampleRate_;
    desiredSpec.format = AUDIO_F32SYS;
    desiredSpec.channels =
        static_cast<Uint8>(channels_);
    desiredSpec.samples = 1024;

    audioDevice_ = SDL_OpenAudioDevice(
        nullptr,
        0,
        &desiredSpec,
        nullptr,
        0
    );

    if (audioDevice_ == 0)
    {
        std::cerr
            << "Failed to open SDL audio device: "
            << SDL_GetError()
            << '\n';

        return false;
    }

    /*
    --------------------------------------------------
    Initialize FFmpeg audio resampler
    --------------------------------------------------
    */

    AVChannelLayout inputLayout;

    av_channel_layout_default(
        &inputLayout,
        channels_
    );

    AVChannelLayout outputLayout;

    av_channel_layout_default(
        &outputLayout,
        channels_
    );

    swrContext_ = nullptr;

    int ret = swr_alloc_set_opts2(
        &swrContext_,
        &outputLayout,
        AV_SAMPLE_FMT_FLT,
        sampleRate_,
        &inputLayout,
        AV_SAMPLE_FMT_FLTP,
        sampleRate_,
        0,
        nullptr
    );

    av_channel_layout_uninit(&inputLayout);
    av_channel_layout_uninit(&outputLayout);

    if (ret < 0 || !swrContext_)
    {
        std::cerr
            << "Failed to create audio resampler\n";

        close();

        return false;
    }

    ret = swr_init(swrContext_);

    if (ret < 0)
    {
        std::cerr
            << "Failed to initialize audio resampler\n";

        close();

        return false;
    }

    SDL_PauseAudioDevice(audioDevice_, 0);

    std::cout
        << "SDL audio initialized\n";

    std::cout
        << "Sample Rate : "
        << sampleRate_
        << '\n';

    std::cout
        << "Channels    : "
        << channels_
        << '\n';

    return true;
}

void AudioRenderer::play(AVFrame *frame)
{
    if (!frame || !swrContext_ || audioDevice_ == 0)
    {
        return;
    }

    int outputSamples =
        av_rescale_rnd(
            swr_get_delay(
                swrContext_,
                frame->sample_rate
            ) + frame->nb_samples,
            sampleRate_,
            frame->sample_rate,
            AV_ROUND_UP
        );

    uint8_t* outputBuffer = nullptr;

    int outputBufferSize =
        av_samples_alloc(
            &outputBuffer,
            nullptr,
            channels_,
            outputSamples,
            AV_SAMPLE_FMT_FLT,
            0
        );

    if (outputBufferSize < 0)
    {
        std::cerr
            << "Failed to allocate audio output buffer\n";

        return;
    }

    int convertedSamples =
        swr_convert(
            swrContext_,
            &outputBuffer,
            outputSamples,
            const_cast<const uint8_t**>(
                frame->extended_data
            ),
            frame->nb_samples
        );

    if (convertedSamples < 0)
    {
        std::cerr
            << "Audio resampling failed\n";

        av_freep(&outputBuffer);

        return;
    }

    int bytesPerSample =
        av_get_bytes_per_sample(
            AV_SAMPLE_FMT_FLT
        );

    int bytesToQueue =
        convertedSamples *
        channels_ *
        bytesPerSample;

    SDL_QueueAudio(
        audioDevice_,
        outputBuffer,
        bytesToQueue
    );

    av_freep(&outputBuffer);
}

void AudioRenderer::close()
{
    if (audioDevice_ != 0)
    {
        SDL_ClearQueuedAudio(audioDevice_);

        SDL_CloseAudioDevice(audioDevice_);

        audioDevice_ = 0;
    }

    if (swrContext_)
    {
        swr_free(&swrContext_);
    }

    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}
