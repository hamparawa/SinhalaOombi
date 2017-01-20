
#ifndef RHS_STATUSBAR_H
#define RHS_STATUSBAR_H

LRESULT CALLBACK WndProc_Rhs_StatusBar(HWND, UINT, WPARAM,LPARAM);
void PrintTrumpsBox(HWND, HDC, int, int);
void PrintWinnedSetes(HDC, int, int);
void PrintPoints(HDC,int, int);
void DrawNIcons(HWND, HDC, int, int);

inline char *CFB(BYTE, char *);

#endif // RHS_STATUSBAR_H
