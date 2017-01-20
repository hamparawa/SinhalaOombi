#include <windows.h>
#include <vector>
#include <algorithm>
//#include <functional>
#include "play_Win.h"
#include "Common.h"

using namespace std;

void Player_PlayCard(HWND hwndCard)
{
	const BYTE card = playaCards[PLAYER_1][GetDlgCtrlID(hwndCard)];

	bCardPlayable = false;

	if (startPlayer == PLAYER_1)
	{
		downCards[card] = CARD_DOWN;
		playaCards[PLAYER_1][GetDlgCtrlID(hwndCard)] = BLANK_CARD;
		Update_CardTable(GetParent(hwndCard));

		//ani
		ShowPlayAnimation(
			GetParent(hwndCard),
			card,
			PLAYER_1);

		cardTable[PLAYER_1] = card;
		Update_CardTable(GetParent(hwndCard));
		Play_OtherThree(GetParent(hwndCard));
		if (SetWinnerVars(GetParent(hwndCard)))
			SetTimer(GetParent(hwndCard), TIMER_CLEAR, 2000, NULL);
	}
	else // com started play
	{
		// update Outcard table
		if (NO_CARD == GetLowestCard(PLAYER_1, GetCardType(cardTable[startPlayer])))
		{
			for (int k = 0; k < 4; k++)
				SetOutCardType(k, PLAYER_1, GetCardType(cardTable[startPlayer]));
		}
		// check if the played card is valid
		if ((GetCardType(card) == GetCardType(cardTable[startPlayer])) || 
			(GetLowestCard(PLAYER_1, GetCardType(cardTable[startPlayer])) == NO_CARD))
		{
			downCards[card] = CARD_DOWN;
			playaCards[PLAYER_1][GetDlgCtrlID(hwndCard)] = BLANK_CARD;
			Update_CardTable(GetParent(hwndCard));

			// ani
			ShowPlayAnimation(
				GetParent(hwndCard),
				card,
				PLAYER_1);

			cardTable[PLAYER_1] = card;
			Update_CardTable(GetParent(hwndCard));

			Play_UntillFinish(GetParent(hwndCard));
			if (SetWinnerVars(GetParent(hwndCard)))
				SetTimer(GetParent(hwndCard), TIMER_CLEAR, 1500, NULL);
		}
		else // invalid
		{
			PlayCantSound(GetParent(hwndCard));
			bCardPlayable = true;
		}
	}
	return;
}


// Updates The table
void Update_CardTable(HWND hwndParent)
{
	BYTE i;
	BYTE player = startPlayer;

	for (i = 0; i < 4; i++)
	{
		SetWindowPos(GetDlgItem(hwndParent, (player + 32)),
			GetDlgItem(hwndParent, NextPlayer(player + 2) + 32),
			0,0,0,0,
			SWP_NOMOVE | SWP_NOSIZE);

		if (cardTable[player] != NO_CARD)
		{
			ShowWindow(GetDlgItem(hwndParent, player + 32), SW_SHOW);
			InvalidateRect(GetDlgItem(hwndParent, player + 32), NULL, false);
		}
		else
			ShowWindow(GetDlgItem(hwndParent , player + 32), SW_HIDE);

		player = NextPlayer(player);
	}

	for (i = 0; i < 32; i++)
	{
		if (playaCards[i / 8][i % 8] == BLANK_CARD)
			ShowWindow(GetDlgItem(hwndParent, i), SW_HIDE);
		else
			ShowWindow(GetDlgItem(hwndParent, i), SW_SHOW);
	}
	return;
}

void Play_OtherThree(HWND hWndParent)
{
	BYTE player = NextPlayer(startPlayer);
	BYTE i;
	BYTE card;

	for (i = 0; i < 3; i++)
	{
		card = Play_NextCard(player);
		cardTable[player] = NO_CARD;
		Update_CardTable(hWndParent);

		// ani
		ShowPlayAnimation(
			hWndParent,
			card,
			player);

		cardTable[player] = card;
		Update_CardTable(hWndParent);
		player = NextPlayer(player);
	}
}

