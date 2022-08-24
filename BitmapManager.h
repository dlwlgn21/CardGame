#pragma once

#include <wrl/client.h>
#include <d2d1.h>
#include <wincodec.h>
#include <unordered_map>
#include <string>

class BitmapManager final
{
public:
	BitmapManager(const BitmapManager& other) = delete;
	BitmapManager& operator=(const BitmapManager& other) = delete;

	static BitmapManager& GetInstance()
	{
		static BitmapManager instance;
		return instance;
	}

	HRESULT Initialize(ID2D1HwndRenderTarget* pRT);
	void Release();

	ID2D1Bitmap* LoadBitmap(const std::wstring filename);

private:
	BitmapManager() = default;
	~BitmapManager() = default;

	HRESULT loadWICBitmap(const std::wstring filename, ID2D1Bitmap** ppBitmap);

private:
	ID2D1HwndRenderTarget* mpRenderTarget{};
	Microsoft::WRL::ComPtr<IWICImagingFactory> mcpWICFactory{};
	std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID2D1Bitmap>> mBitmapResources{};


};

