#define _CRT_SECURE_NO_WARNINGS
#include <cassert>
#include <algorithm>
#include <random>
#include <cstring>
#include "CardGameManager.h"

const wchar_t CardGameManager::BACK_IMG_FILENAME[] = L"Data/bg_blank.png";

HRESULT CardGameManager::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
    HRESULT hr = D2DFramework::Initialize(hInstance, title, width, height);
    if (FAILED(hr)) { assert(false); }

    mspBackImg = std::make_unique<Actor>(this, BACK_IMG_FILENAME);
    initCardPos();
    return S_OK;
}

void CardGameManager::Render()
{
    HRESULT hr;
    mcpRenderTarget->BeginDraw();
    mcpRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    mcpRenderTarget->Clear(D2D1::ColorF(0.f, 0.2f, 0.5f, 1.f));
    mspBackImg->Draw();
    for (auto& e : mList)
    {
        e->Draw();
    }

    hr = mcpRenderTarget->EndDraw();
    if (hr == D2DERR_RECREATE_TARGET)
    {
        CreateDeviceResource();
    }
}

void CardGameManager::Release()
{
    mspBackImg.reset();
    mList.clear();
    D2DFramework::Release();
}

int CardGameManager::GameLoop()
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
            else if (msg.message == WM_LBUTTONUP)
            {
                float x = LOWORD(msg.lParam);
                float y = HIWORD(msg.lParam);
                OnClick(D2D1_POINT_2F{x, y});
            }
        }
        else
        {
            Render();
        }
    }
    return static_cast<int>(msg.wParam);
}

void CardGameManager::OnClick(D2D1_POINT_2F point)
{
    char buffer[MAX_STR_COUNT];

    sprintf(buffer, "%2f, %2f\n", point.x, point.y);
    OutputDebugStringA(buffer);

    Card* pCurCard = nullptr;

    for (auto& e : mList)
    {
        if (e->IsClicked(point))
        {
            pCurCard = e.get();
            break;
        }
    }

    if (pCurCard != nullptr)
    {
        if (mpPrevCard == nullptr)
        {
            mpPrevCard = pCurCard;
        }
        else
        {
            Render();
            if (pCurCard->GetCardType() == mpPrevCard->GetCardType() && pCurCard != mpPrevCard)
            {
                Sleep(STOP_MILEE_SEC);
                auto iter = mList.begin();
                while (iter != mList.end())
                {
                    if ((*iter)->IsFront())
                    {
                        iter = mList.erase(iter);
                    }
                    else
                    {
                        ++iter;
                    }
                }
                mpPrevCard = nullptr;
                if (mList.empty())
                {
                    DestroyWindow(mHwnd);
                }
            }
            else
            {
                if (pCurCard == mpPrevCard)
                {
                    mpPrevCard = nullptr;
                }
                else
                {
                    Sleep(STOP_MILEE_SEC);
                    pCurCard->Flip();
                    mpPrevCard->Flip();
                    mpPrevCard = nullptr;
                }
            }
        }

    }

}

void CardGameManager::initCardPos()
{
    std::vector<eCardType> cardTypes;
    cardTypes.reserve(MAX_CARD_COUNT);
    for (int i = 0; i < MAX_CARD_COUNT / 2; ++i)
    {
        if (i % static_cast<int>(eCardType::COUNT) == 0)
        {
            cardTypes.push_back(eCardType::DRAGON);
            cardTypes.push_back(eCardType::DRAGON);
        }
        else if (i % static_cast<int>(eCardType::COUNT) == 1)
        {
            cardTypes.push_back(eCardType::WOLF);
            cardTypes.push_back(eCardType::WOLF);
        }
        else
        {
            cardTypes.push_back(eCardType::BEAR);
            cardTypes.push_back(eCardType::BEAR);
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(cardTypes.begin(), cardTypes.end(), gen);

    int typeIdx = 0;
    for (int i = 0; i < ROW_COUNT; ++i)
    {
        for (int j = 1; j <= COL_COUNT; ++j)
        {
            mList.push_front(std::make_unique<Card>(this, cardTypes[typeIdx++]));
            mList.front()->SetPostition(CARD_X_DISTANCE * j + PADDING_X,  CARD_Y_DISTANCE * i + PADDING_Y);
        }
    }

}
