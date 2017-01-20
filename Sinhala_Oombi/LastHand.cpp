#include <windows.h>
#include "Common.h"
#include "LastHand.h"
#include "MyWinTools.h"

LONG WINAPI WndProc_LastHand (HWND, UINT, WPARAM, LPARAM);
LONG WINAPI WndProc_CardDeckLastHand (HWND, UINT, WPARAM, LPARAM);

int WINAPI ShowLastHand (HWND hWndParent)
{
	WNDCLASS wc;
	HWND hwnd;
	MSG msg;
	RECT rectMain;
	POINT p1;

	HINSTANCE hInstance = (HINSTANCE) GetWindowLong(
		hWndParent, GWL_HINSTANCE);
	GetClientRect(hWndParent, &rectMain);
	p1.x = 0;
	p1.y = 0;
	ClientToScreen(hWndParent, &p1);

	HBITMAP hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_VELVET));
	HBRUSH	hBrushBack = CreatePatternBrush(hBitmap);

	wc.style = 0;                                   // Class style
	wc.lpfnWndProc = (WNDPROC) WndProc_LastHand;		// window procedure address
	wc.cbClsExtra = 0;						// class extra bytes
	wc.cbWndExtra = 0;						// window extra bytes
	wc.hInstance  = hInstance;				// instance handler
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); // icon handler
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	// cursor handle
	wc.hbrBackground = (HBRUSH) (hBrushBack);	// background color
	wc.lpszMenuName = NULL;		// menu name
	wc.lpszClassName = "WndClass_ShowLastHand";		// WNDCLASS name

	RegisterClass (&wc);

	// card Deck
	wc.lpszClassName = "SinOombi_CDeckWc_LastHand";
	wc.lpfnWndProc = (WNDPROC) WndProc_CardDeckLastHand;
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW);
	RegisterClass(&wc);

	hwnd = CreateWindowEx(
		WS_EX_TOOLWINDOW,
		"WndClass_ShowLastHand",			// WNDCLASS name
		"",		// Window title
		WS_POPUP,	// Window Style
		p1.x,			// Horizontal position
		p1.y,			// Vertical position
		rectMain.bottom - rectMain.top,			// Initial Width
		rectMain.bottom - rectMain.top,			// Initial Height
		hWndParent,			// Handle of parent window
		NULL,					// Menu Handle
		hInstance,				// Application Instance handle
		NULL					// Window creation data
		);

	EnableWindow(hWndParent, false);
	ShowWindow (hwnd, SW_SHOW);
	UpdateWindow (hwnd);

	while  ( GetMessage (&msg, NULL, 0, 0) )
	{
		TranslateMessage (&msg);
		DispatchMessage(&msg);
	}

	DeleteObject(hBitmap);

	EnableWindow(hWndParent, true);
	SetFocus(hWndParent);
	return msg.wParam;
}

LRESULT CALLBACK WndProc_LastHand (HWND hwnd, UINT message, WPARAM wParam,
								   LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static HWND hwndButton;
	RECT rect;
	static HWND hCards[4];
	static int midX, midY;
	BYTE player;

	switch (message)
	{
	case WM_CREATE:
		//PosToMiddle(hwnd);
		GetClientRect(hwnd, &rect);
		midX = (rect.right - rect.left)/ 2;
		midY = (rect.bottom - rect.top)/ 2;
/*
		hwndButton = CreateWindow("BUTTON",
			"yﬂ",
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			(midX) - MSG_OK_X/2,
			rect.bottom - (2 * MSG_OK_Y),
			MSG_OK_X,
			MSG_OK_Y,
			hwnd,
			(HMENU) BUT_ONE,
			(HINSTANCE) GetWindowLong(GetParent(hwnd), GWL_HINSTANCE),
			NULL);
*/

		SendMessage(hwndButton, WM_SETFONT, (WPARAM)hSinFontSm, MAKELPARAM(true, 0)); 

		for (int i = 0; i <4; i++)
		{
			hCards[i] = CreateWindow("SinOombi_CDeckWc_LastHand",
				"",
				WS_VISIBLE | WS_CHILD,
				0,
				0,
				71,
				96,
				hwnd,
				(HMENU) (LAST_HAND1 + i),
				(HINSTANCE) GetWindowLong(GetParent(hwnd), GWL_HINSTANCE),
				NULL);
		}

		SetWindowPos(hCards[PLAYER_1],
			0,
			(midX - 71/2) + (71/5),
			(midX),
			0, 0,
			SWP_NOZORDER | SWP_NOSIZE);

		SetWindowPos(hCards[PLAYER_2],
			0,
			(midX),
			(midX - 96/2) - (96/5),
			0, 0,
			SWP_NOZORDER | SWP_NOSIZE);
		
		SetWindowPos(hCards[PLAYER_3],
			0,
			(midX - 71/2) - (71/5),
			(midX - 96),
			0, 0,
			SWP_NOZORDER | SWP_NOSIZE);
		SetWindowPos(hCards[PLAYER_4],
			0,
			(midX - 71),
			(midX - 96/2) + (96/5),
			0, 0,
			SWP_NOZORDER | SWP_NOSIZE);

		player = GetEnmy2(lastWinner);
		for (int i = 0; i < 4; i++)
		{
			SetWindowPos(hCards[player],
				HWND_TOP,
				0,
				0,
				0, 0,
				SWP_NOSIZE | SWP_NOMOVE);
			player = GetEnmy2(player);
		}

		Trans(hwnd, 230);
		SetFocus(hwndButton);

		return 0;
/*
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case BUT_ONE:
			DestroyWindow(hwnd);
			break;
		}
		return 0;
*/
	case WM_LBUTTONUP:
		DestroyWindow(hwnd);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		SelectObject(hdc, hSinFontL);
		SetTextAlign(hdc, TA_CENTER);
		SetBkMode(hdc, TRANSPARENT);

		if ((startPlayer == PLAYER_1) || (startPlayer == PLAYER_3))
			TextOut(hdc, midX, 20, "hg f.dv oskqÔ", strlen("hg f.dv oskqÔ"));
		else
			TextOut(hdc, midX, 20, "hg f.dv mrdohs", strlen("hg f.dv mrdohs"));

		On_Paint(hwnd, hdc); // call the main func to draw outer rect

		EndPaint (hwnd, &ps);

		return 0;
/*
	case WM_NCPAINT:
	case WM_NCACTIVATE:
		DefWindowProc(hwnd, message, wParam, lParam);
		hdc = GetWindowDC(hwnd);
		SetTextColor(hdc, GetSysColor(COLOR_CAPTIONTEXT));
		SetBkMode(hdc, TRANSPARENT);
		SelectObject(hdc, hSinFontSm);
		TextOut(hdc, 5,1, "hg f.dv", strlen("hg f.dv"));
		ReleaseDC(hwnd, hdc);
		return 0;
*/
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}


	return DefWindowProc (hwnd, message, wParam, lParam);
}


LRESULT CALLBACK WndProc_CardDeckLastHand (HWND hwnd, UINT message, WPARAM wParam,
										   LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HDC hdcMem;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hdcMem = CreateCompatibleDC(hdc);
		SelectObject(hdcMem, *phBitmap[lastHand[(GetDlgCtrlID(hwnd) - LAST_HAND1)]]);
		BitBlt(hdc, 0,0, 71,96, hdcMem, 0,0, SRCCOPY);
		DeleteDC(hdcMem);
		EndPaint (hwnd, &ps);

		return 0;

	case WM_LBUTTONUP:
		DestroyWindow(GetParent(hwnd));
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}