// Set variables // points etc:
BYTE SetWinnerVars(HWND hwndParent)
{
	BYTE winner;

	winner = GetCurrentWinner();

	if ((winner == PLAYER_1) || (winner == PLAYER_3))
		winSets++;
	else
		lostSets++;

	lastWinner = startPlayer;
	startPlayer = winner;
	rounds++;

	for (int i = 0; i < 4; i++)
		lastHand[i] = cardTable[i];

	InvalidateRect(GetDlgItem(hwndParent, RHS_STATUSID), NULL, true);
	if (rounds == 8)
	{
		rounds = 0;
		for (int i = 0; i < 4; i++)
			lastHand[i] = NO_CARD;

		// one round is over
		if (GivePoints(hwndParent))
			PlayNextRound(hwndParent);
		else
			ShowSummary(hwndParent);
		return false;
	}
	return true;
}

void Play_UntillFinish(HWND hwndParent)
{
	BYTE curPlayer = PLAYER_2;
	BYTE card;

	while (cardTable[curPlayer] == NO_CARD)
	{
		card = Play_NextCard(curPlayer);
		cardTable[curPlayer] = NO_CARD;
		Update_CardTable(hwndParent);

		// ani
		ShowPlayAnimation(
			hwndParent,
			card,
			curPlayer);

		cardTable[curPlayer] = card;
		Update_CardTable(hwndParent);

		curPlayer = NextPlayer(curPlayer);
	}

	return;
}


bool GivePoints(HWND hwndParent)
{
	BYTE winP  = 0;
	BYTE lostP = 0;

	OO_MSGBOXDATA mbdata;
	mbdata.butText1 = "yß";
	mbdata.msgBoxType = MB_ONEBUTTON | MBIT_INFORMATION;
	mbdata.hwndParent = hwndParent;

	// clean the client
	ShowEndAnimation(hwndParent);

	if (winSets > lostSets)
	{
		if (!bCheatApplied)
		{
			// win
			mbdata.Title = "oskqï";
			if (winSets == 8) 
			{
				// kapothi
				bIsLastSeporu = false;  // bug cleaned!
				mbdata.MsgStr = "lfmda;s` leg ;=kla oskqï`";
				winP = 3;
			}
			else
			{
				if (bIsLastSeporu) 
				{
					// seporu win!
					bIsLastSeporu = false;
					//MessageBox(0, "last hand Seopru, Wins 2", "", 0);
					mbdata.MsgStr = "hg w; fifmdarehs\" leg folla oskqï`";
					winP = 2;
				}
				else
				{

					if ((gameStartPlayer == PLAYER_1) || (gameStartPlayer == PLAYER_3))
					{
						// say & win
						mbdata.MsgStr = ";=reïmq lsh,d oskqï\" leg tlhs`";
						winP = 1;
					}
					else
					{
						// didnt say 
						mbdata.MsgStr = "wks;a wh ;=reïmq lsõfõ\" leg folla oskqï`";
						winP = 2;
					}
				}
			}

		}
		else
		{
			mbdata.MsgStr = "fydr l< yskaod leg kE'''''''";
			mbdata.Title = "fydr l<d";
		}

	}
	else

	{
		if (winSets == lostSets)
		{
			// seporu
			bIsLastSeporu = true;
			mbdata.Title = "fifmdare";
			mbdata.MsgStr = "fifmdare\" ldgj;a leg kE'''";
		}
		else // lose
		{
			mbdata.Title = "mrdohs";
			if (lostSets == 8)
			{
				// kapothi // lost
				bIsLastSeporu = false; // bug cleaned!
				mbdata.MsgStr = "lfmda;s` leg ;=kla mrdohs`";
				lostP = 3;
			}
			else
			{
				if (bIsLastSeporu)
				{
					// seopru
					bIsLastSeporu =	false;
					mbdata.MsgStr = "hg w; fifmdarehs\" leg folla mrdohs`";
					lostP = 2;
				}
				else
				{
					// lost
					if ((gameStartPlayer == PLAYER_1) || (gameStartPlayer == PLAYER_3))
					{
						// say & lost
						mbdata.MsgStr = ";=reïmq lsh,d mrdohs\" leg folla meros`";
						lostP = 2;
					}
					else
					{
						// didnt say 
						mbdata.MsgStr = "wks;a wh ;=reïmq lsõfõ\" leghla mrdohs`";
						lostP = 1;
					}
				}
			}
		}
	}

	ShowMsgBox(&mbdata);

	if (!bCheatApplied)
		winPoints += winP;

	lostPoints += lostP;
	InvalidateRect(GetDlgItem(hwndParent, RHS_STATUSID), NULL, true);

	if ((winPoints >= 10) || (lostPoints >= 10))
		return false;
	return true;
}


