#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <iostream>
#include <algorithm>
#include <chrono>
#include "../include/drc/streamer.h"

using namespace std;

void PrintFrameTiming(ostream &os = cout, float period = 2.0f) {
    static unsigned int frames = 0;
    frames++;
    static auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();

    float seconds = chrono::duration_cast<chrono::duration<float> >(end - start).count();
    if (seconds > period) {
        float spf = seconds / frames;
        os << frames << " frames in " << setprecision(1) << fixed << seconds << " seconds = " << setprecision(1)
           << fixed << 1.0f / spf << " FPS (" << setprecision(3) << fixed << spf * 1000.0f << " ms/frame)\n";
        frames = 0;
        start = end;
    }
}

bool Streamer::Start() {
    return true;
}

void Streamer::PushVidFrame(std::vector<byte> *frame, u16 width, u16 height, PixelFormat pixfmt, Streamer::FlippingMode flip,
                       Streamer::StretchMode stretch) {

    vector<unsigned char> pixels(width * height * 4, 0);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // handle events
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if ((SDL_QUIT == ev.type) ||
            (SDL_KEYDOWN == ev.type && SDL_SCANCODE_ESCAPE == ev.key.keysym.scancode)) {
            running = false;
            break;
        }

        if (SDL_KEYDOWN == ev.type && SDL_SCANCODE_L == ev.key.keysym.scancode) {
            useLockTexture = !useLockTexture;
            cout << "Using " << (useLockTexture ? "SDL_LockTexture() + copy_n()" : "SDL_UpdateTexture()") << '\n';
        }
    }

    // splat down some random pixels
    for (unsigned int i = 0; i < 1000; i++) {
        const unsigned int x = rand() % width;
        const unsigned int y = rand() % height;

        const unsigned int offset = (width * y * 4) + x * 4;
        pixels[offset + 0] = rand() % 256;        // b
        pixels[offset + 1] = rand() % 256;        // g
        pixels[offset + 2] = rand() % 256;        // r
        pixels[offset + 3] = SDL_ALPHA_OPAQUE;    // a
    }

    // update texture
    if (this->useLockTexture) {
        unsigned char *lockedPixels = nullptr;
        int pitch = 0;
        SDL_LockTexture(texture, nullptr, reinterpret_cast< void ** >( &lockedPixels ), &pitch);
        copy_n(pixels.data(), pixels.size(), lockedPixels);
        SDL_UnlockTexture(texture);
    } else {
        SDL_UpdateTexture(texture, nullptr, pixels.data(), width * 4);
    }

    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);

    PrintFrameTiming();
}

void Streamer::Stop() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}

Streamer::Streamer() {

    SDL_Init(SDL_INIT_EVERYTHING);
    cout << "gaming1" << '\n'<< flush;
    window = SDL_CreateWindow(
            "SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, SDL_WINDOW_SHOWN
    );
    cout << "gaming2" << '\n'<< flush;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    cout << "gaming3" << '\n'<< flush;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    cout << "gaming4" << '\n'<< flush;
    useLockTexture = false;

    // dump renderer info
    SDL_GetRendererInfo(renderer, &info);
    cout << "Renderer name: " << info.name << '\n'<< flush;
    cout << "Texture formats: " << '\n'<< flush;
    for (Uint32 i = 0; i < info.num_texture_formats; i++) {
        cout << SDL_GetPixelFormatName(info.texture_formats[i]) << '\n'<< flush;
    }

    // create texture
    const unsigned int texWidth = 1024;
    const unsigned int texHeight = 1024;
    texture = SDL_CreateTexture(
            renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, texWidth, texHeight
    );
}
