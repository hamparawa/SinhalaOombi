#ifndef PLAY_WIN_H
#define PLAY_WIN_H

void Player_PlayCard(HWND);
void Update_CardTable(HWND);
void Play_OtherThree(HWND);
BYTE SetWinnerVars(HWND);
void Play_UntillFinish(HWND);
bool GivePoints(HWND);
void PlayNextRound(HWND);
void GetTrumpsFromUser(HWND);
void DisplayTrumpsMsg(BYTE, HWND);
void AskAndGiveNewGame(HWND, bool);
void ShowSummary(HWND);
void SortPlayaCards(void);
void SortCardsb4Trumps(void);


#endif	// PLAY_WIN_H
