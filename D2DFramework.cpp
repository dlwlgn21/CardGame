#include <cassert>
#include "D2DFramework.h"
#include "BitmapManager.h"

#pragma comment (lib, "d2d1.lib")

LRESULT D2DFramework::WinProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    D2DFramework* pFramework = reinterpret_cast<D2DFramework*>((GetWindowLongPtrW(hwnd, GWLP_USERDATA)));
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_DESTROY:
        DestroyWindow(hwnd);
        break;
    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

HRESULT D2DFramework::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
    HRESULT hr;
    hr = CoInitialize(nullptr);
    if (FAILED(hr)) { assert(false); return E_FAIL; }

    hr = initWindow(hInstance, title, width, height);
    if (FAILED(hr)) { assert(false); return E_FAIL; }

    hr = initD2D();
    if (FAILED(hr)) { assert(false); return E_FAIL; }

    hr = BitmapManager::GetInstance().Initialize(mcpRenderTarget.Get());
    if (FAILED(hr)) { assert(false); return E_FAIL; }

    return S_OK;
}

int D2DFramework::GameLoop()
{
    MSG msg;
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
            {
                break;
            }
        }
        else
        {
            Render();
        }
    }
    return static_cast<int>(msg.wParam);
}

void D2DFramework::Render()
{
    HRESULT hr;
    mcpRenderTarget->BeginDraw();
    mcpRenderTarget->Clear(D2D1::ColorF(0.f, 0.2f, 0.5f, 1.f));

    hr = mcpRenderTarget->EndDraw();
    if (hr == D2DERR_RECREATE_TARGET)
    {
        CreateDeviceResource();
    }
}

void D2DFramework::Release()
{
    BitmapManager::GetInstance().Release();
    mcpD2DFactory.Reset();
    mcpRenderTarget.Reset();
    CoUninitialize();
}

HRESULT D2DFramework::CreateDeviceResource()
{
    RECT cr;
    GetClientRect(mHwnd, &cr);

    D2D1_SIZE_U size = D2D1::SizeU(cr.right - cr.left, cr.bottom - cr.top);

    HRESULT hr = mcpD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(mHwnd, size),
        mcpRenderTarget.ReleaseAndGetAddressOf()
    );

    if (FAILED(hr)) { assert(false); return E_FAIL; }

    return S_OK;
}

HRESULT D2DFramework::initD2D()
{
    HRESULT hr;
    hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        mcpD2DFactory.GetAddressOf()
    );
    if (FAILED(hr)) { assert(false); return E_FAIL; }
    RECT cr;
    GetClientRect(mHwnd, &cr);
    hr = mcpD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(
            mHwnd,
            D2D1::SizeU(cr.right - cr.left, cr.bottom - cr.top)
        ),
        mcpRenderTarget.GetAddressOf()
    );
    if (FAILED(hr)) { assert(false); return E_FAIL; }

    return S_OK;
}

HRESULT D2DFramework::initWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
    HWND hwnd;
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);

    wc.hInstance = hInstance;
    wc.lpszClassName = title;
    wc.lpfnWndProc = D2DFramework::WinProc;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.style = CS_HREDRAW | CS_VREDRAW;

    if (RegisterClassEx(&wc) == 0){ assert(false); }


    RECT wr{0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
    AdjustWindowRectEx(&wr, WS_OVERLAPPEDWINDOW, false, 0);


    hwnd = CreateWindowEx(
        NULL,
        title,
        L"MyD2DFrameworkWindow",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wr.right - wr.left,
        wr.bottom - wr.top,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    // CreateWindowEX is failed;
    assert(hwnd != nullptr);
    
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    mHwnd = hwnd;

    return S_OK;
}
