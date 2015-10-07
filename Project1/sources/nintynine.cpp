// Author: Chi-Hao Kuo
// Updated: 11/16/2014
// All content © 2014 DigiPen(USA) Corporation, all rights reserved.

#include <algorithm>	// std::random_shuffle
#include <ctime>        // std::time
#include <cstdlib>		// std::rand

#include "nintynine.h"

using namespace POKER;

// public functions

/*--------------------------------------------------------------------------*
Name:           NintyNine

Description:	Constructor.

Arguments:      is_display: Should the game info be displayed.

Returns:        None.
*---------------------------------------------------------------------------*/
NintyNine::NintyNine(bool is_display)
	: deck_index_(0), total_(0), players_left_(NUM_PLAYERS), is_order_increase_(true), is_display_(is_display)
{
	std::srand(unsigned(std::time(0)));
}

/*--------------------------------------------------------------------------*
Name:           ~NintyNine

Description:	Destructor.

Arguments:      None.

Returns:        None.
*---------------------------------------------------------------------------*/
NintyNine::~NintyNine()
{
}

/*--------------------------------------------------------------------------*
Name:           Initialize

Description:	Initialize the game (create players and randomize deck).

Arguments:      None.

Returns:        None.
*---------------------------------------------------------------------------*/
void NintyNine::Initialize(void)
{
	InitPlayers();
	CreateDeck();
}

/*--------------------------------------------------------------------------*
Name:           Play

Description:	Only play a single game, returns the winner

Arguments:      stats: record game stats.

Returns:        Winner id.
*---------------------------------------------------------------------------*/
int NintyNine::Play(GameStats &stats)
{
	// initialize a game
	deck_index_ = 0;
	total_ = 0;
	players_left_ = NUM_PLAYERS;
	is_order_increase_ = true;

	Shuffle();
	DealCards();

	int num_rounds = 0;
	int num_turns = 0;
	// flag on if the game ends (only one player left)
	bool is_end = false;
	int player_num = 0;

	// play the game

	while (!is_end)
	{
		++num_turns;

		if (((player_num == 0) && (is_order_increase_)) || ((player_num == (NUM_PLAYERS - 1)) && (!is_order_increase_)))
		{
			if (is_display_)
				printf("\n(Round %d) TOTAL: %d\n", num_rounds, GetRunningTotal());

			++num_rounds;
		}

		is_end = PlayerTurn(player_num);
		player_num = NextPlayer(player_num);

		if (is_end)
			break;
	}

	// update stats

	stats.num_rounds_ += num_rounds - 1;
	if (stats.highest_rounds_ < num_rounds)
		stats.highest_rounds_ = num_rounds;
	if ((stats.lowest_rounds_ == 0) || (stats.lowest_rounds_ > num_rounds))
		stats.lowest_rounds_ = num_rounds;
	if (NUM_ROUNDS_LIMIT < num_rounds)
		++stats.exceed_limit_rounds_;

	stats.num_turns_ += num_turns - 1;
	if (stats.highest_turns_ < num_turns)
		stats.highest_turns_ = num_turns;
	if ((stats.lowest_turns_ == 0) || (stats.lowest_turns_ > num_turns))
		stats.lowest_turns_ = num_turns;
	if (NUM_TURNS_LIMIT < num_turns)
		++stats.exceed_limit_turns_;

	// get winner

	int winner = 0;
	for (int i = 0; i < NUM_PLAYERS; ++i)
	{
		if (!players_[i]->is_defeat_)
		{
			winner = i;
			break;
		}
	}

	if (is_display_)
		printf("\nPlayer %d (%s) Wins!\n", winner, DisplayAI(players_[winner]->AI_));

	return winner;
}

/*--------------------------------------------------------------------------*
Name:           Free

Description:	Free memory.

Arguments:      None.

Returns:        None.
*---------------------------------------------------------------------------*/
void NintyNine::Free(void)
{
	FreePlayers();
	FreeDeck();
}

/*--------------------------------------------------------------------------*
Name:           GetDeck

Description:	Get a full deck.

Arguments:      None.

Returns:        A deck of cards (vectors of card face values).
*---------------------------------------------------------------------------*/
CardList NintyNine::GetDeck(void)
{
	return deck_;
}

/*--------------------------------------------------------------------------*
Name:           GetRunningTotal

Description:	Get running total

Arguments:      None.

Returns:        Running total of current game.
*---------------------------------------------------------------------------*/
int NintyNine::GetRunningTotal(void)
{
	return total_;
}

