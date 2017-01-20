#include <windows.h>

#include "Win_Msgs.h"
#include "Common.h"

// after v 1.02
void ShowEaster(HWND hwnd);

// after 1.05
void ShowCheatMode(HWND);

void On_Create(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	//	HMENU hMenu;
	HINSTANCE hInstance;
	int i;
	DWORD iFntSize;
	void *hFnt;
	DWORD fntInstalled = 1;


	hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);

	//	hMenu = GetSystemMenu(hwnd, false);
	//	RemoveMenu(hMenu, SC_SIZE, MF_BYCOMMAND);

	// add font to fnt:resource table
	hFnt = LockResource(LoadResource(hInstance,
		FindResource(hInstance, MAKEINTRESOURCE(IDR_SINFNT), "FNT")));
	iFntSize = SizeofResource(hInstance,
		FindResource(hInstance, MAKEINTRESOURCE(IDR_SINFNT), "FNT"));

	if (!AddFontMemResourceEx_dyn(hFnt, iFntSize, 0, &fntInstalled))
	{
		MessageBox(hwnd, "SinhalaOombi will only run on Win 2000/XP/Vista only",
			"Err: Loading Sinhala Font", MB_ICONERROR);
		DestroyWindow(hwnd);
		return ;
	}

	hSinFontSm = CreateFont(18, 0,0,0,0,0,0,0,0,0,0,0,0,"DL-Araliya..");
	hSinFontMe = CreateFont(20, 0,0,0,0,0,0,0,0,0,0,0,0,"DL-Araliya..");
	hSinFontL = CreateFont(28, 0,0,0,0,0,0,0,0,0,0,0,0,"DL-Araliya..");

	// init CardDeck bitmaps
	for (i = 0; i < 32; i++)
	{
		phBitmap[i] = new HBITMAP;
		*phBitmap[i] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_SPADE_7 + i));
	}
	phCoverBitmap = new HBITMAP;
	*phCoverBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_CARD_BACK));

	InitOomibiDeck();

	// start with the user
	startPlayer = PLAYER_1;
	gameStartPlayer = PLAYER_1;

	winPoints = 0;
	lostPoints = 0;

	for (i = 0; i < 4; i++)
		cardCount[i] = 0;

	rounds = 0;
	bIsLastSeporu = false;
	bTrumpsNotYet = true;

	// load sounds
	hDealSound = LockResource(LoadResource(hInstance,
		FindResource(hInstance, MAKEINTRESOURCE(IDR_W_DEAL), "WAVE")));
	hSelectSnd = LockResource(LoadResource(hInstance,
		FindResource(hInstance, MAKEINTRESOURCE(IDR_W_SEL), "WAVE")));
	hCutSnd = LockResource(LoadResource(hInstance,
		FindResource(hInstance, MAKEINTRESOURCE(IDR_W_CUT), "WAVE")));

	for (i = 0; i < 32; i++)
		CreateWindow(
		"SinOombi_CDeckWc",// WNDCLASS name
		"",		// Window title
		WS_CHILD | WS_VISIBLE,	// Window Style
		0,			// Horizontal position
		0,			// Vertical position
		CARD_X,			// Initial Width
		CARD_Y,			// Initial Height
		hwnd,			// Handle of parent window
		(HMENU) i,					// Menu Handle
		hInstance,
		NULL					// Window creation data
		);

	for ( i = 0; i < 4; i++)
	{
		CreateWindow(
			"SinOombi_CTDropWc",// WNDCLASS name
			"",		// Window title
			WS_CHILD,	// Window Style
			0,			// Horizontal position
			0,			// Vertical position
			CARD_X,			// Initial Width
			CARD_Y,			// Initial Height
			hwnd,			// Handle of parent window
			(HMENU) (i + 32),					// Menu Handle
			hInstance,
			NULL					// Window creation data
			);
		lastHand[i] = NO_CARD;
	}

	CreateWindow(
		"So_RhSWc",
		"",
		WS_CHILD | WS_VISIBLE,
		0,
		0,
		0,
		0,
		hwnd,
		(HMENU) RHS_STATUSID,
		hInstance,
		NULL);

	// animation cards
	for (i = 0; i < 4; i++)
		CreateWindow(
		"So_AniCWnd",
		"",
		WS_CHILD | WS_VISIBLE,
		i * CARD_X,0,
		CARD_X, CARD_Y,
		hwnd,
		(HMENU) (ANICARD_P1 + i),
		hInstance,
		0);

	// trumpgetting cards
	for (i = 0; i < 4; i++)
		CreateWindow(
		"So_TrGCrdWc",
		"",
		WS_CHILD,
		0,0,
		CARD_X, CARD_X,
		hwnd,
		(HMENU) (TRMP_GET_SPADE + i),
		hInstance,
		0);


	// can play!
	SetTimer(hwnd, TIMER_PLAY_START, 700, NULL);

	return;
}

