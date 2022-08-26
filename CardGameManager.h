#pragma once
#pragma warning(disable : 4996)
#include <memory>
#include <string>
#include <dwrite.h>
#include <stdio.h>
#include "Card.h"
#include "StartGameMenu.h"
#include "YesNoBox.h"

class CardGameManager final : public D2DFramework
{
public:
	enum { MAX_STR_COUNT = 32 };
	enum { MAX_NUM_COUNT = 4};
	const wchar_t BACK_IMG_FILENAME[MAX_STR_COUNT] = L"Data/InGame.jpg";


	// Game Logic Info
	const int STOP_MILEE_SEC = 500;
	const int MAX_GAME_LEVEL = 3;
	const int FIRST_TRIAL_COUNT = 20;
	const int SUBTRACT_CPEFFICIENT = 4;

	// IN Game Text SECTION
	const wchar_t GAME_LEVEL_TEXT[MAX_STR_COUNT] = L"Game level : ";
	const float LEVEL_TEXT_X_POS = 330.f;
	const float LEVEL_TEXT_Y_POS = 50.f;
	const float LEVEL_TEXT_WIDTH = 400.f;
	const float LEVEL_TEXT_HEIGHT = 100.f;
	const float TEXT_NUM_DIFF = 30.f;

	const wchar_t TRIAL_COUNT_TEXT[MAX_STR_COUNT] = L"Left Filp Count : ";
	const float TRIAL_COUNT_TEXT_X_POS = 250.f;
	const float TRIAL_COUNT_TEXT_Y_POS = 150.f;
	const float TRIAL_COUNT_TEXT_WIDTH = 500.f;
	const float TRIAL_COUNT_TEXT_HEIGHT = 100.f;

	// Game Title DWrite SECTION
	const wchar_t GAME_TITLE_TEXT[MAX_STR_COUNT] = L"Matching Game";
	const float GAME_TITLE_X_POS = 320.f;
	const float GAME_TITLE_Y_POS = 400.f;
	const float GAME_TITLE_WIDTH = 500.f;
	const float GAME_TITLE_HEIGHT = 100.f;

	// YesNoBox SECTION
	const wchar_t NEXT_LEVEL_TEXT[MAX_STR_COUNT] = L"Go to next level?";
	const wchar_t AGAIN_TEXT[MAX_STR_COUNT] = L"Try again?";
	const wchar_t CLEAR_TEXT[MAX_STR_COUNT * 2] = L"Congratuation!\nPlay Again?";

	const float NEXT_LEVEL_X_POS = 300.f;
	const float NEXT_LEVEL_Y_POS = 250.f;
	const float NEXT_LEVEL_WIDTH = 600.f;
	const float NEXT_LEVEL_HEIGHT = 100.f;

	// Card SECTION
	const int MAX_CARD_COUNT = 10;
	const int COL_COUNT = 5;
	const int ROW_COUNT = 2;
	const float CARD_X_DISTANCE = 135.f;
	const float CARD_Y_DISTANCE = 300.f;
	const int PADDING_X = 50;
	const int PADDING_Y = 200;

public:
	virtual HRESULT Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height) override;
	virtual void Render() override;
	virtual void Release() override;
	virtual int GameLoop() override;

	void OnClick(D2D1_POINT_2F point);

private:
	HRESULT createDeviceIndependentResources();
	void initCardPos();
	bool EraseIfMatched(Card** ppCurCard, Card** ppPrevCard);
	void RollBack(Card** ppCurCard, Card** ppPrevCard);
	void IfStartBtnClicked(D2D1_POINT_2F point);
	void IfYesNoBtnClicked(D2D1_POINT_2F point);

private:
	inline void drawText(const wchar_t* text, const float x, const float y, const float width, const float height)
	{
		mcpRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		mcpRenderTarget->DrawTextW(
			text,
			static_cast<UINT32>(wcslen(text)),
			mcpDwriteTextFormat.Get(),
			D2D1::RectF(x, y, x + width, y + height),
			mcpBrush.Get()
		);
	}

	inline void drawInGmaeText(const wchar_t* text, int num, const float x, const float y, const float width, const float height)
	{
		assert(num < 100);
		wchar_t buf[MAX_NUM_COUNT];
		swprintf_s(buf, L"%d", num);
		mcpRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		mcpRenderTarget->DrawTextW(
			text,
			static_cast<UINT32>(wcslen(text)),
			mcpDwriteTextFormat.Get(),
			D2D1::RectF(x, y, x + width, y + height),
			mcpBrush.Get()
		);
		if (num >= 10)
		{
			mcpBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
		}
		else if (num > 5)
		{
			mcpBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow));
		}
		else
		{
			mcpBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
		}
		mcpRenderTarget->DrawTextW(
			buf,
			static_cast<UINT32>(wcslen(buf)),
			mcpDwriteTextFormat.Get(),
			D2D1::RectF(x + width + TEXT_NUM_DIFF, y, x + width + 100.f, y + height),
			mcpBrush.Get()
		);
		mcpBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));

	}
private:
	Microsoft::WRL::ComPtr<IDWriteFactory> mcpDwriteFactory;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> mcpDwriteTextFormat;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> mcpBrush;

	std::unique_ptr<Actor> mspBackImg;
	std::unique_ptr<StartGameMenu> mspStartGameMenu;
	std::unique_ptr<YesNoBox> mspYesNoBox;
	std::list<std::unique_ptr<Card>> mList;

	Card* mpPrevCard{ nullptr };
	bool mBIsStartFisrt;
	bool mBIsYesNoBoxPresent;

private:
	int mCurGameLevel;
	int mLeftFilpCount;

};

