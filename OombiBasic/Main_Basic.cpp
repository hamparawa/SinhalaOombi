#include <iostream>
using namespace std;

#include <windows.h>
#include "OO_common.h"

BYTE cardDeck[32];
BYTE trumps;
BYTE downCards[32];
BYTE playaCards[4][8];
BYTE playaFlags[4];
BYTE playaCardPosVal[4][8];
BYTE cardTable[4];
BYTE startPlayer;

// proto
void ShowShufflePack();
void ShowCardPosVals();
char *CT(BYTE, char *);

struct CARD_TYPE
{
	char *str;
} pCardsT[] = {
"Spade",
"Heart",
"Clubs",
"Diamo"
};

struct CARD_VAL
{
	char *str;
} pCardsV[] = {
" 7",
" 8",
" 9",
"10",
" J",
" Q",
" K",
" A"
};

int main()
{
	BYTE i;
    char b[256];

	InitOomibiDeck();
	ShowShufflePack();

	startPlayer = PLAYER_1;
	trumps = ReturnTrumps(startPlayer);
	cout << "\nTrumps : " << pCardsT[trumps].str << "\n\n";

	SetCardPosVals();
	ShowCardPosVals();

	cout << "\nPlay :\n\n";
	for (i = 0; i < 8; i++)
	{
		cout << "P:" << (int)startPlayer << "\t" << CT(Play_NextCard(startPlayer), b);
		cout << "\tCur win P:" << (int)GetCurrentWinner() << endl;

		cout << "P:" << (int)NextPlayer(startPlayer) << "\t" << CT(Play_NextCard(NextPlayer(startPlayer)), b);
		cout << "\tCur win P:" << (int)GetCurrentWinner() << endl;

		cout << "P:" << (int)NextPlayer(startPlayer + 1) << "\t" << CT(Play_NextCard(NextPlayer(startPlayer + 1)), b);
		cout << "\tCur win P:" << (int)GetCurrentWinner() << endl;

		cout << "P:" << (int)NextPlayer(startPlayer + 2) << "\t" << CT(Play_NextCard(NextPlayer(startPlayer + 2)), b);
		cout << "\tCur win P:" << (int)GetCurrentWinner() << endl;

		cout << endl;
		startPlayer = GetCurrentWinner();
		EmptyTable();
	}

	getchar();
	return 0;
}

void ShowShufflePack()
{
	BYTE i,j;
	char buf[256];

	cout << "Sinhala_Oombi basic_beta\n";
	cout << "(Autoplay mode)\n\n";

	cout << "Shuffled Pack :" << endl;
	cout << "#\tPlayer0\t\tPlayer1\t\tPlayer2\t\tPlayer3\n\n";

	for (i = 0; i < 8; i++)
	{
		cout << (int)i << "\t";
		for (j = 0; j < 4; j++)
			cout << CT(playaCards[j][i], buf) << "\t";
		cout << endl;
	}
	cout << endl;

	return;
}

void ShowCardPosVals()
{
	BYTE i,j;

	cout << "\nCard Position Values :" << endl;
	cout << "#\tPlayer0\t\tPlayer1\t\tPlayer2\t\tPlayer3\n\n";

	for (i = 0; i < 8; i++)
	{
		cout << (int)i << "\t";
		for (j = 0; j < 4; j++)
			cout << (int)playaCardPosVal[j][i] << "\t\t";
		cout << endl;
	}
	cout << endl;

	return;
}

char *CT(BYTE card, char *buffer)
{
/*	int i;
	for (i = 0; i < 256; i++)
		buffer[i] = '\0';

	strcat(buffer, pCardsT[GetCardType(card)].str);
	strcat(buffer, " ");
	strcat(buffer, pCardsV[GetCardVal(card)].str);
*/
	wsprintf(buffer, "%s %s",
		pCardsT[GetCardType(card)].str,
		pCardsV[GetCardVal(card)].str);


	return buffer;
}
