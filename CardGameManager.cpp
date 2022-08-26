#define _CRT_SECURE_NO_WARNINGS
#include <cassert>
#include <algorithm>
#include <random>
#include <cstring>
#include "CardGameManager.h"

#pragma comment (lib, "Dwrite.lib")

HRESULT CardGameManager::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
    HRESULT hr = D2DFramework::Initialize(hInstance, title, width, height);
    if (FAILED(hr)) { assert(false); }
    hr = createDeviceIndependentResources();
    if (FAILED(hr)) { assert(false); }

    mspBackImg = std::make_unique<Actor>(this, BACK_IMG_FILENAME);
    mspStartGameMenu = std::make_unique<StartGameMenu>(this);
    mspYesNoBox = std::make_unique<YesNoBox>(this);
    mBIsStartFisrt = true;
    mBIsYesNoBoxPresent = false;
    initCardPos();
    mCurGameLevel = 1;
    mLeftFilpCount = 20;
    
    return S_OK;
}

void CardGameManager::Render()
{
    HRESULT hr;
    mcpRenderTarget->BeginDraw();
    mcpRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    mcpRenderTarget->Clear(D2D1::ColorF(0.9f, 1.0f, 1.0f, 1.f));
    if (mBIsStartFisrt)
    {
        mspStartGameMenu->Draw();
        drawText(GAME_TITLE_TEXT, GAME_TITLE_X_POS, GAME_TITLE_Y_POS, GAME_TITLE_WIDTH, GAME_TITLE_HEIGHT);
        goto END_DRAW;
    }

    if (mBIsYesNoBoxPresent)
    {
        mspYesNoBox->Draw();
        if (!mList.empty())
        {
            drawText(AGAIN_TEXT, NEXT_LEVEL_X_POS + 70.f, NEXT_LEVEL_Y_POS, NEXT_LEVEL_WIDTH + 70.f, NEXT_LEVEL_HEIGHT);
            goto END_DRAW;
        }
        if (mCurGameLevel == MAX_GAME_LEVEL && mList.empty())
        {
            drawText(CLEAR_TEXT, NEXT_LEVEL_X_POS + 30.f, NEXT_LEVEL_Y_POS, NEXT_LEVEL_WIDTH + 30.f, NEXT_LEVEL_HEIGHT);
            goto END_DRAW;
        }
        if (mList.empty())
        {
            drawText(NEXT_LEVEL_TEXT, NEXT_LEVEL_X_POS, NEXT_LEVEL_Y_POS, NEXT_LEVEL_WIDTH, NEXT_LEVEL_HEIGHT);
            goto END_DRAW;
        }

    }

    mspBackImg->Draw();
    drawInGameText(GAME_LEVEL_TEXT, mCurGameLevel, LEVEL_TEXT_X_POS, LEVEL_TEXT_Y_POS, LEVEL_TEXT_WIDTH, LEVEL_TEXT_HEIGHT);
    drawInGameText(TRIAL_COUNT_TEXT, mLeftFilpCount, TRIAL_COUNT_TEXT_X_POS, TRIAL_COUNT_TEXT_Y_POS, TRIAL_COUNT_TEXT_WIDTH, TRIAL_COUNT_TEXT_HEIGHT);
    for (auto& e : mList)
    {
        e->Draw();
    }


END_DRAW:
    hr = mcpRenderTarget->EndDraw();
    if (hr == D2DERR_RECREATE_TARGET)
    {
        CreateDeviceResource();
    }
}

void CardGameManager::Release()
{
    mcpBrush.Reset();
    mcpDwriteTextFormat.Reset();
    mcpDwriteFactory.Reset();
    mspBackImg.reset();
    mspYesNoBox.reset();
    mspStartGameMenu.reset();
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
    // For Debbuing
    char buffer[MAX_STR_COUNT];
    sprintf(buffer, "%2f, %2f\n", point.x, point.y);
    OutputDebugStringA(buffer);

    if (mBIsStartFisrt)
    {
        IfStartBtnClicked(point);
        return;
    }
    if (mBIsYesNoBoxPresent)
    {
        IfYesNoBtnClicked(point);
        return;
    }

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
            if (pCurCard != mpPrevCard) { --mLeftFilpCount; }
            if (EraseIfMatched(&pCurCard, &mpPrevCard)){ }
            else { RollBack(&pCurCard, &mpPrevCard); }

            if (mList.empty() || mLeftFilpCount <= 0) { mBIsYesNoBoxPresent = true; }
        }

    }

}

