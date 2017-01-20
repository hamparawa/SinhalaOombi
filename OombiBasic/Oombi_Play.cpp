#include <windows.h>
#include "OO_common.h"

#include <iostream>
using namespace std;

BYTE Play_NextCard(const BYTE player)
{
	BYTE i;
	BYTE playCard = NO_CARD;

	if (startPlayer == player)  // first card selections
	{
		playCard = Play_GetLowRiskHighest(player);	// 1
		if (playCard == NO_CARD)
		{
			playCard = Play_GetOponentChanceCard(player);	// 2
			if (playCard == NO_CARD)
			{	
				playCard = Play_NormalHighNoTrumps(player); // 3
				if (playCard == NO_CARD)
					playCard = Play_NormalHighInclTrumps(player);  // 4
			}
		}


		if (playCard != NO_CARD)
		{
			for (i = 0; i < 8; i++)
			{
				if (playCard == playaCards[player][i])
					playaCards[player][i] = BLANK_CARD;
			}
		}
	}
	else // already a card is played
	{
		playCard = Play_AlreadyStarted(player);
	}

	if (playCard != NO_CARD)
	{
		downCards[playCard] = CARD_DOWN;
		cardTable[player] = playCard;
		for (i = 0; i < 8; i++)
		{
			if (playaCards[player][i] == playCard)
				playaCards[player][i] = BLANK_CARD;
		}
	}
	return playCard;
}

BYTE Play_GetLowRiskHighest(const BYTE player)
{
	BYTE i;
	BYTE maxValcard = 0;
	BYTE lowRskCard = NO_CARD;
	BYTE bValidCardFound = false;
	BYTE enmy1, enmy2;
	BYTE bEnmy_1_Danger = true;
	BYTE bEnmy_2_Danger = true;
	BYTE cType;

	// first round: to check if there is a highest card
	SetCardPosVals();
	for (i = 0; i < 8; i++)
		if (playaCardPosVal[player][i] != NO_CARD)
			maxValcard = max(maxValcard, playaCardPosVal[player][i]);

	// second round: check IF card is playable
	if (maxValcard == OO_RANK_8)
	{
		for (i = 0; i < 8; i++)
		{
			if (OO_RANK_8 == playaCardPosVal[player][i])
			{
				cType = GetCardType(playaCards[player][i]);
				if(playaCards[player][i] != BLANK_CARD)
				{
					enmy1 = GetEnmy1(player);
					enmy2 = GetEnmy2(player);

					if (!IsCardsOut(enmy1, GetCardType(cType)) &&
						!IsCardsOut(enmy2, GetCardType(cType)))
						lowRskCard = playaCards[player][i];
					else
					{
						if ((IsCardsOut(enmy1, cType) && IsCardsOut(enmy1, trumps)) ||
							(!IsCardsOut(enmy1, cType)))
							bEnmy_1_Danger = false;

						if ((IsCardsOut(enmy2, cType) && IsCardsOut(enmy2, trumps)) ||
							(!IsCardsOut(enmy2, cType)))
							bEnmy_2_Danger = false;

						if ( !bEnmy_1_Danger && !bEnmy_2_Danger)
							lowRskCard = playaCards[player][i];
					}
				}

			}
		}
	}

	return lowRskCard;
}

// give partner a chance to CUT!
BYTE Play_GetOponentChanceCard(const BYTE player)
{
	BYTE i;
	BYTE cOpoChance = NO_CARD;
	BYTE card;
	BYTE cType;
	BYTE enmy1, enmy2;
	BYTE partner;

	enmy1 = GetEnmy1(player);
	enmy2 = GetEnmy2(player);
	partner = GetPartner(player);

	for (i = 0; i < 8; i++)
	{
		card = playaCards[player][i];
		if (card != BLANK_CARD)
		{
			cType = GetCardType(card);

			if ((!IsCardsOut(enmy1, cType) && !IsCardsOut(enmy2, cType)) && 
				(IsCardsOut(partner, cType) && !IsCardsOut(partner, trumps)))
				return card;
		}
	}
	return cOpoChance;
}


// highest card (excl: Trumps)
BYTE Play_NormalHighNoTrumps(const BYTE player)
{
	BYTE i;
	BYTE maxValCard = 0;
	BYTE normalhighCard = NO_CARD;

	SetCardPosVals();
	for (i = 0; i < 8; i++)
		if (NO_CARD != playaCardPosVal[player][i] )
			if (trumps != GetCardType(playaCards[player][i]))
				maxValCard = max(maxValCard, playaCardPosVal[player][i]);

	for (i = 0; i < 8; i++)
		if ((maxValCard == playaCardPosVal[player][i]) && 
			(trumps != GetCardType(playaCards[player][i])))
			return playaCards[player][i];

	return normalhighCard;
}

