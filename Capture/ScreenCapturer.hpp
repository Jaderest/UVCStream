//
// Created by Jade on 25-2-5.
//

#ifndef SCREENCAPTURER_HPP
#define SCREENCAPTURER_HPP

#include <d3d11.h>
#include <dxgi1_2.h>

struct FrameData {
    ID3D11Texture2D *texture = nullptr;
    int width{};
    int height{};
};

class ScreenCapturer {
public:
    ScreenCapturer();

    ~ScreenCapturer();

    bool initialize();

    FrameData captureFrame();

    [[nodiscard]] int getWidth() const { return screenWidth; }
    [[nodiscard]] int getHeight() const { return screenHeight; }

private:
    bool initDXGI();

    ID3D11Device *d3dDevice = nullptr; // Direct3D 11 设备
    ID3D11DeviceContext *d3dContext = nullptr; // Direct3D 11 设备上下文
    // DXGI 输出复制对象，用于捕获屏幕最新帧
    IDXGIOutputDuplication *dxgiOutputDuplication = nullptr;

    int screenWidth{};
    int screenHeight{};
};


#endif //SCREENCAPTURER_HPP
