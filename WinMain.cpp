#include <cassert>
#include <Windows.h>
#include "CardGameManager.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstacne, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	CardGameManager myFramework;
	HRESULT hr;
	int ret;

	hr = myFramework.Initialize(hInstance, L"MYD2D", 1024, 768);
	if (FAILED(hr)) { assert(false); }

	ret = myFramework.GameLoop();
	myFramework.Release();

	return ret;
}