void PlayNextRound(HWND hwndParent)
{
	BYTE player;
	BYTE card;

	startPlayer = NextPlayer(gameStartPlayer);
	gameStartPlayer = NextPlayer(gameStartPlayer);
	player = startPlayer;

	InitOomibiDeck();
	Update_CardTable(hwndParent);

	if (gameStartPlayer != PLAYER_1)
	{
		trumps = ReturnTrumps(startPlayer);
		SortPlayaCards();
		DisplayTrumpsMsg(trumps, hwndParent);
		Update_CardTable(hwndParent);

		//bug fix deadlock
		Sleep(500);

		while (player != PLAYER_1)
		{
			card = Play_NextCard(player);
			cardTable[player] = NO_CARD;
			Update_CardTable(hwndParent);

			// ani
			ShowPlayAnimation(
				hwndParent,
				card,
				player);

			cardTable[player] = card;
			Update_CardTable(hwndParent);
			player = NextPlayer(player);
		}
		bCardPlayable = true;
	}
	else
	{
		bTrumpsNotYet = true;
		Update_CardTable(hwndParent);
		SetTimer(hwndParent, TIMER_PLAY_START, 500, NULL);
	}
	return;
}

void GetTrumpsFromUser(HWND hwndParent)
{
	BYTE i;

	// hiding cards
	bTrumpsNotYet = true;
	
	// sort 1st 4
	SortCardsb4Trumps();

	InvalidateRect(GetDlgItem(hwndParent, RHS_STATUSID), NULL,false);
	for (i = 0; i < 8; i++)
		InvalidateRect(GetDlgItem(hwndParent, i), NULL, false);

	for (i = 0; i < 4; i++)
		ShowWindow(GetDlgItem(hwndParent,i + TRMP_GET_SPADE), SW_SHOW);

	//MessageBox(hwndParent, "Trumps", "", 0);

	SetTimer(hwndParent, TIMER_WAITTILL_TRUMPS, 300, NULL);

	return;
}


void DisplayTrumpsMsg(BYTE Cardtype, HWND hwndParent)
{
	OO_MSGBOXDATA mbdata;
	char *strType;
	char trumpStr[] = ";=reïmq (  ";
	char buffer[50];

	int i;

	for (i = 0; i < 50; i++)
		buffer[i] = '\0';

	switch (Cardtype)
	{
	case OO_SPADE:
		strType = "biafldamam";
		break;

	case OO_HEART:
		strType = "ydr;";
		break;

	case OO_CLUBS:
		strType = "l,dnr";
		break;

	case OO_DIAMO:
		strType = "reú;";
		break;
	}


	wsprintf(buffer, "%s%s", trumpStr, strType);

	mbdata.msgBoxType = MB_ONEBUTTON | MBIT_INFORMATION;
	mbdata.hwndParent = hwndParent;
	mbdata.butText1 = "yß";
	mbdata.Title = ";=reïmq";
	mbdata.MsgStr = buffer;

	ShowMsgBox(&mbdata);
	return;
}


