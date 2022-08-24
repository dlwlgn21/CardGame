#pragma once
#include <wrl/client.h>
#include <d2d1.h>
#include <wincodec.h>

class D2DFramework
{
	static LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
public:
	virtual HRESULT Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height);
	virtual int GameLoop();
	virtual void Render();
	virtual void Release();

	inline ID2D1HwndRenderTarget* GetRT() const { return mcpRenderTarget.Get(); };
	inline HWND GetHwnd() { return mHwnd; }

protected:
	HRESULT CreateDeviceResource();
private:
	HRESULT initD2D();
	HRESULT initWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height);

protected:
	Microsoft::WRL::ComPtr<ID2D1Factory> mcpD2DFactory{};
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> mcpRenderTarget{};
	HWND mHwnd{};
};