// play Highest card (including trumps)
BYTE Play_NormalHighInclTrumps(const BYTE player)
{
	BYTE i;
	BYTE maxValCard = 0;
	BYTE normalhighCard = NO_CARD;
	BYTE bCardFound = false;

	SetCardPosVals();
	for (i = 0; i < 8; i++)
		if (NO_CARD != playaCardPosVal[player][i] )
		{
			maxValCard = max(maxValCard, playaCardPosVal[player][i]);
			bCardFound = true;
		}

	for (i = 0; i < 8; i++)
		if ((maxValCard == playaCardPosVal[player][i]) && bCardFound)
			return playaCards[player][i];

		return normalhighCard;
}

BYTE Play_AlreadyStarted(const BYTE player)
{
	BYTE playCard;
	const BYTE playType = GetCardType(cardTable[startPlayer]); 
	const BYTE curWinner = GetCurrentWinner();
  
	// update the OutCard type flags
	if (NO_CARD == GetLowestCard(player, playType))
		SetOutCardType(player, playType);

	// if the partner is the winner, just play the lowest card
	if (curWinner == GetPartner(player))
	{
        playCard = GetLowestCard(player, playType);
		if (playCard == NO_CARD)
		{
			playCard = Play_ReleaseSingleCard(player);
			if (playCard == NO_CARD)
			{
				playCard = Play_NormalLowNoTrumps(player);
				if (playCard == NO_CARD)
					playCard = GetLowestCard(player, trumps);
			}
		}
	}
	else // the OTHERS are WINNING!
	{
		playCard = GetHighestCard(player, playType);
        if (playCard == NO_CARD)
		{
			playCard = GetLowestCard(player, trumps);
			if (playCard == NO_CARD) // no trumps either
			{
				playCard = Play_ReleaseSingleCard(player);
				if (playCard == NO_CARD)
				{
					playCard = Play_NormalLowNoTrumps(player);
				}
			}
			else // check the trums are enough to win
			{
                if(!IsThisCardWinnable(player, playCard))
				{
					while(playCard == NO_CARD)
					{
						playCard = GetNextHighestCard(player, playCard);
						if (IsThisCardWinnable(player, playCard))
							break;
					}
					if (!IsThisCardWinnable(player, playCard)) // can't win with trupms
					{
						playCard = Play_ReleaseSingleCard(player);
						if (playCard == NO_CARD)
						{
							playCard = Play_NormalLowNoTrumps(player);
							if (playCard == NO_CARD)
								playCard = GetLowestCard(player, trumps);
						}
					}
				}
			}
		}
		else // if can't win just drop the lowest card
		{
			if (!IsThisCardWinnable(player, playCard))
				playCard = GetLowestCard(player, playType);
		}
	}

	return playCard;
}

BYTE Play_ReleaseSingleCard(const BYTE player)
{
	BYTE releaseCard = NO_CARD;
	BYTE cardVal[4] = { 0, 0, 0, 0};
	BYTE cType;
	BYTE minVal = 8;
	BYTE i,j;

	SetCardPosVals();

    for (i = 0; i < 8; i++)
	{
		if (BLANK_CARD != playaCards[player][i])
		{
			cType = GetCardType(playaCards[player][i]);
			cardVal[cType]++;
		}
	}

	// check for one cards excl: trumps
	for (i = 0; i < 4; i++)
        if (cardVal[i] == 1)
		{
			for (j = 0; j < 8; j++)
			{
				if ((BLANK_CARD != playaCards[player][j]) && 
					(i == GetCardType(playaCards[player][j])) && 
					(i != trumps))
				{
					// if high card don't give it!
					if (OO_RANK_7 > playaCardPosVal[player][j])
						releaseCard = playaCards[player][j];
				}
			}
		}

	return releaseCard;
}

BYTE Play_NormalLowNoTrumps(const BYTE player)
{
	BYTE i;
	BYTE minValCard = 8;
	BYTE normalLowCard = NO_CARD;

	SetCardPosVals();
	for (i = 0; i < 8; i++)
		if (NO_CARD != playaCardPosVal[player][i] )
			if (trumps != GetCardType(playaCards[player][i]))
				minValCard = min(minValCard, playaCardPosVal[player][i]);

	if (minValCard != 8)
	{
	for (i = 0; i < 8; i++)
		if ((minValCard == playaCardPosVal[player][i]) && 
			(trumps != GetCardType(playaCards[player][i])))
			return playaCards[player][i];
	}
	return normalLowCard;
}


// check a card, if that is winnable
BYTE IsThisCardWinnable(const BYTE Cplayer, const BYTE card)
{
	const BYTE curWinner = GetCurrentWinner();
	const BYTE curWinningCard = cardTable[curWinner];

	BYTE winnable = false;

    if (GetCardType(card) == GetCardType(curWinningCard))
	{
		if (GetCardVal(card) > GetCardVal(curWinningCard))
			winnable = true;
	}
	else
	{
		if (trumps == GetCardType(card))
			winnable = true;
	}

	return winnable;
}