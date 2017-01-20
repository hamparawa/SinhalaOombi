#ifndef WIN_MSGS_H
#define WIN_MSGS_H

void On_Create(HWND, WPARAM, LPARAM);
void On_Size(HWND, WPARAM, LPARAM);
void On_Timer(HWND, WPARAM, LPARAM);
void On_Destroy(HWND, WPARAM, LPARAM);
void On_Paint(HWND, HDC);
void On_Close(HWND, WPARAM, LPARAM);
void On_Command(HWND, WPARAM, LPARAM);
void On_Sizing(HWND, WPARAM, LPARAM);
void On_KeyUp(HWND, WPARAM, LPARAM);

#endif // WIN_MSGS_H
