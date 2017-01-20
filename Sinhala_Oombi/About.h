#ifndef ABOUT_H
#define ABOUT_H

#define TIMER_FADEIN	1
#define TIMER_FADEOUT	2

#define MTP_COLORKEY			1
#define MTP_ALPHA				2
#define MTP_WS_EX_LAYERED		0x80000L
typedef BOOL (WINAPI *TRANSPROC)(HWND, COLORREF, BYTE, DWORD);

int ShowAbout(HWND);
LONG WINAPI WndProcAbout (HWND, UINT, WPARAM, LPARAM);
void Trans(HWND, UINT);
void CopyAndShowReadme(HWND);
void CopyAndShowVerHist(HWND hwnd);

#endif	//ABOUT_H