void On_Size(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	int i;
	const int x = (int) LOWORD(lParam);
	const int y = (int) HIWORD(lParam);
	WIN_X = x;
	WIN_Y = y;
	CARD_X = x / 10;
	CARD_Y = y / 6;
	CARD_GAP = CARD_X / 5;
	STATUS_R_X = WIN_X - WIN_Y;
	STATUS_R_Y = WIN_Y;


	// set positions
	// player_1 // user
	for (i = 0; i < 8; i++)
		SetWindowPos(GetDlgItem(hwnd,i),
		0,
		PLAYA1_CARD_X(y) + (i * (CARD_X / 5)),
		PLAYA1_CARD_Y(y),
		CARD_X, CARD_Y,
		SWP_NOZORDER);

	// player_2
	for (i = 0; i < 8; i++)
		SetWindowPos(GetDlgItem(hwnd,i + (8 * PLAYER_2)),
		0,
		PLAYA2_CARD_X(y),
		PLAYA2_CARD_Y(y) - (i * (CARD_X / 5)),
		CARD_X, CARD_Y,
		SWP_NOZORDER);

	// player_3
	for (i = 0; i < 8; i++)
		SetWindowPos(GetDlgItem(hwnd,i + (8 * PLAYER_3)),
		0,
		PLAYA3_CARD_X(y) - (i * (CARD_X / 5)),
		PLAYA3_CARD_Y,
		CARD_X, CARD_Y,
		SWP_NOZORDER);

	// player_4
	for (i = 0; i < 8; i++)
		SetWindowPos(GetDlgItem(hwnd,i + (8 * PLAYER_4)),
		0,
		PLAYA4_CARD_X ,
		PLAYA4_CARD_Y(y) + (i * (CARD_X / 5)),
		CARD_X, CARD_Y,
		SWP_NOZORDER);

	// Ani cards
	for (i = 0; i < 4; i++)
		SetWindowPos(GetDlgItem(hwnd,ANICARD_P1 + i),
		0,
		0 ,
		0,
		CARD_X, CARD_Y,
		SWP_NOMOVE | SWP_NOZORDER);

	// position the card table cards
	// CARD_PLAYER_1
	SetWindowPos(GetDlgItem(hwnd, CT_PLAYER_1),
		0,
		CT_P1_X(y), CT_P1_Y(y),
		CARD_X, CARD_Y,
		SWP_NOZORDER);

	// CARD_PLAYER_2
	SetWindowPos(GetDlgItem(hwnd, CT_PLAYER_2),
		0,
		CT_P2_X(y),CT_P2_Y(y),
		CARD_X, CARD_Y,
		SWP_NOZORDER);

	// CARD_PLAYER_3
	SetWindowPos(GetDlgItem(hwnd, CT_PLAYER_3),
		0,
		CT_P3_X(y), CT_P3_Y(y),
		CARD_X, CARD_Y,
		SWP_NOZORDER);

	// CARD_PLAYER_4
	SetWindowPos(GetDlgItem(hwnd, CT_PLAYER_4),
		0,
		CT_P4_X(y), CT_P4_Y(y),
		CARD_X, CARD_Y,
		SWP_NOZORDER);

	// status bar
	SetWindowPos(GetDlgItem(hwnd, RHS_STATUSID),
		0,
		y,
		0,
		x - y,
		y,
		SWP_NOZORDER);

	// trumpgetting cards
	for (i = 0; i < 4; i++)
		SetWindowPos(GetDlgItem(hwnd,i + TRMP_GET_SPADE),
		0,
		((CARD_X + 2) * i) + (y / 2) - (2 * CARD_X),
		(y - CARD_X) / 2 ,
		CARD_X, CARD_X,
		SWP_NOZORDER);


	InvalidateRect(hwnd, NULL, true);
	return;
}


