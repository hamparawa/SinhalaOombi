// to use AddFontMemResourceEx in VC++ 6
// hamparawa@gmail.com

#ifndef FONTMEM_H
#define FONTMEM_H

#include <windows.h>

typedef int (__stdcall *FONTPROC)(PVOID, DWORD, PVOID, DWORD *);

inline int __stdcall AddFontMemResourceEx(PVOID pbFont, DWORD cbFont, PVOID pdv, DWORD *pcFonts )
{
	FONTPROC pTrans;
	HINSTANCE hInstLib;
	BOOL retVal = true;

	hInstLib = LoadLibrary("gdi32.dll");
	if (hInstLib)
	{
		pTrans = (FONTPROC) (GetProcAddress(hInstLib, "AddFontMemResourceEx"));
		if (pTrans)
		{
			retVal = (pTrans)(pbFont, cbFont, pdv, pcFonts);
		}
		FreeLibrary(hInstLib);
	}
	else
		return false;
	return retVal;
}
#endif	//FONTMEM_H
