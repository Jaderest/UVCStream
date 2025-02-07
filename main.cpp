#include <iostream>
#include <chrono>
#include <thread>
#include <ScreenCapturer.hpp>
#include <Encoder.hpp>

int main() {
    // 1. 初始化屏幕捕获
    ScreenCapturer capturer;
    if (!capturer.initialize()) {
        std::cerr << "Failed to initialize ScreenCapturer!" << std::endl;
        return -1;
    }

    // 2. 获取屏幕尺寸
    int width = capturer.getWidth();
    int height = capturer.getHeight();
    std::cout << "Screen resolution: " << width << "x" << height << std::endl;

    // 3. 初始化编码器
    Encoder encoder;
    if (!encoder.initialize("output.mp4", width, height)) {
        std::cerr << "Failed to initialize Encoder!" << std::endl;
        return -1;
    }

    // 4. 进入帧捕获 & 编码循环
    const int FPS = 30;
    const auto frameDuration = std::chrono::milliseconds(1000 / FPS);

    std::cout << "Starting screen capture and encoding..." << std::endl;

    for (int i = 0; i < 300; ++i) { // 录制 10 秒（30 FPS * 10s）
        auto startTime = std::chrono::steady_clock::now();

        // 捕获屏幕帧
        FrameData frame = capturer.captureFrame();
        if (!frame.texture) {
            std::cerr << "Failed to capture frame!" << std::endl;
            break;
        }

        // 传递帧给编码器
        if (!encoder.encodeFrame(frame)) {
            std::cerr << "Failed to encode frame!" << std::endl;
            break;
        } else {
            std::cout << "Frame " << i << " encoded" << std::endl;
        }

        // 控制帧率
        auto elapsed = std::chrono::steady_clock::now() - startTime;
        std::this_thread::sleep_for(frameDuration - elapsed);
    }

    // 5. 释放资源
    encoder.finalize();
    std::cout << "Encoding complete! Output saved as output.mp4" << std::endl;

    return 0;
}
