#include "Actor.h"

Actor::Actor(D2DFramework* pFramework, const std::wstring filename)
	: mpFramework{ pFramework }
	, mX{0.f} , mY{0.f} , mOpacity{ 1.f }
{
	mpBitmap = BitmapManager::GetInstance().LoadBitmapW(filename);
}

void Actor::Draw()
{
	draw(mX, mY, mOpacity);
}

