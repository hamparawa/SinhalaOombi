#include <windows.h>
#include "common.h"
#include "MyWinTools.h"

int ShowAbout(HWND hwndParent)
{
    WNDCLASS wc;
    MSG msg;
	HWND hwnd;
    
	wc.style = 0;                                   // Class style
	wc.lpfnWndProc = (WNDPROC) WndProcAbout;		// window procedure address
	wc.cbClsExtra = 0;						// class extra bytes
	wc.cbWndExtra = 0;						// window extra bytes
	wc.hInstance  = (HINSTANCE) GetWindowLong(hwndParent, GWL_HINSTANCE);				// instance handler
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); // icon handler
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	// cursor handle
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);	// background color
	wc.lpszMenuName = NULL;		// menu name
	wc.lpszClassName = "SOWndClassAbout";		// WNDCLASS name
	
	RegisterClass (&wc);
	
	hwnd = CreateWindowEx(
		WS_EX_TOOLWINDOW,
		"SOWndClassAbout",			// WNDCLASS name
		"",		// Window title
		WS_POPUP,	// Window Style
		CW_USEDEFAULT,			// Horizontal position
		CW_USEDEFAULT,			// Vertical position
		300,			// Initial Width
		200,			// Initial Height
		hwndParent,			// Handle of parent window
		NULL,					// Menu Handle
		(HINSTANCE) GetWindowLong(hwndParent, GWL_HINSTANCE),// Application Instance handle
		NULL					// Window creation data
		);
	
	EnableWindow(hwndParent, false);
	ShowWindow (hwnd, SW_SHOW);
	UpdateWindow (hwnd);
	
	while  ( GetMessage (&msg, NULL, 0, 0) )
	{
		TranslateMessage (&msg);
		DispatchMessage(&msg);
	}
	
	EnableWindow(hwndParent, true);
	SetFocus(hwndParent);
	return msg.wParam;
}

LRESULT CALLBACK WndProcAbout(HWND hwnd, UINT message, WPARAM wParam,
							  LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static unsigned int i;
	static BOOL bOpening;
	HBRUSH hBrush;
	RECT rect;
	char Text1[] = "Tnf.a woyia yd fhdackd okajd tjkak'";
	char Text2[] = "hamparawa@gmail.com";
	static HFONT hEngFont;
	
	switch (message)
	{
	case WM_CREATE:
		SetWindowPos(hwnd, HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
		PosToMiddle(hwnd);
		i = 0;
		bOpening = true;
		
		hEngFont = CreateFont(22, 11, 0, 0, 300, 0, 0, 0, ANSI_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
			DEFAULT_QUALITY, FF_DONTCARE, "Comic Sans MS");
		
		SetTimer(hwnd, TIMER_FADEIN, 25, NULL); 
		SendMessage(hwnd, WM_TIMER, TIMER_FADEIN, 0);
		
		return 0;
		
	case WM_LBUTTONDOWN:
		if (!bOpening)
			SetTimer(hwnd, TIMER_FADEOUT, 25, NULL);
		return 0;
		
	case WM_KILLFOCUS:
	case WM_SETFOCUS:
		SetWindowPos(hwnd, HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
		return 0;
		
		
	case WM_PAINT:
		
		hBrush= CreateSolidBrush(RGB(0,0,255));
		GetClientRect(hwnd, &rect);
		hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &rect, hBrush);
		DeleteObject(hBrush); // blu one
		
		hBrush = CreateSolidBrush(RGB(0,0,0));
		SelectObject(hdc, hBrush);
		RoundRect(hdc, 2,2, 300,200,50,50);
		DeleteObject(hBrush); // black one
		
		hBrush = CreateSolidBrush(RGB(255,255,255));
		SelectObject(hdc, hBrush);
		RoundRect(hdc, 0,0, 298, 198, 50,50);
		DeleteObject(hBrush);  // white
		
		SelectObject(hdc, hSinFontMe);
		SetBkMode(hdc, TRANSPARENT);
		
		SetTextColor(hdc, RGB(0,0,0));
		SetTextAlign(hdc, TA_CENTER);
		TextOut(hdc, 151,51, Text1, strlen(Text1));
		
		SetTextColor(hdc, RGB(14,162,12));
		SetTextAlign(hdc, TA_CENTER);
		TextOut(hdc, 150,50, Text1, strlen(Text1));
		
		SetTextColor(hdc, RGB(128,0,255));
		SelectObject(hdc, hEngFont);
		TextOut(hdc, 150, 100, Text2, strlen(Text2));
		
		EndPaint (hwnd, &ps);
		return 0;
		
	case WM_TIMER:
		switch (wParam)
		{
		case TIMER_FADEIN:
			if (i < 220)
			{
				Trans(hwnd, i);
				i += 5;
			}
			else
			{
				bOpening = false;
				KillTimer(hwnd, TIMER_FADEIN);
			}
			break;
			
		case TIMER_FADEOUT:
			if (i > 5)
			{
				Trans(hwnd, i);
				i -= 5;
			}
			else
			{
				KillTimer(hwnd, TIMER_FADEOUT);
				SendMessage(hwnd, WM_CLOSE, 0, 0);
			}
			break;
		}
		return 0;
		
		case WM_DESTROY:
			DeleteObject(hEngFont);
			PostQuitMessage(0);
			return 0;
	}
	
	
	return DefWindowProc (hwnd, message, wParam, lParam);
}


void Trans(HWND hwnd, UINT i)
{
	TRANSPROC pTrans;
	HINSTANCE hInstLib;
	BOOL retVal = true;
	
	hInstLib = LoadLibrary("user32.dll");
	if (hInstLib)
	{
		pTrans = (TRANSPROC) (GetProcAddress(hInstLib, "SetLayeredWindowAttributes"));
		if (pTrans)
		{			
			SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | MTP_WS_EX_LAYERED);
			retVal = (pTrans)(hwnd, RGB(0,0,255), i, MTP_COLORKEY | MTP_ALPHA);
		}
		FreeLibrary(hInstLib);
	}
	
	return ;
}


