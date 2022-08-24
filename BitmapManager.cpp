#include <cassert>
#include "BitmapManager.h"

#pragma comment(lib, "WindowsCodecs.lib")
using namespace Microsoft::WRL;
using namespace std;

HRESULT BitmapManager::Initialize(ID2D1HwndRenderTarget* pRT)
{
    assert(pRT != nullptr);
    mpRenderTarget = pRT;

    HRESULT hr = ::CoCreateInstance(
        CLSID_WICImagingFactory,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(mcpWICFactory.GetAddressOf())
    );

    if (FAILED(hr)) { assert(false); return E_FAIL; }

    return S_OK;
}

void BitmapManager::Release()
{
    for (auto& iter : mBitmapResources)
    {
        iter.second.Reset();
    }
    mcpWICFactory.Reset();
}

ID2D1Bitmap* BitmapManager::LoadBitmap(const std::wstring filename)
{
    assert(mcpWICFactory != nullptr);

    ID2D1Bitmap* pBitmap = nullptr;

    auto res = mBitmapResources.insert(pair<wstring, ComPtr<ID2D1Bitmap>>(filename, pBitmap));

    if (res.second == true)
    {
        ComPtr<ID2D1Bitmap> cpBitmap = ComPtr<ID2D1Bitmap>();
        loadWICBitmap(filename, cpBitmap.GetAddressOf());
        res.first->second = cpBitmap;
    }
    pBitmap = res.first->second.Get();

    return pBitmap;
}

HRESULT BitmapManager::loadWICBitmap(const std::wstring filename, ID2D1Bitmap** ppBitmap)
{
    ComPtr<IWICBitmapDecoder> cpDecoder;
    HRESULT hr;

    auto* pWICFactory = mcpWICFactory.Get();
    assert(pWICFactory != nullptr);

    hr = pWICFactory->CreateDecoderFromFilename(
        filename.c_str(),
        nullptr,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        cpDecoder.GetAddressOf()
    );

    if (FAILED(hr)) { assert(false); return E_FAIL; }

    ComPtr<IWICBitmapFrameDecode> cpFrame;
    hr = cpDecoder->GetFrame(0, cpFrame.GetAddressOf());

    if (FAILED(hr)) { assert(false); return E_FAIL; }

    ComPtr<IWICFormatConverter> cpConverter;
    hr = pWICFactory->CreateFormatConverter(cpConverter.GetAddressOf());

    if (FAILED(hr)) { assert(false); return E_FAIL; }

    hr = cpConverter->Initialize(
        cpFrame.Get(),
        GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone,
        nullptr,
        0,
        WICBitmapPaletteTypeCustom
    );

    if (FAILED(hr)) { assert(false); return E_FAIL; }

    hr = mpRenderTarget->CreateBitmapFromWicBitmap(
        cpConverter.Get(),
        ppBitmap
    );

    if (FAILED(hr)) { assert(false); return E_FAIL; }


    return S_OK;
}
