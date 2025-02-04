//
// Created by Jade on 25-2-5.
//

#ifndef ENCODER_HPP
#define ENCODER_HPP

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

#include "ScreenCapturer.hpp"
#include <string>

class Encoder {
public:
    Encoder();
    ~Encoder();

    bool initialize(const std::string& filename, int width, int height);
    bool encodeFrame(FrameData& frame);
    void finalize();

private:
    AVFormatContext* formatCtx = nullptr;
    AVCodecContext* codecCtx = nullptr;
    AVStream* videoStream = nullptr;
    SwsContext* swsCtx = nullptr;
    AVFrame* avFrame = nullptr;
};


#endif //ENCODER_HPP