/*--------------------------------------------------------------------------*
Name:           GetPlayerList

Description:	Get player list.

Arguments:      None.

Returns:        List of players.
*---------------------------------------------------------------------------*/
PlayerList NintyNine::GetPlayerList(void)
{
	return players_;
}

/*--------------------------------------------------------------------------*
Name:           DisplayDeck

Description:	Display the entire deck.

Arguments:      None.

Returns:        None.
*---------------------------------------------------------------------------*/
void NintyNine::DisplayDeck(void)
{
	for (int i = 0; i < (NUM_VALUES * SU_NUMBER); ++i)
		printf("%c, ", POKER::DisplayCard(deck_[i]->value_));

	printf("\n");
}

/*--------------------------------------------------------------------------*
Name:           DisplayHands

Description:	Display players' hands.

Arguments:      None.

Returns:        None.
*---------------------------------------------------------------------------*/
void NintyNine::DisplayHands(void)
{
	for (int i = 0; i < NUM_PLAYERS; ++i)
	{
		printf("Player %d (%s): ", i, DisplayAI(players_[i]->AI_));
		for (int j = 0; j < NUM_CARDS_PER_PLAYER; ++j)
			printf("%c ", POKER::DisplayCard(players_[i]->hands_[j]->value_));

		printf("\n");
	}
}

// private functions

/*--------------------------------------------------------------------------*
Name:           CreateDeck

Description:	Create a deck of 52 cards

Arguments:      None.

Returns:        None.
*---------------------------------------------------------------------------*/
void NintyNine::CreateDeck(void)
{
	// create 52 cards and put them into card list and deck list

	for (int i = 0; i < SU_NUMBER; ++i)
	{
		for (int j = 0; j < NUM_VALUES; ++j)
		{
			Cards *card = new Cards(j + 1, (SUIT)i, 1);

			// assign weights for special ability cards

			switch (card->value_)
			{
			case SP_10:
				card->weight_ = SP_10_WEIGHT;
				break;
			case SP_20:
				card->weight_ = SP_20_WEIGHT;
				break;
			case SP_99:
				card->weight_ = SP_99_WEIGHT;
				break;
			case SP_REVERSE:
				card->weight_ = SP_REVERSE_WEIGHT;
				break;
			case SP_SKIP:
				card->weight_ = SP_SKIP_WEIGHT;
				break;
			case SP_PICK:
				card->weight_ = SP_PICK_WEIGHT;
				break;
			default:
				card->weight_ = SP_NULL_WEIGHT;
				break;
			}

			cards_.emplace_back(card);
			deck_.emplace_back(card);
		}
	}
}

/*--------------------------------------------------------------------------*
Name:           FreeDeck

Description:	Free deck lists.

Arguments:      None.

Returns:        None.
*---------------------------------------------------------------------------*/
void NintyNine::FreeDeck(void)
{
	for (int i = 0; i < (NUM_VALUES * SU_NUMBER); ++i)
		delete cards_[i];

	deck_.clear();
	cards_.clear();
}

/*--------------------------------------------------------------------------*
Name:           InitPlayers

Description:	Initialize players.

Arguments:      None.

Returns:        None.
*---------------------------------------------------------------------------*/
void NintyNine::InitPlayers(void)
{
	if (!USE_PLAYER_AI_LIST)
	{
		int human_ai = 0;

		// get random order for human player
		if (AI_CHANCE_PLAYER)
			human_ai = std::rand() % NUM_PLAYERS;

		for (int i = 0; i < NUM_PLAYERS; ++i)
		{
			// create human player
			if (AI_CHANCE_PLAYER && (human_ai == i))
			{
				Player *player = new Player(0);
				players_.emplace_back(player);
			}
			else
			{
				// let pc to pick which AI for each player
				int cpu_ai = std::rand() % 100;
				if (cpu_ai < AI_CHANCE_RANDOM)
					cpu_ai = 1;
				else if (cpu_ai < (AI_CHANCE_RANDOM + AI_CHANCE_DUMB))
					cpu_ai = 2;
				else
					cpu_ai = 3;

				Player *player = new Player(cpu_ai);
				players_.emplace_back(player);
			}
		}
	}
	else
	{
		// use preset PLAYER_AI list to assign AI to each player
		for (int i = 0; i < NUM_PLAYERS; ++i)
		{
			Player *player = new Player(PLAYER_AI[i]);
			players_.emplace_back(player);
		}
	}
}

