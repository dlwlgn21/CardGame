#pragma once

#include <memory>
#include <string>
#include "Card.h"

class CardGameManager final : public D2DFramework
{
public:
	enum { MAX_STR_COUNT = 64 };
	static const wchar_t BACK_IMG_FILENAME[MAX_STR_COUNT];
	static const wchar_t CARD_BACK_IMG_FILENAME[MAX_STR_COUNT];


	const int STOP_MILEE_SEC = 500;

	// CARD SECTION
	const int MAX_CARD_COUNT = 20;
	const int COL_COUNT = 5;
	const int ROW_COUNT = 4;
	const float CARD_X_DISTANCE = 140.f;
	const float CARD_Y_DISTANCE = 150.f;
	const int PADDING_X = 50;
	const int PADDING_Y = 50;

public:
	virtual HRESULT Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height) override;
	virtual void Render() override;
	virtual void Release() override;
	virtual int GameLoop() override;

	void OnClick(D2D1_POINT_2F point);

private:
	void initCardPos();
	bool EraseIfMatched(Card** ppCurCard, Card** ppPrevCard);
	void RollBack(Card** ppCurCard, Card** ppPrevCard);
private:
	std::unique_ptr<Actor> mspBackImg;
	std::list<std::unique_ptr<Card>> mList;
	Card* mpPrevCard{ nullptr };
};

