#include <windows.h>
#include "Common.h"
#include "Animation.h"

LRESULT CALLBACK WndProc_AnimationCards(HWND hwnd, UINT message, WPARAM wParam,
										LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc, hdcmMem;
	//HBITMAP hBitmap;
	int id;

	switch(message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		id = GetDlgCtrlID(hwnd);
		if (animationCards[id - ANICARD_P1] != NO_CARD)
		{
			hdcmMem = CreateCompatibleDC(hdc);
	//		hBitmap = LoadBitmap((HINSTANCE)GetWindowLong(GetParent(hwnd), GWL_HINSTANCE),
	//			MAKEINTRESOURCE(GETCARDRES((int) animationCards[id - ANICARD_P1])));
			SelectObject(hdcmMem, *phBitmap[(int) animationCards[id - ANICARD_P1]]);

			//BitBlt(hdc, 0,0, CARD_X, CARD_Y, hdcmMem, 0,0, SRCCOPY);
			StretchBlt(hdc, 0,0, CARD_X, CARD_Y, hdcmMem, 0,0, 71,96, SRCCOPY);
	//		DeleteObject(hBitmap);
			DeleteDC(hdcmMem);
		}
		else
			ShowWindow(hwnd, SW_HIDE);

		EndPaint(hwnd, &ps);
		return 0;

	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

// end animation collect cards
void ShowEndAnimation(HWND hwnd)
{
	int i, j;

	RECT rect;
	int x, y;
	POINT pWndPos[4];
	const BYTE winner = GetCurrentWinner();

	for (i = 0; i < 4; i++)
	{
		animationCards[i] = cardTable[i];
	}

	EmptyTable();
	Update_CardTable(hwnd);


	GetClientRect(hwnd, &rect);
	x = rect.right - rect.left;
	y = rect.bottom - rect.top;

	pWndPos[PLAYER_1].x = CT_P1_X(y);
	pWndPos[PLAYER_1].y = CT_P1_Y(y);

	pWndPos[PLAYER_2].x = CT_P2_X(y);
	pWndPos[PLAYER_2].y = CT_P2_Y(y);

	pWndPos[PLAYER_3].x = CT_P3_X(y);
	pWndPos[PLAYER_3].y = CT_P3_Y(y);

	pWndPos[PLAYER_4].x = CT_P4_X(y);
	pWndPos[PLAYER_4].y = CT_P4_Y(y);

	// show
	for (i = 0; i < 4; i++)
	{
		ShowWindow(GetDlgItem(hwnd, ANICARD_P1 + i), SW_SHOW);
	}

	// moving
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 4; j++)
		{
			CalWndpos(pWndPos, winner);
			SetWindowPos(GetDlgItem(hwnd, ANICARD_P1 + j),
				HWND_BOTTOM,
				pWndPos[j].x , pWndPos[j].y,
				0,0,
				SWP_NOSIZE); 
			UpdateWindow(GetDlgItem(hwnd, ANICARD_P1 + j));
		}
		Sleep(10);
	}


	// Hide Anicards
	for (i = 0; i < 4; i++)
	{
		animationCards[i] = NO_CARD;
		ShowWindow(GetDlgItem(hwnd, ANICARD_P1 + i), SW_HIDE);
		InvalidateRect(GetDlgItem(hwnd, i + ANICARD_P1), NULL, false);
	}

	return;
}

// calculate Pos
void CalWndpos(POINT *pWndPos, const BYTE &winner)
{
	int x, y;
	const int addFactor = 5;
	int i;

	switch (winner)
	{
	case PLAYER_1:
		x = 0;
		y = addFactor;
		break;

	case PLAYER_2:
		x = addFactor;
		y = 0;
		break;

	case PLAYER_3:
		x = 0;
		y = -addFactor;
		break;

	case PLAYER_4:
		x = -addFactor;
		y = 0;
		break;
	}
	for (i = 0; i < 4; i++)
	{
		pWndPos[i].x += x;
		pWndPos[i].y += y;
	}
	return;
}


