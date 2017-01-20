#include <windows.h>
#include "common.h"

BYTE Play_NextCard(const BYTE player)
{
	BYTE i;
	BYTE playCard = NO_CARD;

	if (startPlayer == player)  // first card selections
	{
		playCard = Play_GetLowRiskHighest(player,true);	// 1
		if (playCard == NO_CARD)
		{
			playCard = Play_GetLowRiskHighest(player,false); //2
			if (playCard == NO_CARD)
			{
				playCard = Play_GetOponentChanceCard(player);	// 3
				if (playCard == NO_CARD)
				{	
					playCard = Play_NormalHighNoTrumps(player); // 4
					if (playCard == NO_CARD)
						playCard = Play_NormalHighInclTrumps(player);  // 5
				}
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

BYTE Play_GetLowRiskHighest(const BYTE player, bool ace)
{
	BYTE i;
	BYTE maxValcard = 0;
	BYTE lowRskCard = NO_CARD;
	BYTE bValidCardFound = false;
	BYTE enmy1, enmy2;
	BYTE bEnmy_1_Danger = true;
	BYTE bEnmy_2_Danger = true;
	BYTE cType;
	BYTE posVal;
	BYTE highCnt = 0;
//	bool highCardFound = false;

	// first round: to check if there is a highest card
	SetCardPosVals();
	for (i = 0; i < 8; i++)
	{
		if (playaCardPosVal[player][i] != NO_CARD)
		{
			if (trumps != GetCardType(playaCards[player][i]))
				posVal = playaCardPosVal[player][i];
			else
			{
                if (IsCardsOut(player, GetEnmy1(player), trumps) &&
					IsCardsOut(player, GetEnmy2(player), trumps))
					posVal = 0;
				else
					posVal = playaCardPosVal[player][i];
			}

			maxValcard = max(maxValcard, posVal);
			if (maxValcard == OO_RANK_8)
				highCnt++;
		}
	}

	// second round: check IF card is playable
	if (maxValcard == OO_RANK_8)
	{
		for(int k = 0; (k < highCnt); k++)
		{
			for (i = 0; i < 8; i++)
			{
				if (OO_RANK_8 == playaCardPosVal[player][i])
				{
					if ((ace) && (OO_RANK_8 != GetCardVal(playaCards[player][i])))
						break;

					cType = GetCardType(playaCards[player][i]);
					if(playaCards[player][i] != BLANK_CARD)
					{
						enmy1 = GetEnmy1(player);
						enmy2 = GetEnmy2(player);

						if (!IsCardsOut(player, enmy1, GetCardType(cType)) &&
							!IsCardsOut(player, enmy2, GetCardType(cType)))
							lowRskCard = playaCards[player][i];
						else
						{
							if ((IsCardsOut(player, enmy1, cType) && IsCardsOut(player, enmy1, trumps)) ||
								(!IsCardsOut(player, enmy1, cType)))
								bEnmy_1_Danger = false;

							if ((IsCardsOut(player, enmy2, cType) && IsCardsOut(player, enmy2, trumps)) ||
								(!IsCardsOut(player, enmy2, cType)))
								bEnmy_2_Danger = false;

							if ( !bEnmy_1_Danger && !bEnmy_2_Danger)
								lowRskCard = playaCards[player][i];

							if (lowRskCard != NO_CARD)
							{
								return lowRskCard;
								//highCardFound = true;
								//MessageBox(0,"High found", "", 0);
							}
						}
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

			if ((!IsCardsOut(player, enmy1, cType) && !IsCardsOut(player, enmy2, cType)) && 
				(IsCardsOut(player, partner, cType) && !IsCardsOut(player, partner, trumps)))
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
				// no K's played!
				if (OO_RANK_7 != GetCardVal(playaCards[player][i]))
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
	{
		for (int i = 0; i < 4; i++)
			SetOutCardType(i, player, playType);
	}
	// if the partner is the winner, just play the lowest card
	if (curWinner == GetPartner(player))
	{
		// if have the ace play it! 
		playCard = GetHighestCard(player, playType);
		if (!((OO_RANK_8 == GetCardVal(playCard)) &&
			OO_RANK_7 != GetCardVal(cardTable[GetPartner(player)])))
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
			else // check the trumps are enough to win
			{
                if(!IsThisCardWinnable(player, playCard))
				{
					while(playCard == NO_CARD)
					{
						playCard = GetNextHighestCard(player, playCard);
						if (IsThisCardWinnable(player, playCard))
							break;
					}

					// bug fix 1.09
					if ((IsThisCardWinnable(player, GetHighestCard(player, trumps))) &&
						(!IsThisCardWinnable(player, playCard)))
                        playCard = GetHighestCard(player, trumps);

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


// count the cards
void SetCountCards(const BYTE Player, const BYTE card)
{

	int i,j,k;
	int cardCnt;

	// count 8 cards
	cardCount[GetCardType(card)]++;
	for (i = 0; i < 4; i++)
	{
		if (cardCount[i] == 8)
		{
			for (j = 0; j < 4; j++)
				for (k = 0; k <4; k++)
                    SetOutCardType(k,j,i);
		}
	}

	// count 8 cards with the cards in hand
	for (i = 0; i < 4; i++) // 4 each type
	{
		cardCnt = 0;
		for(j = 0;j < 8; j++)
		{
			if ((i == GetCardType(playaCards[Player][j])) &&
				(BLANK_CARD != playaCards[Player][j]))
				cardCnt++;
		}

		if (8 == (cardCnt + cardCount[i]))
		{
			SetOutCardType(Player, GetEnmy1(Player), i);
			SetOutCardType(Player, GetEnmy2(Player), i);
			SetOutCardType(Player, GetPartner(Player), i);
		}
	}

	return;
}
