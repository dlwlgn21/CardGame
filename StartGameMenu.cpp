#include <cassert>
#include "StartGameMenu.h"
#include "BitmapManager.h"
const wchar_t StartGameMenu::GAME_IMG[] = L"Data/GameImgColor.png";
const wchar_t StartGameMenu::START_BACK_IMG[] = L"Data/StartBack.jpg";
const wchar_t StartGameMenu::START_BUTTON_IMG[] = L"Data/StartButton.png";
const wchar_t StartGameMenu::EXIT_BUTTON_IMG[] = L"Data/ExitButton.png";

StartGameMenu::StartGameMenu(D2DFramework* pFramework)
	: Actor(pFramework, START_BACK_IMG)
	, mBIsStartClicked(false)
	, mBIsExitClicked(false)
{
	mpGameImg = BitmapManager::GetInstance().LoadBitmapW(GAME_IMG);
	mpStartButtonImg = BitmapManager::GetInstance().LoadBitmapW(START_BUTTON_IMG);
	mpExitButtonImg = BitmapManager::GetInstance().LoadBitmapW(EXIT_BUTTON_IMG);
}