void AskAndGiveNewGame(HWND hwnd, bool exit)
{
	OO_MSGBOXDATA mbdata;

	mbdata.butText1 = "Tõ";
	mbdata.butText2 = "keye";
	mbdata.hwndParent = hwnd;
	mbdata.msgBoxType = MB_TWOBUTTON | MBIT_QUESTION;
	mbdata.MsgStr = "wÆ;a ;rÕhla wjYH o@";
	mbdata.Title = "wÆ;a ;rÕhla";

	if (BUT_ONE == ShowMsgBox(&mbdata))
	{
		// start with the user
		startPlayer = PLAYER_1;
		gameStartPlayer = PLAYER_1;
		winPoints = 0;
		lostPoints = 0;
		rounds = 0;
		bIsLastSeporu = false;
		bTrumpsNotYet = true;
		bCardPlayable = false;

		// 1.05 cheat
		bCheatApplied = false;
		bCheatMode = 0;

		for (int i = 0; i <4; i++)
		{
			cardCount[i] = 0;
			lastHand[i] = NO_CARD;
		}

		EmptyTable();
		InitOomibiDeck();
		Update_CardTable(hwnd);

		InvalidateRect(GetDlgItem(hwnd, RHS_STATUSID), NULL, true);

		InvalidateRect(hwnd, NULL, false);
		SetTimer(hwnd, TIMER_PLAY_START, 700, NULL);
	}

	return;
}

void ShowSummary(HWND hwndParent)
{
	char buffer[256];
	char winstr[] = "iqNme;=ï` leg oyhu oskqï'''";
	char loststr[] = "leg oyhu mrdohs''' ";
	char strTitle[] = "bjrhs";

	char txtMinus1[] = ";j leg "; 
	char txtMinus2[] = " la Khhs`";
	char txtMinusPoints[5];

	int pointsMinus;
	int i;

	OO_MSGBOXDATA mbdata;

	for (i =0 ; i < 256; i++)
		buffer[i] = '\0';
	for (i = 0; i < 5; i++)
		txtMinusPoints[i] = '\0';

	if (winPoints >= 10)
		strcat(buffer, winstr);
	else
	{
		strcat(buffer, loststr);
		if (lostPoints > 10)
		{
			pointsMinus = lostPoints - 10;
			itoa(pointsMinus, txtMinusPoints, 10);
			strcat(buffer, txtMinus1);
			strcat(buffer, txtMinusPoints);
			strcat(buffer, txtMinus2);
		}
	}

	mbdata.butText1 = "yß";
	mbdata.hwndParent = hwndParent;
	mbdata.msgBoxType = MB_ONEBUTTON | MBIT_INFORMATION;
	mbdata.MsgStr = buffer;
	mbdata.Title = strTitle;

	ShowMsgBox(&mbdata);

	AskAndGiveNewGame(hwndParent, false);

	return;
}


// sorts the cards for display
void SortPlayaCards(void)
{
	typedef vector<BYTE> byteVector;
	typedef byteVector::iterator bIterator;


	byteVector vTemphand(8);
	bIterator iSt, iEnd, it;

	iSt = vTemphand.begin();
	iEnd = vTemphand.end();

	for (int i = 0; i < 8; i++)
		vTemphand[i] = playaCards[PLAYER_1][i];

	sort(iSt, iEnd);

	for (int i = 0; i < 8; i++)
	{
		playaCards[PLAYER_1][i] = vTemphand[i];
		cardDeck[i] = vTemphand[i];
	}
	return ;
}


void SortCardsb4Trumps(void)
{
	typedef vector<BYTE> byteVector;
	typedef byteVector::iterator bIterator;


	byteVector vTemphand(4);
	bIterator iSt, iEnd, it;

	iSt = vTemphand.begin();
	iEnd = vTemphand.end();

	for (int i = 0; i < 4; i++)
		vTemphand[i] = playaCards[PLAYER_1][i];

	sort(iSt, iEnd);

	for (int i = 0; i < 4; i++)
	{
		playaCards[PLAYER_1][i] = vTemphand[i];
		cardDeck[i] = vTemphand[i];
	}
	return ;
}