void On_Timer(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	BYTE player = startPlayer;
	BYTE card;
	BYTE i;

	switch (wParam)
	{
	case TIMER_PLAY_START:
		KillTimer(hwnd, TIMER_PLAY_START);
		bCardPlayable = false;
		GetTrumpsFromUser(hwnd);
		break;

	case TIMER_WAITTILL_TRUMPS:
		if (!bTrumpsNotYet)
		{
			KillTimer(hwnd, TIMER_WAITTILL_TRUMPS);
			SortPlayaCards();
			InvalidateRect(GetDlgItem(hwnd, RHS_STATUSID), NULL, false);
			for (i = 0; i < 8; i++)
				InvalidateRect(GetDlgItem(hwnd, i), NULL, false);

			bCardPlayable = true;
		}
		break;

	case TIMER_CLEAR:
		KillTimer(hwnd, TIMER_CLEAR);
		ShowEndAnimation(hwnd);
		SetTimer(hwnd, TIMER_PLAY, 500, NULL);
		break;

	case TIMER_PLAY:
		KillTimer(hwnd, TIMER_PLAY);
		if (startPlayer != PLAYER_1)
		{
			while (player != PLAYER_1)
			{
				card = Play_NextCard(player);
				cardTable[player] = NO_CARD;
				Update_CardTable(hwnd);

				// ani
				ShowPlayAnimation(
					hwnd,
					card,
					player);

				cardTable[player] = card;
				Update_CardTable(hwnd);
				player = NextPlayer(player);
			}
		}
		bCardPlayable = true;
		break;
	}

	return;
}


// cleanup
void On_Destroy(HWND, WPARAM, LPARAM)
{
	for (int i = 0; i < 32; i++)
	{
		DeleteObject(*phBitmap[i]);
		delete phBitmap[i];
	}
	DeleteObject(*phCoverBitmap);
	delete phCoverBitmap;

	DeleteObject(hSinFontL);
	DeleteObject(hSinFontMe);
	DeleteObject(hSinFontSm);

	FreeResource(hDealSound);
	FreeResource(hSelectSnd);
	FreeResource(hCutSnd);

	return;
}


void On_Paint(HWND hwnd, HDC hdc)
{
	RECT rect;
	HPEN hPen;

	GetClientRect(hwnd, &rect);

	//	rect.top += 1;
	//	rect.left+= 1;
	//	rect.bottom -= 1;
	rect.right = rect.bottom;

	hPen = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DLIGHT));
	SelectObject(hdc, hPen);
	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Rectangle(hdc, rect.top, rect.left, rect.bottom, rect.right);

	DeleteObject(hPen);
	return;
}

void On_Close(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	OO_MSGBOXDATA mbdata;

	mbdata.butText1 = "Tõ";
	mbdata.butText2 = "keye";
	mbdata.hwndParent = hwnd;
	mbdata.msgBoxType = MB_TWOBUTTON | MBIT_QUESTION;
	mbdata.MsgStr = "whskafjkak wjYHu o@";
	mbdata.Title = "whskafjkak";

	if (BUT_ONE == ShowMsgBox(&mbdata))
		DestroyWindow(hwnd);

	return ;
}


void On_Command(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	OO_MSGBOXDATA omb;

	switch (LOWORD(wParam))
	{
	case IDM_GAME_NEW:
		AskAndGiveNewGame(hwnd, false);
		break;

	case IDM_GAME_SOUND:
		bSndOn ^= 1;
		InvalidateRect(hwnd, NULL, false);
		break;

	case IDM_GAME_EXIT:
		SendMessage(hwnd, WM_CLOSE, 0, 0);
		break;

	case IDM_HELP_HELP:
		CopyAndShowReadme(hwnd);
		break;

	case IDM_HELP_VHIST:
		CopyAndShowVerHist(hwnd);
		break;

	case IDM_HELP_ABOUT:
		ShowAbout(hwnd);
		break;

	case IDM_HELP_LASTHAND:
		if (lastHand[0] != NO_CARD)
			ShowLastHand(hwnd);
		else
		{
			omb.butText1 = "yß";
			omb.hwndParent = hwnd;
			omb.MsgStr = ";ju tl f.dvla j;a .sfh kEfk'''";
			omb.Title = "hg f.dv";
			omb.msgBoxType = MB_ONEBUTTON | MBIT_EXCLAMATION;

			ShowMsgBox(&omb);
		}
		break;
	}

	return;
}


