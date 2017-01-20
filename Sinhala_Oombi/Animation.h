#ifndef ANIMATION_H
#define ANIMATION_H

LRESULT CALLBACK WndProc_AnimationCards(HWND, UINT, WPARAM, LPARAM);
void ShowEndAnimation(HWND);
void ShowPlayAnimation(HWND, BYTE, BYTE);
void CalWndpos(POINT *,const BYTE &);
void PlayCantSound(HWND);


#endif  // ANIMATION_H