void CopyAndShowReadme(HWND hwnd)
{
	HINSTANCE hInstance;
	char *pReadMeStr;
	DWORD fileSize;
	DWORD dwBtoWrite;
	HANDLE hTempFile;
	char lpPathBuffer[256];
	char szTempName[256];
	char sOperation[] = "edit";

	hInstance = (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE);
	pReadMeStr = (char *) LockResource(LoadResource(hInstance, 
			FindResource(hInstance, MAKEINTRESOURCE(IDR_README), "TXT")));
	fileSize =  SizeofResource(hInstance, 
		FindResource(hInstance, MAKEINTRESOURCE(IDR_README), "TXT"));

	GetTempPath(256, lpPathBuffer);

	strcpy(szTempName, lpPathBuffer);
	strcat(szTempName, "Oombi_Sinhala_Readme.txt");

    hTempFile = CreateFile((LPTSTR) szTempName,
		GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hTempFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(hwnd, "Error, Unable to show Readme", "Sinhala Oombi", MB_ICONERROR);
		return;
	}

	WriteFile(hTempFile, (void *) pReadMeStr, fileSize, &dwBtoWrite, 0);
	CloseHandle(hTempFile);

	ShellExecute(hwnd, sOperation, szTempName, 0, lpPathBuffer, SW_SHOW);

	return;
}

void CopyAndShowVerHist(HWND hwnd)
{
	HINSTANCE hInstance;
	char *pReadMeStr;
	DWORD fileSize;
	DWORD dwBtoWrite;
	HANDLE hTempFile;
	char lpPathBuffer[256];
	char szTempName[256];
	char sOperation[] = "edit";

	hInstance = (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE);
	pReadMeStr = (char *) LockResource(LoadResource(hInstance, 
			FindResource(hInstance, MAKEINTRESOURCE(IDR_VERHISTORY), "TXT")));
	fileSize =  SizeofResource(hInstance, 
		FindResource(hInstance, MAKEINTRESOURCE(IDR_VERHISTORY), "TXT"));

	GetTempPath(256, lpPathBuffer);

	strcpy(szTempName, lpPathBuffer);
	strcat(szTempName, "Oombi_Sinhala_Version_History.txt");

    hTempFile = CreateFile((LPTSTR) szTempName,
		GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hTempFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(hwnd, "Error, Unable to show Version History", "Sinhala Oombi", MB_ICONERROR);
		return;
	}

	WriteFile(hTempFile, (void *) pReadMeStr, fileSize, &dwBtoWrite, 0);
	CloseHandle(hTempFile);

	ShellExecute(hwnd, sOperation, szTempName, 0, lpPathBuffer, SW_SHOW);

	return;
}