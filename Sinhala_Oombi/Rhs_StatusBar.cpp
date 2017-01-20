#include <windows.h>
#include "Common.h"
#include "Rhs_Statusbar.h"

LRESULT CALLBACK WndProc_Rhs_StatusBar(HWND hwnd, UINT message, WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	static int cx, cy;
	HDC hdc;

	switch (message)
	{
	case WM_SIZE:
		cx = (int) LOWORD(lParam);
		cy = (int) HIWORD(lParam);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		PrintTrumpsBox(hwnd, hdc, cx, cy);
		PrintWinnedSetes(hdc, cx, cy);
		PrintPoints(hdc, cx, cy);

		DrawNIcons(hwnd, hdc, cx, cy);

		EndPaint(hwnd, &ps);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

void PrintTrumpsBox(HWND hwnd, HDC hdc, int cx, int cy)
{
	HPEN hPen;
	const char szTrumps[] = ";=reïmq";
	const int unit = cy / 12;
	HDC hdcMem;
	HBITMAP hBitmap;
	HINSTANCE hInst;

	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);

	hdcMem = CreateCompatibleDC(hdc);

	// covering box
	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	hPen = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DHILIGHT));
	SelectObject(hdc, hPen);
	RoundRect(hdc, cx / 8, unit + 10,
		(7 * cx / 8) , unit + 35 + (cx /2), 10, 10); 
	DeleteObject(hPen);

	hPen = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
	SelectObject(hdc, hPen);
	RoundRect(hdc, (cx / 8) + 1, (unit + 10) + 1,
		(7 * cx / 8)+ 1 , (unit + 35 + (cx /2)) + 1, 10, 10); 
	DeleteObject(hPen);

	// text
	SelectObject(hdc, hSinFontMe);
	SetTextAlign(hdc, TA_CENTER);
	SetBkMode(hdc, OPAQUE);
	SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));
	SetTextColor(hdc, GetSysColor(COLOR_BTNTEXT));
	TextOut(hdc,(int) cx /2, (int)unit * 1, szTrumps, strlen(szTrumps));

	// bmp
	if (bTrumpsNotYet)
	{
		hBitmap = LoadBitmap((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), 
			MAKEINTRESOURCE(IDB_QUESTION));
	}
	else
	{
		hBitmap = LoadBitmap((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			MAKEINTRESOURCE((IDB_SPADE + (trumps % 4))));
	}

	//if (bTrumpsNotYet)
	//{
	// //       pBitmap = new Bitmap((WCHAR *)LockResource(LoadResource(hInst, FindResource(
	////			hInst, MAKEINTRESOURCE(IDB_QUESTION), "PNG"))), false);
	//		pBitmap = new Bitmap(L"QUESTION.png", false);
	//	}
	//	else
	//		pBitmap = new Bitmap((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),(WCHAR *)
	//			MAKEINTRESOURCE((IDB_SPADE + (trumps % 4))));
	//
	//	pBitmap->GetHBITMAP(NULL, &hBitmap);
	SelectObject(hdcMem, hBitmap);

	StretchBlt(hdc, cx / 4,(unit * 1) + 25,cx / 2, cx / 2, hdcMem, 0, 0, 100, 100, SRCCOPY);

	DeleteObject(hBitmap);
	DeleteDC(hdcMem);
}


void PrintWinnedSetes(HDC hdc, int cx, int cy)
{
	const int unit = cy / 12;
	char szWinnedSets[] = "f.dvj,a";
	const char szWins[] = "oskqï";
	const char szLost[] = "meros";
	HPEN hPen;
	char txtBuffer[10];

	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	hPen = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DHILIGHT));
	SelectObject(hdc, hPen);
	RoundRect(hdc, cx / 8, unit * 5,
		cx * 7 / 8, unit * 7, 10, 10); 
	DeleteObject(hPen);

	hPen = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
	SelectObject(hdc, hPen);
	RoundRect(hdc, (cx / 8) + 1, (unit * 5)+ 1,
		(cx * 7 / 8)+ 1, (unit * 7) + 1, 10, 10); 
	DeleteObject(hPen);

	SelectObject(hdc, hSinFontMe);
	SetTextAlign(hdc, TA_CENTER);
	SetBkMode(hdc, OPAQUE);
	SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));
	TextOut(hdc, cx / 2, (unit * 5) - 10, szWinnedSets, strlen(szWinnedSets));

	SelectObject(hdc, hSinFontSm);
	SetTextAlign(hdc, TA_LEFT);
	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, (cx /8) + 10, (unit * 5) + 20, szWins, strlen(szWins));
	TextOut(hdc, (cx /8) + 10, (unit * 7) - 35, szLost, strlen(szLost));

	SetTextAlign(hdc, TA_RIGHT);
	TextOut(hdc, (cx * 7/8) - 10, (unit * 5) + 20, 
		CFB(winSets, txtBuffer),
		strlen(CFB(winSets, txtBuffer)));
	TextOut(hdc, (cx * 7/8) - 10, (unit * 7) - 35, 
		CFB(lostSets, txtBuffer),
		strlen(CFB(lostSets, txtBuffer)));

	SetTextAlign(hdc, TA_CENTER);
	TextOut(hdc, cx / 2, (unit * 5) + 20, "(", 1);
	TextOut(hdc, cx / 2, (unit * 7) - 35, "(", 1);
	return;
}

