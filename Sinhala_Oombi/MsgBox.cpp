#include <windows.h>
#include "common.h"
#include "Msgbox.h"
#include "MyWinTools.h"

long hPrevWndProc;

int ShowMsgBox (OO_MSGBOXDATA *mbData)
{	
	WNDCLASS wc;
	HWND hwnd;
	MSG msg;

	wc.style = 0;                                   // Class style
	wc.lpfnWndProc = (WNDPROC) WndProcMsg;		// window procedure address
	wc.cbClsExtra = 0;						// class extra bytes
	wc.cbWndExtra = 0;						// window extra bytes
	wc.hInstance  = (HINSTANCE) GetWindowLong(mbData->hwndParent, GWL_HINSTANCE);				// instance handler
	wc.hIcon = LoadIcon(NULL, IDI_ASTERISK); // icon handler
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	// cursor handle
	wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);	// background color
	wc.lpszMenuName = NULL;		// menu name
	wc.lpszClassName = "SinOMsgBoxWndClass";		// WNDCLASS name

	RegisterClass (&wc);

	hwnd = CreateWindowEx(
		WS_EX_TOOLWINDOW,
		"SinOMsgBoxWndClass",			// WNDCLASS name
		"",		// Window title
		WS_OVERLAPPED | WS_SYSMENU,	// Window Style
		CW_USEDEFAULT,			// Horizontal position
		CW_USEDEFAULT,			// Vertical position
		CW_USEDEFAULT,			// Initial Width
		CW_USEDEFAULT,			// Initial Height
		mbData->hwndParent,			// Handle of parent window
		NULL,					// Menu Handle
		(HINSTANCE) GetWindowLong(mbData->hwndParent, GWL_HINSTANCE),				// Application Instance handle
		(void *)mbData					// Window creation data
		);
	
	PosToMiddle(hwnd);
	EnableWindow(mbData->hwndParent, false);
	ShowWindow (hwnd, SW_SHOW);
	UpdateWindow (hwnd);

	while  ( GetMessage (&msg, NULL, 0, 0) )
	{
		TranslateMessage (&msg);
		DispatchMessage(&msg);
	}

	EnableWindow(mbData->hwndParent, true);
	SetFocus(mbData->hwndParent);

	return (int) msg.wParam;
}



