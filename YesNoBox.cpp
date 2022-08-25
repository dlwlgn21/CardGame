#include "YesNoBox.h"
#include "BitmapManager.h"
const wchar_t YesNoBox::TEXT_BOX_IMG[MAX_STR_COUNT] = L"Data/TextBox.png";
const wchar_t YesNoBox::YES_IMG[MAX_STR_COUNT] = L"Data/YesBtn.png";
const wchar_t YesNoBox::NO_IMG[MAX_STR_COUNT] = L"Data/NoBtn.png";

YesNoBox::YesNoBox(D2DFramework* pFramework)
	: Actor(pFramework, TEXT_BOX_IMG)
	, mBIsYesClicked{ false }
	, mBIsNoClicked{ false }
{
	mpYesBtn = BitmapManager::GetInstance().LoadBitmapW(YES_IMG);
	mpNoBtn = BitmapManager::GetInstance().LoadBitmapW(NO_IMG);
	mX = 200.f;
	mY = 100.f;
}
