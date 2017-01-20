//#include <windows.h>
#include <algorithm>
#include <functional>
#include <vector>
#include <time.h>

#include <assert.h>
#include <iostream>

using namespace std;
#include "OO_Common.h"


void ShuffleDeck()
{
	int i;
	const int VECTOR_SIZE = 32 ;

	// Define a template class vector of BYTE
	typedef vector<BYTE> ByteVector ;

	//Define an iterator for template class vector of strings
	typedef ByteVector::iterator ByteVectorIt ;

	ByteVector Numbers(VECTOR_SIZE) ;
	ByteVectorIt start, end, it ;

	// Initialize vector Numbers
	for ( i =0; i < 32; i++)
		Numbers[i] = cardDeck[i];

	// location of first element of Numbers
	start = Numbers.begin();

	// one past the location last element of Numbers
	end = Numbers.end();

	// shuffle the elements in a random order
	random_shuffle(start, end) ;

	i = 0;
	for (it = start; it != end; it++)
		cardDeck[i++] = *it;

	return;
}

BYTE ReturnTrumps(const BYTE player)
{
	BYTE tempVal;
	BYTE i;
	BYTE cardValues[4] = { 0, 0, 0, 0 };

	EvaluateCard(playaCards[player][0], cardValues);
	EvaluateCard(playaCards[player][1], cardValues);
	EvaluateCard(playaCards[player][2], cardValues);
	EvaluateCard(playaCards[player][3], cardValues);

	tempVal = cardValues[0];
	for (i = 0; i < 4; i++)
		tempVal = max(tempVal, cardValues[i]);

	i = 0;
	while (tempVal != cardValues[i++]);
	return --i;
}

inline void EvaluateCard(const BYTE &Card, BYTE *CardVal)
{
	switch (GetCardType(Card))
	{
	case OO_SPADE:
		CardVal[OO_SPADE] += GetCardVal(Card);
		break;
	case OO_HEART:
		CardVal[OO_HEART] += GetCardVal(Card);
		break;
	case OO_CLUBS:
		CardVal[OO_CLUBS] += GetCardVal(Card);
		break;
	case OO_DIAMO:
		CardVal[OO_DIAMO] += GetCardVal(Card);
		break;
	}
	return;
}

void InitOomibiDeck()
{
	BYTE i, j;

	// Initialize random number generator
	srand((unsigned int) time(NULL));

	// initialize the deck full of cards
	for (i = 0; i < 32; i++)
	{
		cardDeck[i] = i;
		downCards[i] = CARD_UP;
	}
	for(i = 0; i < 4; i++)
	{
		playaFlags[i] = 0;
		cardTable[i] = NO_CARD;
	}

	ShuffleDeck();

	for (i = 0; i < 4; i++)
		for (j = 0; j < 8; j++)
			playaCards[i][j] = cardDeck[(i * 8) + j];

	return;
}


// returns the highest card in hand for the given type
BYTE GetHighestCard(const BYTE player, const BYTE cardType)
{
	BYTE i;
	BYTE highCardVal = 0;
	BYTE highCard;
	BYTE card;
	BYTE bCardFound = false;

	for ( i = 0; i < 8; i++ )
	{
		card = playaCards[player][i];
		if (card != BLANK_CARD)
			if (GetCardType(card) == cardType)
			{
				bCardFound = true;
				highCardVal = max(GetCardVal(card), highCardVal);
			}
	}
	if (bCardFound)
		highCard = GetCardFromValType(highCardVal, cardType);
	else
		highCard = NO_CARD;

	return highCard;
}

// returns the lowest card in hand for given type
BYTE GetLowestCard(const BYTE player, const BYTE cardType)
{
	BYTE i;
	BYTE lowestCardVal = NO_CARD;
	BYTE card;

	for ( i = 0; i < 8; i++)
	{
		card = playaCards[player][i];
		if (card != BLANK_CARD)
			if (GetCardType(card) == cardType)
				lowestCardVal = min(GetCardVal(card), lowestCardVal);
	}

	if (lowestCardVal != NO_CARD)
		return (GetCardFromValType(lowestCardVal, cardType));
	return lowestCardVal;
}

