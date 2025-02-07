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
    AVFormatContext* formatCtx = nullptr; // FFmpeg 格式上下文
    AVCodecContext* codecCtx = nullptr; // FFmpeg 编解码器上下文，存储H.264编码器的参数
    AVStream* videoStream = nullptr; // 视频流，管理帧数据
    SwsContext* swsCtx = nullptr; // 颜色空间转换上下文
    AVFrame* avFrame = nullptr; // FFmpeg 帧数据，存储YUV数据
};


#endif //ENCODER_HPP
