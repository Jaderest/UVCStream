//
// Created by Jade on 25-2-5.
//

#include "Encoder.hpp"
#include <iostream>

Encoder::Encoder() {}

Encoder::~Encoder() {
    finalize();
}

bool Encoder::initialize(const std::string& filename, int width, int height) {
    avformat_alloc_output_context2(&formatCtx, nullptr, "mp4", filename.c_str());
    if (!formatCtx) {
        std::cerr << "Failed to create output format context\n";
        return false;
    }

    const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        std::cerr << "H.264 codec not found\n";
        return false;
    }

    codecCtx = avcodec_alloc_context3(codec);
    codecCtx->width = width;
    codecCtx->height = height;
    codecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    codecCtx->time_base = {1, 30};

    if (avcodec_open2(codecCtx, codec, nullptr) < 0) {
        std::cerr << "Failed to open codec\n";
        return false;
    }

    videoStream = avformat_new_stream(formatCtx, nullptr);
    videoStream->codecpar->codec_id = AV_CODEC_ID_H264;
    videoStream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
    videoStream->codecpar->width = width;
    videoStream->codecpar->height = height;

    if (!(formatCtx->flags & AVFMT_NOFILE)) {
        avio_open(&formatCtx->pb, filename.c_str(), AVIO_FLAG_WRITE);
    }

    if (avformat_write_header(formatCtx, nullptr) < 0) {
        std::cerr << "Failed to write header\n";
        return false;
    }
    return true;
}

bool Encoder::encodeFrame(FrameData& frame) {
    // TODO: 这里需要处理 DirectX 纹理转换为 YUV
    return true;
}

void Encoder::finalize() {
    av_write_trailer(formatCtx);
    avcodec_free_context(&codecCtx);
    avformat_free_context(formatCtx);
}