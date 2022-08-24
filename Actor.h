#pragma once

#include <string>
#include "D2DFramework.h"
#include "BitmapManager.h"

class Actor
{
public:
	Actor(D2DFramework* pFramework, const std::wstring filename);
	
	void Draw();

	inline D2D1_POINT_2F GetPosition() const { return D2D1_POINT_2F{ mX, mY }; }
	inline void SetPostition(D2D1_POINT_2F pos) 
	{
		mX = pos.x;
		mY = pos.y;
	}
	inline void SetPostition(float x, float y)
	{
		mX = x;
		mY = y;
	}
protected:
	inline void draw(float x, float y, float opacity) 
	{
		D2D1_SIZE_U size = mpBitmap->GetPixelSize();
		D2D1_RECT_F rect{
			0.f, 0.f,
			static_cast<float>(0 + size.width),
			static_cast<float>(0 + size.height)
		};
		D2D1::Matrix3x2F matTranslation = D2D1::Matrix3x2F::Translation(mX, mY);
		mpFramework->GetRT()->SetTransform(matTranslation);
		mpFramework->GetRT()->DrawBitmap(
			mpBitmap,
			rect,
			opacity
		);
	}

protected:
	D2DFramework* mpFramework;
	ID2D1Bitmap* mpBitmap;
	float mX;
	float mY;
	float mOpacity;
	
};

