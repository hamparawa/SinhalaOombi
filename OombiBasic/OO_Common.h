#ifndef COMMON_H
#define _COMMON_H

#include "Deck_Func.h"
#include "Oombi_play.h"

extern BYTE cardDeck[32];
extern BYTE trumps;
extern BYTE downCards[32];
extern BYTE playaCards[4][8];
extern BYTE playaFlags[4];
extern BYTE playaCardPosVal[4][8];
extern BYTE cardTable[4];
extern BYTE startPlayer;

// card const
#define	NO_CARD		100 // return val, means cannot find a card   
#define BLANK_CARD	101	// used in playaCards, means no card is there

// down card const
#define CARD_UP		0
#define CARD_DOWN	1

// player const
#define NO_PLAYER	100

enum OOMBI_DECK
{
	SPADE_7,	// 0
	SPADE_8,
	SPADE_9,
	SPADE_10,
	SPADE_J,
	SPADE_Q,
	SPADE_K,
	SPADE_A,

	HEART_7,	// 8
	HEART_8,
	HEART_9,
	HEART_10,
	HEART_J,
	HEART_Q,
	HEART_K,
	HEART_A,

	CLUBS_7,	// 16	
	CLUBS_8,
	CLUBS_9,
	CLUBS_10,
	CLUBS_J,
	CLUBS_Q,
	CLUBS_K,
	CLUBS_A,

	DIAMO_7,	// 24
	DIAMO_8,
	DIAMO_9,
	DIAMO_10,
	DIAMO_J,
	DIAMO_Q,
	DIAMO_K,
	DIAMO_A		// 31
};


enum OOMBI_CARDTYPE
{
	OO_SPADE,
	OO_HEART,
	OO_CLUBS,
	OO_DIAMO
};

enum OOMBI_CARDVAL
{
	OO_RANK_1,
	OO_RANK_2,
	OO_RANK_3,
	OO_RANK_4,
	OO_RANK_5,
	OO_RANK_6,
	OO_RANK_7,
	OO_RANK_8
};

enum OOMBI_PLAYER
{
	PLAYER_1,
	PLAYER_2,
	PLAYER_3,
	PLAYER_4
};

// Player Flags
#define NO_SPADE	0x01
#define NO_HEART	0x02
#define NO_CLUBS	0x04
#define NO_DIAMO	0x08

#endif // _COMMON_H