// gets the next biggest card in hand after the given
BYTE GetNextHighestCard(const BYTE player, const BYTE prevCard)
{
	BYTE i;
	const BYTE sCardType = GetCardType(prevCard);
	const BYTE sCardVal  = GetCardVal(prevCard);
	BYTE card;
	BYTE highCardVal = NO_CARD;
	BYTE highCard;

	for (i = 0; i < 8; i++)
	{
		card = playaCards[player][i];
		if ((card != BLANK_CARD) && (GetCardType(card) == sCardType))
			if (sCardVal < GetCardVal(card))
				highCardVal = min(GetCardVal(card), highCardVal);
	}

	if (highCardVal == NO_CARD)
		highCard = NO_CARD;
	else
		highCard = GetCardFromValType(highCardVal, sCardType);

	return highCard;
}

void SetCardPosVals(void)
{
	BYTE i,j,k;
	BYTE cpVal;
	BYTE cardVal;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 8; j++)
		{
			if (BLANK_CARD != playaCards[i][j])
			{
				cpVal = 7;
				cardVal = GetCardVal(playaCards[i][j]);
				for (k = cardVal + 1; k < 8; k++)
					if (downCards[GetCardFromValType(k,GetCardType(playaCards[i][j]))] != CARD_DOWN)
						--cpVal;

			}
			else
				cpVal = NO_CARD;

			playaCardPosVal[i][j] = cpVal;
		}
		return;
}

// Set flag for out card type
void SetOutCardType(const BYTE player, const BYTE cType)
{
	BYTE flag;
	switch (cType)
	{
	case OO_SPADE:
		flag = NO_SPADE;
		break;

	case OO_HEART:
		flag = NO_HEART;
		break;

	case OO_CLUBS:
		flag = NO_CLUBS;
		break;

	case OO_DIAMO:
		flag = NO_DIAMO;
		break;

	default:
		cout << "err: invalid 'cType' for SetOutCardType : " << (int)cType << endl;
		assert(0);	// invalid value

	}

	playaFlags[player] |= flag;

	return;
}

// IS card Out?
BYTE IsCardsOut(const BYTE player, const BYTE cType)
{
	BYTE flag = 0;

	switch (cType)
	{
	case OO_SPADE:
		flag = NO_SPADE;
		break;

	case OO_HEART:
		flag = NO_HEART;
		break;

	case OO_CLUBS:
		flag = NO_CLUBS;
		break;

	case OO_DIAMO:
		flag = NO_DIAMO;
		break;

	default:
		cout << "err: invalid 'cType' for IsCardsOut: " << (int)cType << endl;
		assert(0);	// invalid value
	}

	if (playaFlags[player] & flag)
		return true;
	return false;
}

BYTE GetCurrentWinner(void)
{
	BYTE i;
	BYTE winner = startPlayer;
	BYTE player = startPlayer;
	BYTE trumpVal = 0;
	BYTE cardVal = 0;
	BYTE bTrumpPlayed = false;
	const BYTE firstTypePlayed = GetCardType(cardTable[startPlayer]);

	for (i = 0; i < 4; i++)
	{
		if (NO_CARD != cardTable[player])
		{
			if (trumps == GetCardType(cardTable[player]))
			{
				bTrumpPlayed = true;
				if(trumpVal <= GetCardVal(cardTable[player]))
				{
					trumpVal = GetCardVal(cardTable[player]);
					winner = player;
				}
			}
			else
			{
				if ((!bTrumpPlayed) && (firstTypePlayed == GetCardType(cardTable[player])))
				{
					if(cardVal <= GetCardVal(cardTable[player]))
					{
						cardVal = GetCardVal(cardTable[player]);
						winner = player;
					}
				}
			}
		}
		player = NextPlayer(player);
	}

	return winner;
}

void EmptyTable(void)
{
	BYTE i;
	for (i = 0; i < 4; i++)
		cardTable[i] = NO_CARD;
}
