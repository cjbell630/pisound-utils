#ifndef GAMEPAD_UI_STREAMER_H
#define GAMEPAD_UI_STREAMER_H

#include <vector>
#include <string>
#include "types.h"
#include "pixel-format.h"
#include "input.h"
#include "video-frame-rate.h"

using namespace drc;

class Streamer {
public:
    Streamer();
    bool Start();

    void Stop();

    void ResyncStreamer();

    // Data streaming/receiving methods. These methods are used for the core
    // features of the gamepad: video streaming, audio streaming and input
    // polling.

    // Takes ownership of the frame.
    enum FlippingMode {
        NoFlip,
        FlipVertically
    };
    enum StretchMode {
        NoStretch,
        StretchKeepAspectRatio,
        StretchFull
    };

    void PushVidFrame(std::vector<byte> *frame, u16 width, u16 height,
                      PixelFormat pixfmt, FlippingMode flip = NoFlip,
                      StretchMode stretch = StretchKeepAspectRatio);

    // Same as PushVidFrame, but the frame needs to already be in the native
    // format for encoding: YUV420P at ScreenWidth x ScreenHeight.
    //
    // Faster: PushVidFrame requires pixel format conversion before encoding.
    void PushNativeVidFrame(std::vector<u8> *frame);

    void PushH264Chunks(std::vector<std::string> &&chunks);

    // Expects 48KHz samples.
    void PushAudSamples(const std::vector<s16> &samples);

    // Gets the most recent input data received from the Gamepad. Usually
    // refreshed at 180Hz.
    void PollInput(InputData *data);

    // Enables system input feeding, which sends input data received from the
    // GamePad directly to the operating system. This allows an application to
    // use the basic GamePad input data transparently, without any code change.
    void EnableSystemInputFeeder();

    void SetFrameRate(VideoFrameRate frame_rate);

    // Sets the mode the rendering is in so returned coordinates are expressed
    // in terms of source pixels. This is used to simplify input for the
    // modes where StretchFull is not used.

    void SetTSArea(u16 width, u16 height,
                   StretchMode stretch = StretchKeepAspectRatio);

    // More minor features are exposed through the following methods. These
    // methods provide a "bool wait" argument in order to wait for the change to
    // actually be applied. If waiting is disabled (default), these methods are
    // not guaranteed to succeed. Waiting can block for up to 10s.

    // Level must be in [0;4] (0 is minimum, 4 is maximum).
    bool SetLcdBacklight(int level, bool wait = false);

    // Retrieves the device's config from UIC
    bool SyncUICConfig(bool wait = false);

    // Attempts to shutdown the pad, not guaranteed to work.
    void ShutdownPad();

private:
    SDL_Renderer *renderer;
    SDL_Window *window;
    bool running;
    bool useLockTexture;
    SDL_RendererInfo info;
    SDL_Texture *texture;
};


#endif //GAMEPAD_UI_STREAMER_H
