// Author: Chi-Hao Kuo
// Updated: 11/16/2014
// All content © 2014 DigiPen(USA) Corporation, all rights reserved.

// Ninty Nine Poker Game / simulations
// by tweaking the setting in nintynine.h, player can choose to play against AI
// or let AI simulate and grab data

#include "nintynine.h"

using namespace POKER;

/*--------------------------------------------------------------------------*
Name:           main

Description:    Main function.

Arguments:      None.

Returns:        0.
*---------------------------------------------------------------------------*/
int main(void)
{
	// store stats from simulations
	GameStats stats;
	NintyNine game(DISPLAY_GAME);

	// initialize
	game.Initialize();

	// play games
	int win_player[NUM_PLAYERS] = { 0 };
	{
		for (int i = 0; i < NUM_GAMES; ++i)
		{
			int winner = game.Play(stats);
			++stats.num_games_;
			++win_player[winner];
		}
	}

	// display result
	printf("\n\nNumber of games: %d\n\n", stats.num_games_);

	printf("Average number of rounds: %.2f\n", (float)stats.num_rounds_ / (float)stats.num_games_);
	printf("Highest number of rounds: %d\n", stats.highest_rounds_);
	printf("Lowest number of rounds: %d\n", stats.lowest_rounds_);
	printf("Times that exceed %d rounds: %d\n\n", NUM_ROUNDS_LIMIT, stats.exceed_limit_rounds_);

	printf("Average number of turns: %.2f\n", (float)stats.num_turns_ / (float)stats.num_games_);
	printf("Highest number of turns: %d\n", stats.highest_turns_);
	printf("Lowest number of turns: %d\n", stats.lowest_turns_);
	printf("Times that exceed %d turns: %d\n\n", NUM_TURNS_LIMIT, stats.exceed_limit_turns_);

	PlayerList playerlist = game.GetPlayerList();
	for (int i = 0; i < NUM_PLAYERS; ++i)
		printf("Player %d (%s) won %d times.\n", i, DisplayAI(playerlist[i]->AI_), win_player[i]);

	// free
	game.Free();

	return 0;
}