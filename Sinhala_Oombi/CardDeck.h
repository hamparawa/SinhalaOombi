#ifndef CARD_DECK_H
#define CARD_DECK_H

#include "resource.h"

#define GETCARDRES(card) (card + IDB_SPADE_7)

LRESULT CALLBACK WndProc_CardDeck (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc_CardTableDropped (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc_TrumpGetting(HWND, UINT, WPARAM, LPARAM);

HWND HitTest(HWND);
void DrawTextInMainWnd(HWND);


#endif // CARD_DECK_H