void ShowPlayAnimation(HWND hWndParent, BYTE card, BYTE Player)
{
	int i,j;
	RECT rect;
	POINT pWndPos;
	POINT pStartPos;
	int x,y;
	int addFactorX, addFactorY;
	const int aniSteps = 20;
	int cPos; // card position in user

	// set count card was coded here cuz, everytime a card
	// draws this function run only once
	SetCountCards(Player, card);

	GetClientRect(hWndParent, &rect);
	x = rect.right - rect.left;
	y = rect.bottom - rect.top;

	for (i = 0; i < 32; i++)
	{
		if (cardDeck[i] == card)
			cPos = (i % 8);
	}

	switch (Player)
	{
	case PLAYER_1:
		pWndPos.x = CT_P1_X(y);
		pWndPos.y = CT_P1_Y(y);

		pStartPos.x = PLAYA1_CARD_X(y) + (cPos * (CARD_X / 5));
		pStartPos.y = PLAYA1_CARD_Y(y);
		break;

	case PLAYER_2:
		pWndPos.x = CT_P2_X(y);
		pWndPos.y = CT_P2_Y(y);

		pStartPos.x = PLAYA2_CARD_X(y);
		pStartPos.y = PLAYA2_CARD_Y(y) - (cPos * (CARD_X / 5));
		break;

	case PLAYER_3:
		pWndPos.x = CT_P3_X(y);
		pWndPos.y = CT_P3_Y(y);

		pStartPos.x = PLAYA3_CARD_X(y) - (cPos * (CARD_X / 5));
		pStartPos.y = PLAYA3_CARD_Y;
		break;

	case PLAYER_4:
		pWndPos.x = CT_P4_X(y);
		pWndPos.y = CT_P4_Y(y);

		pStartPos.x = PLAYA4_CARD_X;
		pStartPos.y = PLAYA4_CARD_Y(y) + (cPos * (CARD_X / 5));
		break;
	}

	addFactorX = (pWndPos.x - pStartPos.x) / aniSteps;
	addFactorY = (pWndPos.y - pStartPos.y) / aniSteps;

	// only animates card1
	animationCards[Player] = card;

	SetWindowPos(GetDlgItem(hWndParent, ANICARD_P1 + Player),
		0,
		pStartPos.x,
		pStartPos.y,
		0,0,
		SWP_NOZORDER | SWP_NOSIZE);

	ShowWindow(GetDlgItem(hWndParent, ANICARD_P1 + Player), SW_SHOW);

	if (bSndOn)
	{
		if ((trumps == GetCardType(card)) && (IsThisCardWinnable(Player, card)))
			PlaySound((LPCTSTR)hCutSnd, NULL, SND_MEMORY | SND_ASYNC);
		else
			PlaySound((LPCTSTR)hDealSound, NULL, SND_MEMORY | SND_ASYNC);
	}
	// animate
	for (i = 0; i < aniSteps; i++)
	{
		pStartPos.x += addFactorX;
		pStartPos.y += addFactorY;
		SetWindowPos(GetDlgItem(hWndParent, ANICARD_P1 + Player),
			0,
			pStartPos.x,
			pStartPos.y,
			0,0,
			SWP_NOSIZE | SWP_NOZORDER);
		UpdateWindow(GetDlgItem(hWndParent, ANICARD_P1 + Player));
		for (j = 0; j < 8; j++)
			UpdateWindow(GetDlgItem(hWndParent,j + (8 * Player)));
		InvalidateRect(GetDlgItem(hWndParent, ANICARD_P1 + Player), NULL, false);

		for (j = 0; j < 4; j++)
			UpdateWindow(GetDlgItem(hWndParent, CT_PLAYER_1 + j));
		Sleep(10);
	}

	SetWindowPos(GetDlgItem(hWndParent, ANICARD_P1 + Player),
		0,
		pWndPos.x,
		pWndPos.y,
		0,0,
		SWP_NOSIZE | SWP_NOZORDER);
	UpdateWindow(GetDlgItem(hWndParent, ANICARD_P1 + Player));

	animationCards[Player] = NO_CARD;
	ShowWindow(GetDlgItem(hWndParent, ANICARD_P1 + Player), SW_HIDE);
	UpdateWindow(GetDlgItem(hWndParent, CT_PLAYER_1 + Player));

	return;
}


void PlayCantSound(HWND hwndParent)
{
	HANDLE hCantSound;
	HINSTANCE hInstance;

	hInstance = (HINSTANCE) GetWindowLong(hwndParent, GWL_HINSTANCE);
	hCantSound = LockResource(LoadResource(hInstance, 
		FindResource(hInstance, MAKEINTRESOURCE(IDR_W_STOP), "WAVE")));
	if (bSndOn)
		PlaySound((LPCTSTR)hCantSound, NULL, SND_MEMORY | SND_ASYNC);
	return;
}
