#include <windows.h>

#include "resource.h"
#include "common.h"
#include "MyWinTools.h"

// Globals
BYTE cardDeck[32];
BYTE trumps;
BYTE downCards[32];
BYTE playaCards[4][8];
BYTE playaFlags[4][4];
BYTE playaCardPosVal[4][8];
BYTE cardTable[4];
BYTE startPlayer;
BYTE gameStartPlayer;
BYTE lastHand[4];
BYTE lastWinner;
BYTE cardCount[4];

BYTE animationCards[4];

BYTE winSets;
BYTE lostSets;
BYTE winPoints;
BYTE lostPoints;
BYTE rounds;

BYTE bIsLastSeporu;

BYTE bCardPlayable;
BYTE bTrumpsNotYet;

BYTE bCheatMode;
BYTE bCheatApplied;

HFONT hSinFontSm;
HFONT hSinFontMe;
HFONT hSinFontL;

BYTE bSndOn;
HANDLE hDealSound;
HANDLE hSelectSnd;
HANDLE hCutSnd;

HBITMAP *phBitmap[32];
HBITMAP *phCoverBitmap;

int WIN_X;
int WIN_Y;

int cWIN_X;
int cWIN_Y;

int CARD_X;
int CARD_Y;

int cCARD_X;
int cCARD_Y;

int CARD_GAP;

int STATUS_R_X;
int STATUS_R_Y;


// prototypes
LONG WINAPI WndProcMain (HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine, int nCmdShow)
{
    WNDCLASS wc;
    HWND hwnd;
    MSG msg;
	HBRUSH hBrushBack, hBrushCard;
	HBITMAP hBitmap;
	RECT wndRect;
	SetRect(&wndRect, 0,0, 710, 576);

	AdjustWindowRect(&wndRect,  
		WS_THICKFRAME | WS_MINIMIZEBOX | WS_SYSMENU, 
		true);

	cWIN_X = wndRect.right - wndRect.left;
	cWIN_Y = wndRect.bottom - wndRect.top;

	CARD_X = 71;
	CARD_Y = 576 / 6;
	CARD_GAP = CARD_X / 5;
	STATUS_R_X = 710 - 576;
	STATUS_R_Y = 576;

	// cheat mode 1.05
	bCheatMode = 0;
	bCheatApplied = false;

	// sound on off 1.07
	bSndOn = true;

	hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_VELVET));
	hBrushBack = CreatePatternBrush(hBitmap);
	hBrushCard = CreateSolidBrush(RGB(255, 255, 255));

    wc.style = 0;                                   // Class style
	wc.lpfnWndProc = (WNDPROC) WndProcMain;		// window procedure address
	wc.cbClsExtra = 0;						// class extra bytes
	wc.cbWndExtra = 0;						// window extra bytes
	wc.hInstance  = hInstance;				// instance handler
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON)); // icon handler
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	// cursor handle
	wc.hbrBackground = hBrushBack;	// background color
	wc.lpszMenuName = NULL;		// menu name
	wc.lpszClassName = "SinOombi_mainWnd";		// WNDCLASS name
	RegisterClass (&wc);

	// card Deck
	wc.lpszClassName = "SinOombi_CDeckWc";
	wc.lpfnWndProc = (WNDPROC) WndProc_CardDeck;
	wc.hbrBackground = hBrushCard;
	RegisterClass(&wc);

	// Dropped Cards on the table
	wc.lpszClassName = "SinOombi_CTDropWc";
	wc.lpfnWndProc = (WNDPROC) WndProc_CardTableDropped;
	RegisterClass(&wc);

	// animation cards
	wc.lpszClassName = "So_AniCWnd";
	wc.lpfnWndProc = (WNDPROC) WndProc_AnimationCards;
	RegisterClass(&wc);

	// trumpGeting cards
	wc.lpszClassName = "So_TrGCrdWc";
	wc.lpfnWndProc = (WNDPROC) WndProc_TrumpGetting;
	wc.hCursor = LoadCursor(NULL, IDC_HAND);	// cursor handle
	RegisterClass(&wc);

	// Right HandSide Status bar
	wc.lpszClassName = "So_RhSWc";
	wc.lpfnWndProc = (WNDPROC) WndProc_Rhs_StatusBar;
	wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	// cursor handle
	RegisterClass(&wc);


	hwnd = CreateWindow(
		"SinOombi_mainWnd",			// WNDCLASS name
		"Oombi_Sinhala_v1.09",		// Window title
		WS_OVERLAPPED | WS_THICKFRAME | WS_MINIMIZEBOX | WS_SYSMENU,	// Window Style
		CW_USEDEFAULT,			// Horizontal position
		CW_USEDEFAULT,			// Vertical position
		cWIN_X,			// Initial Width
		cWIN_Y,			// Initial Height
		HWND_DESKTOP,			// Handle of parent window
		DefineMenu(),					// Menu Handle
		hInstance,				// Application Instance handle
		NULL					// Window creation data
		);

	PosToMiddle(hwnd);
	ShowWindow (hwnd, nCmdShow);
	UpdateWindow (hwnd);

	while  ( GetMessage (&msg, NULL, 0, 0) )
	{
		TranslateMessage (&msg);
		DispatchMessage(&msg);
	}

	DeleteObject(hBitmap);
	DeleteObject(hBrushBack);
	DeleteObject(hBrushCard);
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProcMain (HWND hwnd, UINT message, WPARAM wParam,
						  LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		On_Create(hwnd, wParam, lParam);
		return 0;

	case WM_SIZE:
		On_Size(hwnd, wParam, lParam);
		return 0;

	case WM_SIZING:
		On_Sizing(hwnd, wParam, lParam);
		return true;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
        On_Paint(hwnd, hdc);
		EndPaint (hwnd, &ps);
		return 0;

	case WM_TIMER:
		On_Timer(hwnd, wParam, lParam);
		return 0;

	case WM_MEASUREITEM:
		On_MeasureItem(hwnd, (LPMEASUREITEMSTRUCT) lParam);
		return 0;

	case WM_DRAWITEM:
		On_DrawItem(hwnd, (LPDRAWITEMSTRUCT) lParam);
		return 0;

	case WM_COMMAND:
		On_Command(hwnd, wParam, lParam);
		return 0;

	case WM_KEYUP:
		On_KeyUp(hwnd, wParam, lParam);
		return 0;

	case WM_CLOSE:
		On_Close(hwnd, wParam, lParam);
		return 0;

	case WM_DESTROY:
		On_Destroy(hwnd, wParam, lParam);
		PostQuitMessage(0);
		return 0;
	}


	return DefWindowProc (hwnd, message, wParam, lParam);
}