void PrintPoints(HDC hdc,int cx, int cy)
{
	HPEN hPen;
	const int unit = cy / 12;
	const char szPoint[] = "leg";
	const char szWins[] = "oskqï";
	const char szLost[] = "meros";
	char txtBuffer[10];

	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	hPen = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DHILIGHT));
	SelectObject(hdc, hPen);
	RoundRect(hdc, cx / 8, unit * 8,
		cx * 7 / 8, unit * 10, 10, 10); 
	DeleteObject(hPen);

	hPen = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
	SelectObject(hdc, hPen);
	RoundRect(hdc, (cx / 8) + 1, (unit * 8) + 1,
		(cx * 7 / 8) + 1, (unit * 10) + 1, 10, 10); 
	DeleteObject(hPen);

	SelectObject(hdc, hSinFontMe);
	SetTextAlign(hdc, TA_CENTER);
	SetBkMode(hdc, OPAQUE);
	SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));
	TextOut(hdc, cx / 2, (unit * 8) - 10, szPoint, strlen(szPoint));

	SelectObject(hdc, hSinFontSm);
	SetTextAlign(hdc, TA_LEFT);
	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, (cx /8) + 10, (unit * 8) + 20, szWins, strlen(szWins));
	TextOut(hdc, (cx /8) + 10, (unit * 10) - 35, szLost, strlen(szLost));

	SetTextAlign(hdc, TA_RIGHT);
	TextOut(hdc, (cx * 7/8) - 10, (unit * 8) + 20, 
		CFB(winPoints, txtBuffer),
		strlen(CFB(winPoints, txtBuffer)));
	TextOut(hdc, (cx * 7/8) - 10, (unit * 10) - 35, 
		CFB(lostPoints, txtBuffer),
		strlen(CFB(lostPoints, txtBuffer)));

	SetTextAlign(hdc, TA_CENTER);
	TextOut(hdc, cx / 2, (unit * 8) + 20, "(", 1);
	TextOut(hdc, cx / 2, (unit * 10) - 35, "(", 1);

	return;
}

// draws the icons
void DrawNIcons(HWND hwnd, HDC hdc, int cx, int cy)
{
	const int unit = cy / 12;
	HPEN hPen;

	HINSTANCE hInst;
	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);

	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	hPen = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DHILIGHT));
	SelectObject(hdc, hPen);
	RoundRect(hdc, cx / 8, (unit * 11) - 3,
		cx * 7 / 8, (unit * 11) + GetSystemMetrics(SM_CYICON) + 3 ,
		10, 10); 
	DeleteObject(hPen);

	hPen = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
	SelectObject(hdc, hPen);
	RoundRect(hdc, (cx / 8) + 1, (unit * 11) - 2,
		(cx * 7 / 8) + 1, (unit * 11) + GetSystemMetrics(SM_CYICON) + 4 ,
		10, 10); 
	DeleteObject(hPen);

	HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICO_SON));
	DrawIcon(hdc, (cx - GetSystemMetrics(SM_CXICON))/ 2 ,unit * 11, hIcon);
	DeleteObject(hIcon);

	if (!bSndOn)
	{
		hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICO_SOFF));
		DrawIcon(hdc, (cx - GetSystemMetrics(SM_CXICON))/ 2 ,unit * 11, hIcon);
		DeleteObject(hIcon);
	}
	return;
}


inline char *CFB(BYTE inVal, char* Buffer)
{
	int val = (int) inVal;
	itoa(val, Buffer, 10);
	return Buffer;
}