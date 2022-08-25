#pragma once
#include "Actor.h"
class YesNoBox final : public Actor
{
	enum {MAX_STR_COUNT = 32};
	static const wchar_t TEXT_BOX_IMG[MAX_STR_COUNT];
	static const wchar_t YES_IMG[MAX_STR_COUNT];
	static const wchar_t NO_IMG[MAX_STR_COUNT];
	

	const float T_BOX_X_POS = 240.f;
	const float T_BOX_Y_POS = 170.f;
	const float YES_BOX_X_POS = 360.f;
	const float NO_BOX_X_POS = 560.f;
	const float YES_NO_BOX_Y_POS = 370.f;
	
public:
	YesNoBox(D2DFramework* pFramework);

	inline virtual void Draw() override
	{
		D2D1_SIZE_U size{mpBitmap->GetPixelSize()};
		D2D1_RECT_F rect{
			0, 0,
			static_cast<float>(size.width),
			static_cast<float>(size.height)
		};
		D2D1::Matrix3x2F matTranslation = D2D1::Matrix3x2F::Translation(T_BOX_X_POS, T_BOX_Y_POS);
		mpFramework->GetRT()->SetTransform(matTranslation);
		mpFramework->GetRT()->DrawBitmap(mpBitmap, rect, mOpacity);

		size = mpYesBtn->GetPixelSize();
		rect = {0, 0, static_cast<float>(size.width), static_cast<float>(size.height) };
		matTranslation = D2D1::Matrix3x2F::Translation(YES_BOX_X_POS, YES_NO_BOX_Y_POS);
		mpFramework->GetRT()->SetTransform(matTranslation);
		mpFramework->GetRT()->DrawBitmap(mpYesBtn, rect, mOpacity);

		matTranslation = D2D1::Matrix3x2F::Translation(NO_BOX_X_POS, YES_NO_BOX_Y_POS);
		mpFramework->GetRT()->SetTransform(matTranslation);
		mpFramework->GetRT()->DrawBitmap(mpNoBtn, rect, mOpacity);

	}

	inline bool IsYesClicked() const { return mBIsYesClicked; }
	inline bool IsNoClicked() const { return mBIsNoClicked; }

	inline void OnClicked(D2D1_POINT_2F point)
	{
		D2D1_SIZE_U size{ mpYesBtn->GetPixelSize() };

		if (point.x >= YES_BOX_X_POS && point.x <= YES_BOX_X_POS + size.width &&
			point.y >= YES_NO_BOX_Y_POS && point.y <= YES_NO_BOX_Y_POS + size.height)
		{
			mBIsYesClicked = true;
			return;
		}
		
		if (point.x >= NO_BOX_X_POS && point.x <= NO_BOX_X_POS + size.width &&
			point.y >= YES_NO_BOX_Y_POS && point.y <= YES_NO_BOX_Y_POS + size.height)
		{
			mBIsNoClicked = true;
			return;
		}

	}

private:
	ID2D1Bitmap* mpYesBtn;
	ID2D1Bitmap* mpNoBtn;
	bool mBIsYesClicked;
	bool mBIsNoClicked;
};

