#include <cassert>
#include "Card.h"
#include "BitmapManager.h"

const wchar_t Card::CARD_BACK_IMG_FILENAME[] = L"Data/card_back.png";
const wchar_t Card::DRAGON_IMG_FILENAME[MAX_STR_COUNT] = L"Data/card_creature_dragon.png";
const wchar_t Card::WOLF_IMG_FILENAME[MAX_STR_COUNT] = L"Data/card_creature_wolf.png";
const wchar_t Card::BEAR_IMG_FILENAME[MAX_STR_COUNT] = L"Data/card_creature_bear.png";
Card::Card(D2DFramework* pFramework, const eCardType type)
	: Actor(pFramework, CARD_BACK_IMG_FILENAME)
	, mCardType{type}
	, mBIsFront{ false }
{
	switch (mCardType)
	{
	case eCardType::DRAGON:
		mpFront = BitmapManager::GetInstance().LoadBitmapW(DRAGON_IMG_FILENAME);
		break;
	case eCardType::WOLF:
		mpFront = BitmapManager::GetInstance().LoadBitmapW(WOLF_IMG_FILENAME);
		break;
	case eCardType::BEAR:
		mpFront = BitmapManager::GetInstance().LoadBitmapW(BEAR_IMG_FILENAME);
		break;
	default:
		assert(false);
		mpFront = nullptr;
		break;
	}
}