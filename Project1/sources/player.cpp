// Author: Chi-Hao Kuo
// Updated: 11/16/2014
// All content © 2014 DigiPen(USA) Corporation, all rights reserved.

#include <cstdlib>		// std::rand

#include "nintynine.h"

using namespace POKER;

// public functions

/*--------------------------------------------------------------------------*
Name:           Play

Description:	Each player takes turn.

Arguments:      total: Current running total. (Random player doesn't care)

Returns:        Running total after this turn.
*---------------------------------------------------------------------------*/
int Player::Play(int total)
{
	switch (AI_)
	{
	case AI_RANDOM:
		return PlayRandom();
		break;

	case AI_DUMB:
		return PlayerDumb(total);
		break;

	case AI_SMART:
		return PlayerSmart(total);
		break;

	default:
		return PlayHuman(total);
		break;
	}
}

/*--------------------------------------------------------------------------*
Name:           PlayHuman

Description:	(AI 0) human player. Ask human to pick card.

Arguments:      total: Current running total.

Returns:        Running total after this turn.
*---------------------------------------------------------------------------*/
int Player::PlayHuman(int total)
{
	printf("Total: %d\n", total);
	printf("Your hands: ");
	for (int i = 0; i < NUM_CARDS_PER_PLAYER; ++i)
		printf(" %c  ", DisplayCard(hands_[i]->value_));
	printf("\n            ");
	for (int i = 1; i <= NUM_CARDS_PER_PLAYER; ++i)
		printf("(%d) ", i);
	printf("\n");
	DisplaySpecial(hands_);
	printf("\nYour choice (1-%d): ", NUM_CARDS_PER_PLAYER);

	int input = 1;
	scanf_s("%i", &input);
	printf("\n");

	return (input - 1);
}

/*--------------------------------------------------------------------------*
Name:           PlayRandom

Description:	(AI 1) "random" player. Randomly choose a card from hands.

Arguments:      None.

Returns:        Running total after this turn.
*---------------------------------------------------------------------------*/
int Player::PlayRandom(void)
{
	return std::rand() % NUM_CARDS_PER_PLAYER;
}

/*--------------------------------------------------------------------------*
Name:           PlayerDumb

Description:	(AI 2) "dumb" player. Only pick the highest possible number of cards.
It will not pick the card that will get the running total to exceed the limit.
Also it does not know about the special ability on card.

Arguments:      total: Current running total.

Returns:        Running total after this turn.
*---------------------------------------------------------------------------*/
int Player::PlayerDumb(int total)
{
	int value = 0;
	int card_index = 0;
	int value_limit = RUNNING_TOTAL_LIMIT - total;

	// go through all cards on hand, find the largest value not exceeding limit

	for (int i = 0; i < NUM_CARDS_PER_PLAYER; ++i)
	{
		if (hands_[i]->value_ > value)
		{
			if (hands_[i]->value_ <= value_limit)
			{
				card_index = i;
				value = hands_[i]->value_;
			}
		}
	}

	return card_index;
}

/*--------------------------------------------------------------------------*
Name:           PlayerSmart

Description:	(AI 3) "smart" player. Use special cards when dealing with critical situation
First, it will try to pick the highest possible number of cards. (but it knows those special cards)
If there's no normal card it can deal, it will then choose special ability cards.
But it will deal the ones with lower weight first.
And only save the most important ones (highest weight) to last.

Arguments:      total: Current running total.

Returns:        Running total after this turn.
*---------------------------------------------------------------------------*/
int Player::PlayerSmart(int total)
{
	int value = 0;
	int card_index = 0;
	int value_limit = RUNNING_TOTAL_LIMIT - total;
	bool can_use_normal_card = false;

	int special_index = 0;

	// go through all normal cards (weight == SP_NULL_WEIGHT) on hand
	// find the largest value not exceeding limit
	// if no card is found, use special card

	for (int i = 0; i < NUM_CARDS_PER_PLAYER; ++i)
	{
		// find special card (weight 1 than weight 2)
		if ((special_index == 0) && (hands_[i]->weight_ != SP_NULL_WEIGHT))
		{
			if ((hands_[i]->weight_ == 2) && (hands_[special_index]->weight_ == 0))
				special_index = i;
			else if ((hands_[i]->weight_ == 1) && (hands_[special_index]->weight_ != 1))
				special_index = i;
		}

		if ((hands_[i]->weight_ == SP_NULL_WEIGHT) && (hands_[i]->value_ > value))
		{
			if (hands_[i]->value_ <= value_limit)
			{
				card_index = i;
				value = hands_[i]->value_;
				can_use_normal_card = true;
			}
		}
	}

	if (can_use_normal_card)
		return card_index;
	else
		return special_index;
}