LRESULT CALLBACK WndProcMsg (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static SIZE size;
	static int ixIcon, iyIcon;
	static int iWidth, iHeight;
	static HWND hwndButton1, hwndButton2;

	static OO_MSGBOXDATA *mbData;
	static int retVal = 0;

	switch (message)
	{
	case WM_CREATE:
		mbData = (OO_MSGBOXDATA *) ((CREATESTRUCT *) lParam)->lpCreateParams;
		hdc = GetDC(hwnd);
		SelectObject(hdc, hSinFontMe);
		GetTextExtentPoint32(hdc, mbData->MsgStr, strlen(mbData->MsgStr), &size);
		ReleaseDC(hwnd, hdc);

		ixIcon = GetSystemMetrics(SM_CXICON);
		iyIcon = GetSystemMetrics(SM_CYICON);

		iWidth  = MSG_SPCITEMSX + ixIcon + MSG_SPCITEMSX + size.cx + MSG_SPCITEMSX ;
		iHeight = MSG_SPCITEMSY + iyIcon + MSG_SPCITEMSY + MSG_OK_Y + MSG_SPCITEMSY;

		iWidth  = max(iWidth, (MSG_SPCITEMSX + ixIcon + MSG_OK_X + MSG_SPCITEMSX));
		iHeight = max(iHeight,(MSG_SPCITEMSY + iyIcon + MSG_SPCITEMSY +  MSG_SPCITEMSY));

		SetWindowPos(hwnd, HWND_TOPMOST, 250, 250, iWidth, iHeight, 0);

		if (mbData->msgBoxType & MB_ONEBUTTON)
		{
			hwndButton1 = CreateWindow("BUTTON",
				mbData->butText1,
				WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
				(iWidth - MSG_OK_X) / 2,
				MSG_SPCITEMSY + iyIcon + 5,
				MSG_OK_X,
				MSG_OK_Y,
				hwnd,
				(HMENU) BUT_ONE,
				(HINSTANCE) GetWindowLong(mbData->hwndParent, GWL_HINSTANCE),
				NULL);
		}
		else
		{
			if (mbData->msgBoxType & MB_TWOBUTTON)
			{
				hwndButton1 = CreateWindow("BUTTON",
					mbData->butText1,
					WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
					((iWidth - MSG_OK_X) / 2) - (MSG_OK_X/ 2) - 5,
					MSG_SPCITEMSY + iyIcon + 5,
					MSG_OK_X,
					MSG_OK_Y,
					hwnd,
					(HMENU) BUT_ONE,
					(HINSTANCE) GetWindowLong(mbData->hwndParent, GWL_HINSTANCE),
					NULL);

				hwndButton2 = CreateWindow("BUTTON",
					mbData->butText2,
					WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
					((iWidth - MSG_OK_X) / 2) + (MSG_OK_X/2) + 5,
					MSG_SPCITEMSY + iyIcon + 5,
					MSG_OK_X,
					MSG_OK_Y,
					hwnd,
					(HMENU) BUT_TWO,
					(HINSTANCE) GetWindowLong(mbData->hwndParent, GWL_HINSTANCE),
					NULL);

			}
			else
				MessageBox(0,"Error!","", 0);
		}

		//SetSinhalaFont to button
		SendMessage(hwndButton1, WM_SETFONT, (WPARAM)hSinFontSm, MAKELPARAM(true, 0)); 
		SendMessage(hwndButton2, WM_SETFONT, (WPARAM)hSinFontSm, MAKELPARAM(true, 0)); 
		
		MessageBeep(MB_ICONEXCLAMATION);
		SetFocus(hwnd);
		return 0;


	case WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, GetSysColor(COLOR_BTNTEXT));
		SelectObject(hdc, hSinFontSm);

		if (mbData->msgBoxType & MBIT_EXCLAMATION)
			DrawIcon(hdc, MSG_SPCITEMSX, MSG_SPCITEMSY, LoadIcon(NULL, IDI_EXCLAMATION));
		else
		{
			if (mbData->msgBoxType & MBIT_QUESTION)
				DrawIcon(hdc, MSG_SPCITEMSX, MSG_SPCITEMSY, LoadIcon(NULL, IDI_QUESTION));
			else
				if (mbData->msgBoxType & MBIT_INFORMATION)
					DrawIcon(hdc, MSG_SPCITEMSX, MSG_SPCITEMSY, LoadIcon(NULL, IDI_INFORMATION));
		}
	
		TextOut(hdc,
			MSG_SPCITEMSX + ixIcon + MSG_SPCITEMSX, 
			MSG_SPCITEMSY, 
			mbData->MsgStr,
			strlen(mbData->MsgStr));

		EndPaint (hwnd, &ps);
		return 0;


	case WM_NCPAINT:
	case WM_NCACTIVATE:
		DefWindowProc(hwnd, message, wParam, lParam);
		hdc = GetWindowDC(hwnd);
		SetTextColor(hdc, GetSysColor(COLOR_CAPTIONTEXT));
		SetBkMode(hdc, TRANSPARENT);
		SelectObject(hdc, hSinFontSm);
		TextOut(hdc, 5,1, mbData->Title, strlen(mbData->Title));
		ReleaseDC(hwnd, hdc);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case BUT_ONE:
			retVal = 0;
			break;

		case BUT_TWO:
			retVal = 1;
			break;
		}
		DestroyWindow(hwnd);
		return 0; 

	case WM_KEYUP:
		switch(wParam)
		{
		case VK_ESCAPE:
			if (mbData->msgBoxType & MB_TWOBUTTON)
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(BUT_TWO, 0), 0);
			break;

		case VK_RETURN:
			SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(BUT_ONE, 0), 0);
			break;
		}
		return 0;

	case WM_DESTROY: 
		PostQuitMessage(retVal); 
		return 0;
	}


	return DefWindowProc (hwnd, message, wParam, lParam);
}