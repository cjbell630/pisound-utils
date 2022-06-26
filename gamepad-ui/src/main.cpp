#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

#include "../include/drc/streamer.h"


using namespace std;

int main(int, char **) {


    vector<drc::byte> *pixels = new vector<drc::byte>(1024 * 1024 * 4);

    Streamer* streamer = new Streamer();
    streamer->Start();

    // main loop
    bool running = true;
    while (running) {
        streamer->PushVidFrame(pixels, 1024, 1024, drc::PixelFormat::kRGBA, Streamer::FlippingMode::NoFlip, Streamer::StretchMode::NoStretch);
    }



    return 0;
}