// resizable code!
void On_Sizing(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	RECT *pRect = (RECT *)lParam;
	int width = max(cWIN_X, pRect->right - pRect->left);
	int height = max(cWIN_Y, pRect->bottom - pRect->top);
	int wndSze;

	switch (wParam)
	{
	case WMSZ_TOPLEFT:
	case WMSZ_TOP:
		wndSze = (width - STATUS_WIDTH == height) ? (width - STATUS_WIDTH) : height;
		pRect->left = pRect->right - wndSze - STATUS_WIDTH;
		pRect->top = pRect->bottom - wndSze;
		break;

	case WMSZ_BOTTOMRIGHT:
	case WMSZ_BOTTOM:
		wndSze = (width - STATUS_WIDTH == height) ? (width - STATUS_WIDTH) : height;
		pRect->right = pRect->left + wndSze + STATUS_WIDTH;
		pRect->bottom = pRect->top + wndSze;
		break;

	case WMSZ_RIGHT:
		wndSze = (width - STATUS_WIDTH != height) ? (width - STATUS_WIDTH) : height;
		pRect->right = pRect->left + wndSze + STATUS_WIDTH;
		pRect->bottom = pRect->top + wndSze;
		break;

	case WMSZ_LEFT:
		wndSze = (width - STATUS_WIDTH != height) ? (width - STATUS_WIDTH) : height;
		pRect->left = pRect->right - wndSze - STATUS_WIDTH;
		pRect->top = pRect->bottom -  wndSze;
		break;

	case WMSZ_TOPRIGHT:
		wndSze = (width - STATUS_WIDTH == height) ? (width - STATUS_WIDTH) : height;
		pRect->right = pRect->left + wndSze + STATUS_WIDTH;
		pRect->top = pRect->bottom - wndSze;
		break;


	case WMSZ_BOTTOMLEFT:
		wndSze = (width - STATUS_WIDTH == height) ? (width - STATUS_WIDTH) : height;
		pRect->left = pRect->right - wndSze - STATUS_WIDTH;
		pRect->bottom = pRect->top + wndSze;
		break;
	}

	return;
}

// keyboard
void On_KeyUp(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case VK_ESCAPE:
		ShowWindow(hwnd, SW_MINIMIZE);
		break;

	case VK_F1:
		SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDM_HELP_HELP, 0), 0);
		break;

	case VK_F2:
		SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDM_GAME_NEW, 0), 0);
		break;

	case VK_F5:
		SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDM_HELP_ABOUT, 0), 0);
		break;

	case VK_F4:
		SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDM_HELP_LASTHAND, 0), 0);
		break;

	case VK_F9:
		if (GetKeyState(VK_CONTROL) < 0)
			ShowEaster(hwnd);
		break;

	case VK_F3:
		if (GetKeyState(VK_CONTROL) < 0)
			ShowCheatMode(hwnd);
			break;
	}
	return;
}

void ShowEaster(HWND hwnd)
{
	OO_MSGBOXDATA omb;
	omb.butText1 = "yß";
	omb.hwndParent = hwnd;
	omb.MsgStr = "071 9021733";
	omb.Title = "ÿrl:k wxl";
	omb.msgBoxType = MB_ONEBUTTON | MBIT_INFORMATION;

	ShowMsgBox(&omb);
	return;
}


void ShowCheatMode(HWND hwnd)
{
	OO_MSGBOXDATA omb;
	omb.butText1 = "yß";
	omb.hwndParent = hwnd;
	omb.Title = "fydr l%uh";
	omb.msgBoxType = MB_ONEBUTTON | MBIT_INFORMATION;

	if (!bCheatMode)
	{
		bCheatApplied = true;
		omb.MsgStr = "fydr lrk tl kï fyd| jevla fkfuhs'''''";
	}
	else
		omb.MsgStr = "wdkak tfyuhs fi,a,ï lrkak òks''''''";

	bCheatMode ^= 1;
	InvalidateRect(hwnd, NULL, NULL);

	ShowMsgBox(&omb);
}

