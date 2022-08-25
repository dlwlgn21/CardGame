#pragma once
#include "Actor.h"

enum class eCardType 
{
	KING,
	QUEEN,
	JACK,
	COUNT
};

class Card final : public Actor
{
public:
	enum {MAX_STR_COUNT = 32};
	static const wchar_t CARD_BACK_IMG_FILENAME[MAX_STR_COUNT];
	const static wchar_t KING_IMG_FILENAME[MAX_STR_COUNT];
	const static wchar_t QUEEN_IMG_FILENAME[MAX_STR_COUNT];
	const static wchar_t JACK_IMG_FILENAME[MAX_STR_COUNT];

public:
	Card(D2DFramework* pFramework, const eCardType type);

	inline virtual void Draw() override 
	{
		D2D1_SIZE_U size{ mpFront->GetPixelSize() };
		D2D1_RECT_F rect{
			0, 0,
			static_cast<float>(size.width),
			static_cast<float>(size.height)

		};

		D2D1::Matrix3x2F matTranslation = D2D1::Matrix3x2F::Translation(mX, mY);
		mpFramework->GetRT()->SetTransform(matTranslation);
		if (mBIsFront)
		{
			mpFramework->GetRT()->DrawBitmap(mpFront, rect, mOpacity);
		}
		else
		{
			mpFramework->GetRT()->DrawBitmap(mpBitmap, rect, mOpacity);
		}
	}
	inline bool IsClicked(const D2D1_POINT_2F point) 
	{
		D2D1_SIZE_U size{ mpFront->GetPixelSize() };
		if (point.x >= mX && point.x <= mX + size.width &&
			point.y >= mY && point.y <= mY + size.height)
		{
			mBIsFront = !mBIsFront;
			return true;
		}
		return false;
	}
	inline bool IsFront() const { return mBIsFront; }
	inline eCardType GetCardType() const { return mCardType; }
	inline void Flip() { mBIsFront = !mBIsFront; }
private:
	bool mBIsFront;
	ID2D1Bitmap* mpFront;
	eCardType mCardType;
};

