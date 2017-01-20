#ifndef DECK_FUNC_H
#define DECK_FUNC_H

// macros
#define GetCardType(Card) (Card / 8)
#define GetCardVal(Card) (Card % 8)
#define GetCardFromValType(Val, Type) ((Type * 8) + Val)
#define GetPartner(player) ((player + 2) % 4)
#define GetEnmy1(player) ((player + 1) % 4)
#define GetEnmy2(player) ((player + 3) % 4)
#define NextPlayer(player) ((player + 1) % 4)

void ShuffleDeck();
BYTE ReturnTrumps(const BYTE);
inline void EvaluateCard(const BYTE &, BYTE *);
void InitOomibiDeck();
BYTE GetHighestCard(const BYTE, const BYTE);
BYTE GetLowestCard(const BYTE, const BYTE);
BYTE GetNextHighestCard(const BYTE, const BYTE);
void SetCardPosVals(void);
void SetOutCardType(const BYTE, const BYTE, const BYTE);
BYTE IsCardsOut(const BYTE, const BYTE, const BYTE);
BYTE GetCurrentWinner(void);
void EmptyTable(void);
bool CanSayTrumps(const BYTE);


#endif // DECK_FUNC_H

