#include <windows.h>
#include "Common.h"
#include "resource.h"

LRESULT CALLBACK WndProc_CardDeck (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HDC hdcMem;

	switch (message)
	{

	case WM_LBUTTONUP:
		if (PLAYER_1 == (GetDlgCtrlID(hwnd) / 8))
		{
			if (bCardPlayable)
			{
				Player_PlayCard(HitTest(hwnd));
			}
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		if (BLANK_CARD != playaCards[GetDlgCtrlID(hwnd) / 8][GetDlgCtrlID(hwnd) % 8])
		{
			hdcMem = CreateCompatibleDC(hdc);
			if (bTrumpsNotYet)
			{
				if ((((GetDlgCtrlID(hwnd) % 8) < 4) && (PLAYER_1 == (GetDlgCtrlID(hwnd) / 8))) ||
					bCheatMode)
					SelectObject(hdcMem, *phBitmap[playaCards[GetDlgCtrlID(hwnd) / 8][GetDlgCtrlID(hwnd) % 8]]);
				else
					SelectObject(hdcMem, *phCoverBitmap);
			}
			else
			{
				if ((PLAYER_1 == (GetDlgCtrlID(hwnd) / 8)) || bCheatMode)
					SelectObject(hdcMem, *phBitmap[playaCards[GetDlgCtrlID(hwnd) / 8][GetDlgCtrlID(hwnd) % 8]]);
				else
					SelectObject(hdcMem, *phCoverBitmap);
			}  

			//BitBlt(hdc, 0, 0, CARD_X, CARD_Y, hdcMem, 0, 0, SRCCOPY);
			StretchBlt(hdc, 0,0, CARD_X, CARD_Y, hdcMem, 0,0, 71,96, SRCCOPY);
			DeleteDC(hdcMem);
		}
		EndPaint(hwnd, &ps);
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}


LRESULT CALLBACK WndProc_CardTableDropped (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HDC hdcMem;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		if (NO_CARD != cardTable[GetDlgCtrlID(hwnd) - 32])
		{
			hdcMem = CreateCompatibleDC(hdc);
			SelectObject(hdcMem, *phBitmap[(int)cardTable[GetDlgCtrlID(hwnd) - 32]]);

			//BitBlt(hdc, 0, 0, CARD_X, CARD_Y, hdcMem, 0, 0, SRCCOPY);
			StretchBlt(hdc, 0,0, CARD_X, CARD_Y, hdcMem, 0,0, 71,96, SRCCOPY);
			DeleteDC(hdcMem);
		}
		EndPaint(hwnd, &ps);
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

// hit testing the cards
HWND HitTest(HWND hwnd)
{
	POINT point;
	HWND retHwnd = hwnd;
	int hitPart;
	int count;

	GetCursorPos(&point);
	ScreenToClient(hwnd, &point);
	hitPart = point.x / CARD_GAP;

	retHwnd = GetDlgItem(GetParent(hwnd), min(7, GetDlgCtrlID(hwnd) + hitPart));

	count = GetDlgCtrlID(retHwnd);
	while (!IsWindowVisible(retHwnd))
		retHwnd = GetDlgItem(GetParent(hwnd), count--);

	return retHwnd;
}



LRESULT CALLBACK WndProc_TrumpGetting(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc, hdcMem;
	HBITMAP hBitmap;
	DWORD mode;
	BYTE i;


	switch(message)
	{

	case WM_MOUSEMOVE:
		SetFocus(hwnd);
		return 0;

	case WM_SETFOCUS:
	case WM_KILLFOCUS:
		if (bSndOn)
			PlaySound((LPCTSTR) hSelectSnd, NULL, SND_MEMORY | SND_ASYNC);
		InvalidateRect(hwnd, NULL, false);
		return 0;

	case WM_PAINT:
		if (hwnd == GetFocus())
			mode = NOTSRCCOPY;
		else
			mode = SRCCOPY;

		hdc = BeginPaint(hwnd, &ps);
		if (bTrumpsNotYet)
		{
			hdcMem = CreateCompatibleDC(hdc);
			hBitmap = LoadBitmap((HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
				MAKEINTRESOURCE((GetDlgCtrlID(hwnd) - TRMP_GET_SPADE + IDB_SPADE)));

			SelectObject(hdcMem, hBitmap);
			StretchBlt(hdc, 0,0,CARD_X, CARD_X, hdcMem,0,0, 100,100, mode);

			DeleteObject(hBitmap);
			DeleteDC(hdcMem);

			DrawTextInMainWnd(GetParent(hwnd));
		}
		EndPaint(hwnd, &ps);
		return 0;

	case WM_LBUTTONUP:
		trumps = GetDlgCtrlID(hwnd) - TRMP_GET_SPADE;
		for (i = 0; i < 4; i++)
			ShowWindow(GetDlgItem(GetParent(hwnd), TRMP_GET_SPADE+i), SW_HIDE);

		bTrumpsNotYet = false;
		InvalidateRect(GetParent(hwnd), NULL, true);
		return 0;

		// pass keyboard msg to main
	case WM_KEYUP:
		SendMessage(GetParent(hwnd), message, wParam, lParam);
		return 0;

	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}


void DrawTextInMainWnd(HWND hwndParent)
{
	HDC hdc;
	RECT rect;

	GetClientRect(hwndParent, &rect);
	hdc = GetDC(hwndParent);
	SetTextAlign(hdc, TA_CENTER);
	//SetBkColor(hdc, RGB(0,127,0));
	SetBkMode(hdc, TRANSPARENT);
	SelectObject(hdc, hSinFontL);

	TextOut(hdc, 
		(rect.bottom - rect.top)/ 2,
		((rect.bottom - rect.top)/ 2) - (CARD_X),
		";=reïmq lshkak", 14);


	ReleaseDC(hwndParent, hdc);
	return;
}