/*--------------------------------------------------------------------------*
Name:           FreePlayers

Description:	free player memory.

Arguments:      None.

Returns:        None.
*---------------------------------------------------------------------------*/
void NintyNine::FreePlayers(void)
{
	for (int i = 0; i < NUM_PLAYERS; ++i)
	{
		delete players_[i];
	}
}


/*--------------------------------------------------------------------------*
Name:           Shuffle

Description:	Shuffle the deck.

Arguments:      None.

Returns:        None.
*---------------------------------------------------------------------------*/
void NintyNine::Shuffle(void)
{
	std::random_shuffle(deck_.begin(), deck_.end());

	deck_index_ = 0;
}

/*--------------------------------------------------------------------------*
Name:           DealCards

Description:	Deal cards to all players at beginning of the game.

Arguments:      None.

Returns:        None.
*---------------------------------------------------------------------------*/
void NintyNine::DealCards(void)
{
	for (int i = 0; i < NUM_PLAYERS; ++i)
	{
		// reset player status
		players_[i]->is_defeat_ = false;

		for (int j = 0; j < NUM_CARDS_PER_PLAYER; ++j)
		{
			// to simulate the fact that only if all 52 cards of the current deck are dealt
			// then open the next deck
			if (deck_index_ >= NUM_CARDS)
				Shuffle();

			players_[i]->hands_.emplace_back(deck_[deck_index_++]);
		}
	}
}

/*--------------------------------------------------------------------------*
Name:           PlayerTurn

Description:	A player plays a turn.

Arguments:      player_num: index of the player.

Returns:        true: only one player left. Game end condition.
				false: more than one players left.
*---------------------------------------------------------------------------*/
bool NintyNine::PlayerTurn(int player_num)
{
	if (!players_[player_num]->is_defeat_)
	{
		// this player deals a card, add running total
		// then check if there's a need to open the next deck to the pool
		// then player gets a card
		// then check if running total exceeds the limit
		// if so, the player loses

		int index = players_[player_num]->Play(total_);

		if (is_display_)
			printf("Player %d (%s) deals %c\n", player_num, DisplayAI(players_[player_num]->AI_), POKER::DisplayCard(players_[player_num]->hands_[index]->value_));

		AddRunningTotal(players_[player_num]->hands_[index]->value_);

		if (deck_index_ >= NUM_CARDS)
			Shuffle();

		players_[player_num]->hands_[index] = deck_[deck_index_++];

		// detect if the player loses
		CheckRunningTotal(player_num);
	}

	if (players_left_ <= 1)
		return true;

	return false;
}

/*--------------------------------------------------------------------------*
Name:           AddRunningTotal

Description:	Add card number to running total, also deals special cards

Arguments:      value: Card face value.

Returns:        None.
*---------------------------------------------------------------------------*/
void NintyNine::AddRunningTotal(int value)
{
	switch (value)
	{
	case SP_10:
		if (total_ > 90)
			total_ -= 10;
		else
			total_ += 10;
		break;

	case SP_20:
		if (total_ > 80)
			total_ -= 20;
		else
			total_ += 20;
		break;

	case SP_99:
		total_ = 99;
		break;

	case SP_REVERSE:
		is_order_increase_ = !is_order_increase_;
		break;

	case SP_SKIP:
		break;

	case SP_PICK:
		break;

	default:
		total_ += value;
		break;
	}
}

/*--------------------------------------------------------------------------*
Name:           CheckRunningTotal

Description:	Check if the player loses (running total higher than RUNNING_TOTAL_LIMIT).

Arguments:      player_num: Index of the player.

Returns:        None.
*---------------------------------------------------------------------------*/
void NintyNine::CheckRunningTotal(int player_num)
{
	// detect if the player loses
	if (total_ > RUNNING_TOTAL_LIMIT)
	{
		if (is_display_)
			printf("\nPlayer %d (%s) Loses!\n", player_num, DisplayAI(players_[player_num]->AI_));

		players_[player_num]->is_defeat_ = true;

		--players_left_;
		total_ = RUNNING_TOTAL_LIMIT;
	}
}

/*--------------------------------------------------------------------------*
Name:           NextPlayer

Description:	Get next player index.
				Depend on if the order is increasing or decreasing.

Arguments:      player_num: Index of the player.

Returns:        Next player index.
*---------------------------------------------------------------------------*/
int NintyNine::NextPlayer(int player_num)
{
	int num = 0;

	if (is_order_increase_)
	{
		num = player_num + 1;
		if (num == NUM_PLAYERS)
			num = 0;
	}
	else
	{
		num = player_num - 1;
		if (num < 0)
			num = NUM_PLAYERS - 1;
	}

	return num;
}
