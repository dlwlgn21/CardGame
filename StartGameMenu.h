#pragma once
#include <cassert>
#include "Actor.h"

class StartGameMenu final : public Actor
{
public:
	enum { MAX_STR_COUNT = 32 };
	const static wchar_t GAME_IMG[MAX_STR_COUNT];
	const static wchar_t START_BACK_IMG[MAX_STR_COUNT];
	const static wchar_t START_BUTTON_IMG[MAX_STR_COUNT];
	const static wchar_t EXIT_BUTTON_IMG[MAX_STR_COUNT];

	const float GAME_IMG_X_POS = 390.f;
	const float GAME_IMG_Y_POS = 100.f;
	const float START_BTN_X_POS = 240.f;
	const float EXIT_BTN_X_POS = 520.f;
	const float START_END_BTN_Y_POS = 450.f;

public:
	StartGameMenu(D2DFramework* pFramework);
	inline virtual void Draw() override
	{
		D2D1_SIZE_U size{ mpBitmap->GetPixelSize() };
		D2D1_RECT_F rect{
			0, 0,
			static_cast<float>(size.width),
			static_cast<float>(size.height)
		};
		mpFramework->GetRT()->DrawBitmap(mpBitmap, rect, mOpacity);

		size = mpGameImg->GetPixelSize();
		rect = { 0, 0, static_cast<float>(size.width), static_cast<float>(size.height) };
		D2D1::Matrix3x2F matTranslation = D2D1::Matrix3x2F::Translation(GAME_IMG_X_POS, GAME_IMG_Y_POS);
		mpFramework->GetRT()->SetTransform(matTranslation);
		mpFramework->GetRT()->DrawBitmap(mpGameImg, rect, mOpacity);

		size = mpStartButtonImg->GetPixelSize();
		rect = { 0, 0, static_cast<float>(size.width), static_cast<float>(size.height) };
		matTranslation = D2D1::Matrix3x2F::Translation(START_BTN_X_POS, START_END_BTN_Y_POS);
		mpFramework->GetRT()->SetTransform(matTranslation);
		mpFramework->GetRT()->DrawBitmap(mpStartButtonImg, rect, mOpacity);

		size = mpExitButtonImg->GetPixelSize();
		rect = { 0, 0, static_cast<float>(size.width), static_cast<float>(size.height) };
		matTranslation = D2D1::Matrix3x2F::Translation(EXIT_BTN_X_POS, START_END_BTN_Y_POS);
		mpFramework->GetRT()->SetTransform(matTranslation);
		mpFramework->GetRT()->DrawBitmap(mpExitButtonImg, rect, mOpacity);
	}

	inline bool GetStartClicked() const { return mBIsStartClicked; }
	inline bool GetExitClicked() const { return mBIsExitClicked; }

	inline void OnClick(D2D1_POINT_2F point)
	{
		D2D1_SIZE_U size{mpStartButtonImg->GetPixelSize()};
		if (point.x >= START_BTN_X_POS && point.x <= START_BTN_X_POS + size.width &&
			point.y >= START_END_BTN_Y_POS && point.y <= START_END_BTN_Y_POS + size.height)
		{
			mBIsStartClicked = true;
			return;
		}
		
		if (point.x >= EXIT_BTN_X_POS && point.x <= EXIT_BTN_X_POS + size.width &&
			point.y >= START_END_BTN_Y_POS && point.y <= START_END_BTN_Y_POS + size.height)
		{
			mBIsExitClicked = true;
			return;
		}
	}
private:
	ID2D1Bitmap* mpGameImg;
	ID2D1Bitmap* mpStartButtonImg;
	ID2D1Bitmap* mpExitButtonImg;
	bool mBIsStartClicked;
	bool mBIsExitClicked;
	
};