HRESULT CardGameManager::createDeviceIndependentResources()
{
    HRESULT hr;
    const WCHAR MSC_FONTNAME[] = L"Consolas";
    const FLOAT MSC_FONT_SIZE = 50;

    hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(mcpDwriteFactory.Get()),
        reinterpret_cast<IUnknown**>(mcpDwriteFactory.GetAddressOf())
    );

    if (FAILED(hr)) { assert(false); return hr; }

    hr = mcpDwriteFactory->CreateTextFormat(
        MSC_FONTNAME,
        NULL,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        MSC_FONT_SIZE,
        L"en-us",
        mcpDwriteTextFormat.GetAddressOf()
    );
    if (FAILED(hr)) { assert(false); return hr; }


    mcpDwriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    if (FAILED(hr)) { assert(false); return hr; }


    mcpDwriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    if (FAILED(hr)) { assert(false); return hr; }


    mcpRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), mcpBrush.GetAddressOf());

    return S_OK;
}

void CardGameManager::initCardPos()
{
    std::vector<eCardType> cardTypes;
    cardTypes.reserve(MAX_CARD_COUNT);
    for (int i = 0; i < MAX_CARD_COUNT / 2; ++i)
    {
        if (i % static_cast<int>(eCardType::COUNT) == 0)
        {
            cardTypes.push_back(eCardType::KING);
            cardTypes.push_back(eCardType::KING);
        }
        else if (i % static_cast<int>(eCardType::COUNT) == 1)
        {
            cardTypes.push_back(eCardType::QUEEN);
            cardTypes.push_back(eCardType::QUEEN);
        }
        else
        {
            cardTypes.push_back(eCardType::JACK);
            cardTypes.push_back(eCardType::JACK);
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(cardTypes.begin(), cardTypes.end(), gen);

    int typeIdx = 0;
    for (int i = 1; i <= ROW_COUNT; ++i)
    {
        for (int j = 1; j <= COL_COUNT; ++j)
        {
            mList.push_front(std::make_unique<Card>(this, cardTypes[typeIdx++]));
            if (i == 1)
            {
                mList.front()->SetPostition(CARD_X_DISTANCE * j + PADDING_X,  CARD_Y_DISTANCE * i);
            }
            else
            {
                mList.front()->SetPostition(CARD_X_DISTANCE * j + PADDING_X, CARD_Y_DISTANCE + PADDING_Y);
            }
        }
    }

}

bool CardGameManager::EraseIfMatched(Card** ppCurCard, Card** ppPrevCard)
{
    if ((*ppCurCard)->GetCardType() == (*ppPrevCard)->GetCardType() && *ppCurCard != *ppPrevCard)
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
        *ppPrevCard = nullptr;
        return true;
    }

    return false;
}

void CardGameManager::RollBack(Card** ppCurCard, Card** ppPrevCard)
{
    if (*ppCurCard == *ppPrevCard)
    {
        *ppPrevCard = nullptr;
    }
    else
    {
        Sleep(STOP_MILEE_SEC);
        (*ppCurCard)->Flip();
        (*ppPrevCard)->Flip();
        *ppPrevCard = nullptr;
    }

}

void CardGameManager::IfStartBtnClicked(D2D1_POINT_2F point)
{
    mspStartGameMenu->OnClick(point);
    if (mspStartGameMenu->GetStartClicked())
    {
        mBIsStartFisrt = false;
        return;
    }
    if (mspStartGameMenu->GetExitClicked())
    {
        DestroyWindow(mHwnd);
        return;
    }
}

void CardGameManager::IfYesNoBtnClicked(D2D1_POINT_2F point)
{
    mspYesNoBox->OnClicked(point);
    if (mspYesNoBox->IsNoClicked())
    {
        DestroyWindow(mHwnd);
        return;
    }
    if (mspYesNoBox->IsYesClicked())
    {
        if (mList.empty())
        {
            ++mCurGameLevel;
        }
        if (mCurGameLevel == MAX_GAME_LEVEL + 1)
        {
            mCurGameLevel = 1;
        }
        mList.clear();
        initCardPos();
        mLeftFilpCount = FIRST_TRIAL_COUNT - (SUBTRACT_CPEFFICIENT * (mCurGameLevel - 1));
        mBIsYesNoBoxPresent = false;
        return;
    }
}
