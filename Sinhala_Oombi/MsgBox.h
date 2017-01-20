#ifndef MSGBOX_H
#define MSGBOX_H

struct OO_MSGBOXDATA
{
	HWND hwndParent;
	char *MsgStr;
	char *Title;
	BYTE msgBoxType;
	char *butText1;
	char *butText2;
};

#define MSG_SPCITEMSX	20
#define MSG_SPCITEMSY	20

#define MSG_OK_X		80
#define MSG_OK_Y		30

#define BUT_ONE			0
#define BUT_TWO			1

// mb flags
#define MB_ONEBUTTON		0x01
#define MB_TWOBUTTON		0x02

#define	MBIT_INFORMATION	0x04
#define MBIT_QUESTION		0x08
#define MBIT_EXCLAMATION	0x10

int ShowMsgBox (OO_MSGBOXDATA *);
LRESULT CALLBACK WndProcMsg (HWND, UINT, WPARAM, LPARAM);

#endif	// MSGBOX_H
