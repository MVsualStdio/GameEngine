#include "TextureManager.h"
#include <locale>
#include <codecvt>
#include <wincodec.h>

class TextureLoader {
public:
    static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> LoadTextureFromPNG(
        ID3D11Device* device,
        const std::wstring& filename) {

        Microsoft::WRL::ComPtr<IWICImagingFactory> wicFactory;
        Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
        Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
        Microsoft::WRL::ComPtr<IWICFormatConverter> converter;

        // 初始化COM（如果尚未初始化）
        static bool comInitialized = false;
        if (!comInitialized) {
            CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
            comInitialized = true;
        }

        // 创建WIC工厂
        HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&wicFactory));
        if (FAILED(hr)) return nullptr;

        // 加载图像
        hr = wicFactory->CreateDecoderFromFilename(filename.c_str(), nullptr, GENERIC_READ,
            WICDecodeMetadataCacheOnLoad, &decoder);
        if (FAILED(hr)) return nullptr;

        hr = decoder->GetFrame(0, &frame);
        if (FAILED(hr)) return nullptr;

        UINT width, height;
        hr = frame->GetSize(&width, &height);
        if (FAILED(hr)) return nullptr;

        hr = wicFactory->CreateFormatConverter(&converter);
        if (FAILED(hr)) return nullptr;

        hr = converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppRGBA,
            WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom);
        if (FAILED(hr)) return nullptr;

        // 读取图像数据
        UINT stride = width * 4;
        UINT imageSize = stride * height;
        std::vector<uint8_t> imageData(imageSize);

        hr = converter->CopyPixels(nullptr, stride, imageSize, imageData.data());
        if (FAILED(hr)) return nullptr;

        // 创建D3D11纹理
        D3D11_TEXTURE2D_DESC texDesc = {};
        texDesc.Width = width;
        texDesc.Height = height;
        texDesc.MipLevels = 1;
        texDesc.ArraySize = 1;
        texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        texDesc.SampleDesc.Count = 1;
        texDesc.SampleDesc.Quality = 0;
        texDesc.Usage = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        texDesc.CPUAccessFlags = 0;
        texDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = imageData.data();
        initData.SysMemPitch = stride;
        initData.SysMemSlicePitch = imageSize;

        Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
        hr = device->CreateTexture2D(&texDesc, &initData, &texture);
        if (FAILED(hr)) return nullptr;

        // 创建SRV
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = texDesc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = 1;

        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
        hr = device->CreateShaderResourceView(texture.Get(), &srvDesc, &srv);
        if (FAILED(hr)) return nullptr;

        return srv;
    }
};


TextureManager* TextureManager::instance() {
	static TextureManager gManager;
	return &gManager;
}

void TextureManager::addTexture(std::string path) {
	if (gTextures.find(path) == gTextures.end()) {
		gLazyCache.emplace(path);
	}
}

void TextureManager::loading(D3D11Context* context) {
	for (auto cache : gLazyCache) {
        ComPtr<ID3D11ShaderResourceView> textureView;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wideString = converter.from_bytes(cache);

        if (isDDS(getFileExtension(cache))) {
            DirectX::CreateDDSTextureFromFile(context->m_Device.Get(), wideString.c_str(), nullptr, &textureView);
        }
        else if (isPNG(getFileExtension(cache))) {
            textureView = TextureLoader::LoadTextureFromPNG(context->m_Device.Get(), wideString);
        }

		gTextures[cache] = std::make_shared<Texture2D>(context, textureView);
	}
	gLazyCache.clear();
}

Texture2D* TextureManager::getTexture(std::string path, D3D11Context* context) {
	loading(context);
	if (gTextures.find(path) != gTextures.end()) {
		return gTextures[path].get();
	}
	return nullptr;
}

std::string TextureManager::getFileExtension(const std::string& path) {
    size_t dotPos = path.find_last_of(".");
    if (dotPos != std::string::npos && dotPos < path.length() - 1) {
        return path.substr(dotPos + 1);
    }
    return "";
}

bool TextureManager::isDDS(std::string& suffix) {
    return suffix == "DDS" || suffix == "dds";
}

bool TextureManager::isPNG(std::string& suffix) {
    return suffix == "PNG" || suffix == "png";
}