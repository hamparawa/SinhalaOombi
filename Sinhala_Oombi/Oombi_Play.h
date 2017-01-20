#ifndef OOMBI_PLAY_H
#define OOMBI_PLAY_H

BYTE Play_NextCard(const BYTE);

BYTE Play_AlreadyStarted(const BYTE);

BYTE Play_GetLowRiskHighest(const BYTE, bool);
BYTE Play_GetOponentChanceCard(const BYTE);
BYTE Play_NormalHighNoTrumps(const BYTE);
BYTE Play_NormalHighInclTrumps(const BYTE);
BYTE Play_ReleaseSingleCard(const BYTE);
BYTE Play_NormalLowNoTrumps(const BYTE);
BYTE IsThisCardWinnable(const BYTE, const BYTE);
void SetCountCards(const BYTE, const BYTE);


#endif	// OOMBI_PLAY_H
