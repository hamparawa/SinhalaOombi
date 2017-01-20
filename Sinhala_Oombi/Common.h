#ifndef COMMON_H
#define COMMON_H

//#ifndef _DEBUG
#pragma warning( disable : 4312 4267 4311 4244 )
//#endif

extern BYTE cardDeck[32];
extern BYTE trumps;
extern BYTE downCards[32];
extern BYTE playaCards[4][8];
extern BYTE playaFlags[4][4];
extern BYTE playaCardPosVal[4][8];
extern BYTE cardTable[4];
extern BYTE startPlayer;
extern BYTE gameStartPlayer;
extern BYTE animationCards[4];
extern BYTE lastHand[4];
extern BYTE lastWinner;
extern BYTE cardCount[4];

extern BYTE bCardPlayable;

extern BYTE winSets;
extern BYTE lostSets;
extern BYTE winPoints;
extern BYTE lostPoints;
extern BYTE rounds;

extern BYTE bIsLastSeporu;
extern BYTE bTrumpsNotYet;

extern BYTE bCheatMode;
extern BYTE bCheatApplied;

extern HFONT hSinFontSm;
extern HFONT hSinFontMe;
extern HFONT hSinFontL;

extern BYTE bSndOn;
extern HANDLE hDealSound;
extern HANDLE hSelectSnd;
extern HANDLE hCutSnd;

extern HBITMAP *phBitmap[32];
extern HBITMAP *phCoverBitmap;

extern int WIN_X;//	700	
extern int WIN_Y;//	550

extern int STATUS_R_X;//	(WIN_X - WIN_Y)
extern int STATUS_R_Y;//	WIN_Y

#define tcWIN_X 710
#define tcWIN_Y 576
#define STATUS_WIDTH (cWIN_X - cWIN_Y)

extern int cWIN_X;
extern int cWIN_Y;

extern int CARD_X;	//71
extern int CARD_Y;	//96

extern int CARD_GAP; //	(CARD_X / 5)

#define CARD_OFFSET_X	10
#define CARD_OFFSET_Y	10

// Player cards positions
#define PLAYA1_CARD_X(y)	((y - (CARD_X + (7 * CARD_GAP))) / 2)
#define PLAYA1_CARD_Y(y)	(y - CARD_Y - CARD_OFFSET_Y)	

#define PLAYA2_CARD_X(y)	(y - CARD_X - CARD_OFFSET_X)
#define PLAYA2_CARD_Y(y)	((y - (CARD_Y - (CARD_GAP * 7))) / 2)

#define PLAYA3_CARD_X(y)	((y - (CARD_X - (7 * CARD_GAP))) / 2)
#define PLAYA3_CARD_Y		CARD_OFFSET_Y

#define PLAYA4_CARD_X		CARD_OFFSET_X
#define PLAYA4_CARD_Y(y)	((y - (CARD_Y + (CARD_GAP * 7))) / 2)

// Card Table Cards Positions
#define CT_P1_X(y)		((y - CARD_X) / 2) + CARD_GAP	
#define CT_P1_Y(y)		(y / 2)	

#define CT_P2_X(y)		(y / 2)	
#define CT_P2_Y(y)		((y - CARD_Y) / 2) - CARD_GAP

#define CT_P3_X(y)		((y - CARD_X) / 2) - CARD_GAP	
#define CT_P3_Y(y)		((y / 2) - CARD_Y)	

#define CT_P4_X(y)		((y / 2) - CARD_X)
#define CT_P4_Y(y)		((y - CARD_Y) / 2) + CARD_GAP

// Card table card identifiers
#define CT_PLAYER_1		32	
#define CT_PLAYER_2		33	
#define CT_PLAYER_3		34
#define CT_PLAYER_4		35

#define RHS_STATUSID	80

// last hand cards
#define LAST_HAND1		82
#define LAST_HAND2		83
#define LAST_HAND3		84
#define LAST_HAND4		85

// animation card id
#define ANICARD_P1		90
#define ANICARD_P2		91
#define ANICARD_P3		92
#define ANICARD_P4		93

// trumpgetting Cards
#define TRMP_GET_SPADE	95
#define TRMP_GET_HEART	96
#define TRMP_GET_CLUBS	97
#define TRMP_GET_DIAMO	98

// card const
#define	NO_CARD		100 // return val, means cannot find a card   
#define BLANK_CARD	101	// used in playaCards, means no card is there

// down card const
#define CARD_UP		0
#define CARD_DOWN	1

// player const
#define NO_PLAYER	100

// timer const
#define TIMER_CLEAR			1
#define TIMER_PLAY			2
#define TIMER_PLAY_START	3
#define TIMER_WAITTILL_TRUMPS	4

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

#include "Deck_Func.h"
#include "Oombi_play.h"
#include "Win_Msgs.h"
#include "CardDeck.h"
#include "Play_win.h"
#include "Rhs_Statusbar.h"
#include "FontMem.h"
#include "Animation.h"
#include "Msgbox.h"
#include "Menu.h"
#include "About.h"
#include "LastHand.h"

#endif // _COMMON_H