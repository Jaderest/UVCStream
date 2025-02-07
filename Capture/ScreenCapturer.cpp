//
// Created by Jade on 25-2-5.
//

#include "ScreenCapturer.hpp"
#include <iostream>

ScreenCapturer::ScreenCapturer() = default;

ScreenCapturer::~ScreenCapturer() {
    if (dxgiOutputDuplication) dxgiOutputDuplication->Release();
    if (d3dContext) d3dContext->Release();
    if (d3dDevice) d3dDevice->Release();
}

bool ScreenCapturer::initialize() {
    // 初始化 DirectX 设备和 DXGI 输出复制
    return initDXGI();
}

bool ScreenCapturer::initDXGI() {
    HRESULT hr;

    // 创建 D3D11 设备
    D3D_FEATURE_LEVEL featureLevel;
    // 创建Direct3D 11设备和设备上下文，用于GPU渲染
    // &d3dDevice: 返回创建的Direct3D 11设备
    // &featureLevel: 返回创建的设备的特性级别
    // &d3dContext: 返回创建的设备上下文
    hr = D3D11CreateDevice(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
        D3D11_SDK_VERSION, &d3dDevice, &featureLevel, &d3dContext);
    if (FAILED(hr)) {
        std::cerr << "Failed to create D3D11 device\n";
        return false;
    }

    // 获取 DXGI 设备
    // DXGI是D3D11和显示器交互的关键组件
    IDXGIDevice* dxgiDevice = nullptr;
    hr = d3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void **>(&dxgiDevice));
    if (FAILED(hr)) {
        std::cerr << "Failed to get DXGI device\n";
        return false;
    }

    // 获取 DXGI 适配器
    IDXGIAdapter* dxgiAdapter = nullptr;
    hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void **>(&dxgiAdapter));
    dxgiDevice->Release();
    if (FAILED(hr)) {
        std::cerr << "Failed to get DXGI adapter\n";
        return false;
    }

    // 获取 DXGI 输出（显示器）
    IDXGIOutput* dxgiOutput = nullptr;
    // 获取索引为0的显示器的输出
    hr = dxgiAdapter->EnumOutputs(0, &dxgiOutput);
    dxgiAdapter->Release();
    if (FAILED(hr)) {
        std::cerr << "Failed to get DXGI output\n";
        return false;
    }

    // 获取 DXGI 输出信息
    DXGI_OUTPUT_DESC outputDesc;
    dxgiOutput->GetDesc(&outputDesc);
    screenWidth = outputDesc.DesktopCoordinates.right - outputDesc.DesktopCoordinates.left;
    screenHeight = outputDesc.DesktopCoordinates.bottom - outputDesc.DesktopCoordinates.top;

    // 复制 DXGI 输出
    IDXGIOutput1* dxgiOutput1 = nullptr;
    hr = dxgiOutput->QueryInterface(__uuidof(IDXGIOutput1), reinterpret_cast<void **>(&dxgiOutput1));
    dxgiOutput->Release();
    if (FAILED(hr)) {
        std::cerr << "Failed to get DXGI Output1\n";
        return false;
    }

    hr = dxgiOutput1->DuplicateOutput(d3dDevice, &dxgiOutputDuplication);
    dxgiOutput1->Release();
    if (FAILED(hr)) {
        std::cerr << "Failed to duplicate output\n";
        return false;
    }

    return true;
}

FrameData ScreenCapturer::captureFrame() {
    FrameData frame;
    frame.width = screenWidth;
    frame.height = screenHeight;

    DXGI_OUTDUPL_FRAME_INFO frameInfo;
    ID3D11Texture2D* texture = nullptr;
    IDXGIResource* desktopResource = nullptr;

    HRESULT hr = dxgiOutputDuplication->AcquireNextFrame(100, &frameInfo, &desktopResource);
    if (FAILED(hr)) {
        return frame;
    }

    hr = desktopResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&texture));
    desktopResource->Release();
    if (FAILED(hr)) {
        dxgiOutputDuplication->ReleaseFrame();
        std::cout << "faq" << std::endl;
        return frame;
    }

    frame.texture = texture;
    dxgiOutputDuplication->ReleaseFrame();

    return frame;
}