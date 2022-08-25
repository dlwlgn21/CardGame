#include <cassert>
#include "Card.h"
#include "BitmapManager.h"

const wchar_t Card::CARD_BACK_IMG_FILENAME[] = L"Data/BackBlack.png";
const wchar_t Card::KING_IMG_FILENAME[MAX_STR_COUNT] = L"Data/KingColor.png";
const wchar_t Card::QUEEN_IMG_FILENAME[MAX_STR_COUNT] = L"Data/QueenColor.png";
const wchar_t Card::JACK_IMG_FILENAME[MAX_STR_COUNT] = L"Data/JackColor.png";
Card::Card(D2DFramework* pFramework, const eCardType type)
	: Actor(pFramework, CARD_BACK_IMG_FILENAME)
	, mCardType{type}
	, mBIsFront{ false }
{
	switch (mCardType)
	{
	case eCardType::KING:
		mpFront = BitmapManager::GetInstance().LoadBitmapW(KING_IMG_FILENAME);
		break;
	case eCardType::QUEEN:
		mpFront = BitmapManager::GetInstance().LoadBitmapW(QUEEN_IMG_FILENAME);
		break;
	case eCardType::JACK:
		mpFront = BitmapManager::GetInstance().LoadBitmapW(JACK_IMG_FILENAME);
		break;
	default:
		assert(false);
		mpFront = nullptr;
		break;
	}
}