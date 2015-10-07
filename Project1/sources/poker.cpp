// Author: Chi-Hao Kuo
// Updated: 11/16/2014
// All content © 2014 DigiPen(USA) Corporation, all rights reserved.

#include "nintynine.h"

using namespace POKER;

/*--------------------------------------------------------------------------*
Name:           DisplayCard

Description:	Display card number. Display X, J, Q, K if >= 10.

Arguments:      value: card face value.

Returns:        The card number or character.
*---------------------------------------------------------------------------*/
char POKER::DisplayCard(int value)
{
	if (value < 10)
		return (value + '0');
	else
	{
		if (value == 10)
			return 'X';
		else if (value == 11)
			return 'J';
		else if (value == 12)
			return 'Q';
		else
			return 'K';
	}
}

/*--------------------------------------------------------------------------*
Name:           DisplaySpecial

Description:	Display special cards.

Arguments:      list: card list.

Returns:        None.
*---------------------------------------------------------------------------*/
void POKER::DisplaySpecial(CardList list)
{
	bool is_SP_10 = false;
	bool is_SP_20 = false;
	bool is_SP_99 = false;
	bool is_SP_REVERSE = false;
	bool is_SP_SKIP = false;
	bool is_SP_PICK = false;
	bool is_special = false;

	for (int i = 0; i < NUM_CARDS_PER_PLAYER; ++i)
	{
		// use weight to detect if the card is special
		// normal card always has weight 0

		if (list[i]->weight_)
		{
			is_special = true;

			if (list[i]->value_ == SP_10)
				is_SP_10 = true;
			if (list[i]->value_ == SP_20)
				is_SP_20 = true;
			if (list[i]->value_ == SP_99)
				is_SP_99 = true;
			if (list[i]->value_ == SP_REVERSE)
				is_SP_REVERSE = true;
			if (list[i]->value_ == SP_SKIP)
				is_SP_SKIP = true;
			if (list[i]->value_ == SP_PICK)
				is_SP_PICK = true;
		}
	}

	if (is_special)
		printf("\n");

	if (is_SP_10)
		printf("X: Increase/decrease 10.\n");
	if (is_SP_20)
		printf("Q: Increase/decrease 20.\n");
	if (is_SP_99)
		printf("K: Instantly goes to 99.\n");
	if (is_SP_REVERSE)
		printf("4: Reverse play order.\n");
	if (is_SP_SKIP)
		printf("J: Skip turn.\n");
	if (is_SP_PICK)
		printf("5: Pick another player.\n");
}

// Display AI status
/*--------------------------------------------------------------------------*
Name:           DisplayAI

Description:	Display AI status.

Arguments:      ai: player AI.

Returns:        None.
*---------------------------------------------------------------------------*/
char *POKER::DisplayAI(int ai)
{
	if (ai == AI_PLAYER)
		return "Human";
	else if (ai == AI_RANDOM)
		return "Monkey";
	else if (ai == AI_DUMB)
		return "Idiot";
	else
		return "Smart";
}