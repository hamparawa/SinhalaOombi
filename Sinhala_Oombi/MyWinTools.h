/*	Tools for Windows...?
	by HAMA			
	2008.03.05 @ 4.36 PM

*/
#ifndef MYWINTOOLS
#define MYWINTOOLS

#include <windows.h>
inline void __stdcall PosToMiddle(HWND hwnd)
{
	RECT rc, rcDlg, rcOwner; 

    GetWindowRect(GetDesktopWindow(), &rcOwner); 
    GetWindowRect(hwnd, &rcDlg); 
    CopyRect(&rc, &rcOwner); 

     // Offset the owner and dialog box rectangles so that 
     // right and bottom values represent the width and 
     // height, and then offset the owner again to discard 
     // space taken up by the dialog box. 

    OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top); 
    OffsetRect(&rc, -rc.left, -rc.top); 
    OffsetRect(&rc, -rcDlg.right, -rcDlg.bottom); 

     // The new position is the sum of half the remaining 
     // space and the owner's original position. 

    SetWindowPos(hwnd, 
        HWND_TOP, 
        rcOwner.left + (rc.right / 2), 
        rcOwner.top + (rc.bottom / 2), 
        0, 0,          // ignores size arguments 
        SWP_NOSIZE); 

	return;
}

#endif	//MYWINTOOLS