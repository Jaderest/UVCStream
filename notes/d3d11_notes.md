# Direct3D 11 Graphics Notes

## 目录
- [Direct3D 11 Graphics Notes](#direct3d-11-graphics-notes)
  - [目录](#目录)
  - [D3D11 Textures](#d3d11-textures)
    - [纹理描述: `D3D11_TEXTURE2D_DESC`](#纹理描述-d3d11_texture2d_desc)


## D3D11 Textures

- 创建一个纹理：描述其属性&调用纹理创建API

### 纹理描述: `D3D11_TEXTURE2D_DESC`
- 这是一个数据结构，用于描述纹理的属性，包括宽度、高度、MipMap级别、格式等。
- 以下是一个示例代码，用于创建一个2D纹理的描述：
```cpp
D3D11_TEXTURE2D_DESC desc;
desc.Width = 256;                       // 纹理宽度
desc.Height = 256;                      // 纹理高度
desc.MipLevels = desc.ArraySize = 1;    // 仅包含一级 Mipmap，纹理数组大小为 1
desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32 位颜色格式 (RGBA 8-bit per channel)
desc.SampleDesc.Count = 1;              // 关闭 MSAA 采样
desc.Usage = D3D11_USAGE_DYNAMIC;       // 允许 CPU 写入数据
desc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // 绑定为着色器资源
desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // 允许 CPU 写入
desc.MiscFlags = 0;                      // 无特殊标志
```

### 创建纹理
- ID3D11Texture2D 是一个接口，用于表示一个 2D 纹理对象。
- 首先需要初始化一个 ID3D11Device 对象，然后调用其 `CreateTexture2D` 方法创建一个纹理对象。