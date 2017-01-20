#include <windows.h>
#include "common.h"

struct MENUSTRINGS
{
	int iMenuId;
	char *szMenuStr;
} MenuStr[] =
{
	IDM_NULL,			"",

		IDM_GAME,			";rÕh",
		IDM_GAME_NEW,		"wÆ;a ;rÕhla",
		IDM_GAME_SOUND,		"Yíoh",
		IDM_GAME_EXIT,		"bj;a fjkak",

		IDM_HELP,			"Woõ",
		IDM_HELP_LASTHAND,	"hg f.dv fmkajkak",
		IDM_HELP_HELP,		"Woõ ,smsh", // ^bx.qSis&",
		IDM_HELP_VHIST,		"b;sydih",
		IDM_HELP_ABOUT,		".ek",

};


HMENU DefineMenu()
{
	HMENU hMenuPopup;
	HMENU hMenu;

	hMenu = CreateMenu () ;

	// game
	hMenuPopup =  CreateMenu () ;
	AppendMenu (hMenuPopup, MFT_OWNERDRAW,    IDM_GAME_NEW,		MenuStr[IDM_GAME_NEW].szMenuStr);
	AppendMenu (hMenuPopup, MFT_OWNERDRAW,    IDM_GAME_SOUND,		MenuStr[IDM_GAME_SOUND].szMenuStr);
	AppendMenu (hMenuPopup, MFT_OWNERDRAW,    IDM_GAME_EXIT,	MenuStr[IDM_GAME_EXIT].szMenuStr);
	AppendMenu (hMenu, MF_POPUP | MFT_OWNERDRAW, (UINT) hMenuPopup, MenuStr[IDM_GAME].szMenuStr) ;

	// help
	hMenuPopup =  CreateMenu () ;
	AppendMenu (hMenuPopup, MFT_OWNERDRAW,    IDM_HELP_LASTHAND,	MenuStr[IDM_HELP_LASTHAND].szMenuStr);
	AppendMenu (hMenuPopup, MFT_OWNERDRAW,    IDM_HELP_HELP,		MenuStr[IDM_HELP_HELP].szMenuStr);
	AppendMenu (hMenuPopup, MFT_OWNERDRAW,    IDM_HELP_VHIST,		MenuStr[IDM_HELP_VHIST].szMenuStr);
	AppendMenu (hMenuPopup, MFT_OWNERDRAW,    IDM_HELP_ABOUT,		MenuStr[IDM_HELP_ABOUT].szMenuStr);
	AppendMenu (hMenu, MF_POPUP | MFT_OWNERDRAW, (UINT) hMenuPopup, MenuStr[IDM_HELP].szMenuStr) ;

	return hMenu;
}

void On_MeasureItem(HWND hwnd, LPMEASUREITEMSTRUCT lpmis)
{
	HDC hdc = GetDC(hwnd);
	SIZE size;

	SelectObject(hdc, hSinFontSm);
	GetTextExtentPoint32(hdc, (char *)lpmis->itemData,
		strlen((char *)lpmis->itemData), &size);

	lpmis->itemWidth = size.cx;
	lpmis->itemHeight = size.cy;

	lpmis->itemWidth += GetSystemMetrics(SM_CXMENUCHECK);
	ReleaseDC(hwnd, hdc);
}


void On_DrawItem(HWND hwnd, LPDRAWITEMSTRUCT lpdis)
{
	int x, y;
	int X,Y;
	HPEN hPen = CreatePen(PS_SOLID, 2, GetSysColor(COLOR_HIGHLIGHTTEXT));

	// Determine where to draw and leave space for a check mark.
	x = lpdis->rcItem.left;
	y = lpdis->rcItem.top;

	// Select the font and draw the text.
	SelectObject(lpdis->hDC, hSinFontSm);

	switch(lpdis->itemID)
	{
	case IDM_GAME_NEW:
	case IDM_GAME_EXIT:
	case IDM_HELP_ABOUT:
	case IDM_HELP_HELP:
	case IDM_HELP_LASTHAND:
	case IDM_GAME_SOUND:
	case IDM_HELP_VHIST:
		// Set the appropriate foreground and background colors.
		if (lpdis->itemState & ODS_SELECTED)
		{
			SetTextColor(lpdis->hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
			SetBkColor(lpdis->hDC, GetSysColor(COLOR_HIGHLIGHT));
			hPen = CreatePen(PS_SOLID, 2, GetSysColor(COLOR_HIGHLIGHTTEXT));
		}
		else
		{
			SetTextColor(lpdis->hDC, GetSysColor(COLOR_MENUTEXT));
			SetBkColor(lpdis->hDC, GetSysColor(COLOR_MENU));
			hPen = CreatePen(PS_SOLID, 2, GetSysColor(COLOR_MENUTEXT));
		}
		SelectObject(lpdis->hDC, hPen);

		x += GetSystemMetrics(SM_CXMENUCHECK);

        ExtTextOut(lpdis->hDC, x, y, ETO_OPAQUE,
			&lpdis->rcItem, (char *)lpdis->itemData,
			strlen((char *)lpdis->itemData), NULL);
		break;

		// menubar
	default:
		if (lpdis->itemState & ODS_SELECTED)
		{
			SetTextColor(lpdis->hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
			SetBkColor(lpdis->hDC, GetSysColor(COLOR_HIGHLIGHT));
		}
		else
		{
			SetTextColor(lpdis->hDC, GetSysColor(COLOR_MENUTEXT));
			SetBkColor(lpdis->hDC, GetSysColor(COLOR_MENU));
		}
		ExtTextOut(lpdis->hDC, x, y, ETO_CLIPPED | ETO_OPAQUE,
			&lpdis->rcItem, (char *)lpdis->itemData,
			strlen((char *)lpdis->itemData), NULL);
		break;
	}

	// check box in menu offset
	const int CM_XOFFSET = 3;
	const int CM_YOFFSET = 5;

	x = lpdis->rcItem.left+ CM_XOFFSET;
	y = lpdis->rcItem.top + CM_YOFFSET;
	X = lpdis->rcItem.left+ GetSystemMetrics(SM_CXMENUCHECK) - CM_XOFFSET;
	Y = lpdis->rcItem.bottom - CM_YOFFSET;

	if ((lpdis->itemID == IDM_GAME_SOUND) && bSndOn)
	{
		MoveToEx(lpdis->hDC, x, y, NULL);
		LineTo(lpdis->hDC, X, Y);
		MoveToEx(lpdis->hDC, x, Y, NULL);
		LineTo(lpdis->hDC, X, y);
	}

	DeleteObject(hPen);
	return;
}

