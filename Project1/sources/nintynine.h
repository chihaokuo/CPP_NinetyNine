// Author: Chi-Hao Kuo
// Updated: 11/16/2014
// All content © 2014 DigiPen(USA) Corporation, all rights reserved.

// Ninty Nine Poker Game / simulations
// by tweaking the setting in nintynine.h, player can choose to play against AI
// or let AI simulate and grab data

#pragma once

#include <vector>

enum AI_ID
{
	AI_PLAYER = 0,
	AI_RANDOM = 1,
	AI_DUMB = 2,
	AI_SMART = 3,
};

// max face values of cards, default is 1~13
static const int NUM_VALUES			 = 13;
// number of cards, default is 52
static const int NUM_CARDS			 = 4 * NUM_VALUES;
// default limit of the game, when running total is higher than this value, player loses
static const int RUNNING_TOTAL_LIMIT = 99;

// number of players for the game
static const int NUM_PLAYERS			= 4;
// default number of cards each player has
static const int NUM_CARDS_PER_PLAYER	= 5;

// define AI

// set this to true to use PLAYER_AI[] to manually assign AI to each player
// set this to false to let pc picks AI by random
static const bool USE_PLAYER_AI_LIST = false;

// use this list to manually set the AI for each player
static const int PLAYER_AI[NUM_PLAYERS] = { AI_SMART, AI_SMART, AI_SMART, AI_SMART };

// or, use the following to let pc randomly pick AI by percentage

// set this to ture to assign a player to be human
static const bool AI_CHANCE_PLAYER		= false;
// percentage of AI to be random
static const int AI_CHANCE_RANDOM		= 0;	// out of 100
// percentage of AI to be dumb
static const int AI_CHANCE_DUMB			= 0;	// out of 100
// percentage of AI to be smart
static const int AI_CHANCE_SMART		= 100;	// out of 100

// number of games to be simulated
// (if allow human player, set this to 1)
static const int NUM_GAMES			= 10000;
// display detail info for each game?
// do net set this to true if there're many games played
static const int DISPLAY_GAME		= false;
// max number of rounds allowed for each game
// because this game might play too many rounds without getting result
static const int NUM_ROUNDS_LIMIT	= 100;
// max number of turns allowed for each game
// leave this one unchanged
static const int NUM_TURNS_LIMIT	= NUM_ROUNDS_LIMIT * NUM_PLAYERS;

namespace POKER
{
	// poker suits
	enum SUIT
	{
		SU_SPADE,
		SU_HEART,
		SU_DIAMOND,
		SU_CLUB,
		SU_NUMBER,
	};

	// special abilities, number means the face value of the card
	// if user wishes to remove certain special ability
	// just change the number to be more than max face value
	// (so there's no chance to be picked)
	enum SPECIAL
	{
		SP_NULL		= 0,	// normal card
		SP_10		= 10,	// increase/decrease 10
		SP_20		= 12,	// increase/decrease 20
		SP_99		= 13,	// to 99 instantly
		SP_REVERSE	= 4,	// reverse order
		SP_SKIP		= 11,	// skip order
		SP_PICK		= 5,	// pick another player
	};

	// card weight
	// used by smart AI, smart AI will prefer to keep card with higher weight value
	// and only use them when: 1. it's necessary to prevent losing 2. no other card can be dealt
	// special card weight should always has weight >= 1
	enum WEIGHT
	{
		SP_NULL_WEIGHT		= 0,	// normal card
		SP_10_WEIGHT		= 2,	// increase/decrease 10
		SP_20_WEIGHT		= 2,	// increase/decrease 20
		SP_99_WEIGHT		= 1,	// to 99 instantly
		SP_REVERSE_WEIGHT	= 1,	// reverse order
		SP_SKIP_WEIGHT		= 1,	// skip order
		SP_PICK_WEIGHT		= 1,	// pick another player
	};

	struct Cards
	{
		int value_;			// card value
		SUIT suit_;			// card suit
		int weight_;		// card weight

		// constructor
		Cards() : value_(0), suit_(SU_SPADE), weight_(0) {}
		// constructor
		Cards(int value, SUIT suit, int weight) 
			: value_(value), suit_(suit), weight_(weight) {}
	};

	typedef std::vector<Cards *> CardList;

	class Player
	{
	public:
		bool is_defeat_;	// flag on if the player is defeated
		CardList hands_;	// player hands
		int AI_;			// player AI (the strategy)

		// constructor
		Player(int ai) : is_defeat_(false), AI_(ai) {}

		// play game
		int Play(int total);

		// strategy

		// (AI 0) human player
		int PlayHuman(int total);
		// (AI 1) randomly choose a card
		int PlayRandom(void);
		// (AI 2) only pick the highest possible number of cards
		int PlayerDumb(int total);
		// (AI 3) use special cards when dealing with critical situation
		int PlayerSmart(int total);
	};

	typedef std::vector<Player *> PlayerList;

	struct GameStats
	{
		int num_games_;				// number games played
		int num_rounds_;			// total number of rounds
		int lowest_rounds_;			// lowest number of rounds of all games
		int highest_rounds_;		// highest number of rounds of all games
		int exceed_limit_rounds_;	// number of times that exceed round limit (play too long)
		int num_turns_;				// total number of rounds
		int lowest_turns_;			// lowest number of rounds of all games
		int highest_turns_;			// highest number of rounds of all games
		int exceed_limit_turns_;	// number of times that exceed round limit (play too long)

		GameStats() : num_games_(0), num_rounds_(0), lowest_rounds_(0), highest_rounds_(0), exceed_limit_rounds_(0),
			num_turns_(0), lowest_turns_(0), highest_turns_(0), exceed_limit_turns_(0) {}
		~GameStats() {}
	};

	class NintyNine
	{
	public:
		// constructor
		NintyNine(bool is_display);
		// destructor
		~NintyNine();

		// initialize
		void Initialize(void);
		// play a single game
		int Play(GameStats &stats);
		// free
		void Free(void);

		// Get a full deck
		CardList GetDeck(void);
		// Get running total
		int GetRunningTotal(void);
		// Get player list
		PlayerList GetPlayerList(void);

		// display deck
		void DisplayDeck(void);
		// display players' hands
		void DisplayHands(void);

	private:
		CardList cards_;			// total cards
		CardList deck_;				// one deck of cards
		int deck_index_;			// index of the current card of deck
		PlayerList players_;		// players
		int total_;					// running total number
		int players_left_;			// number of players left
		bool is_order_increase_;	// player order
		bool is_display_;			// display game info

		// create a deck of 52 cards
		void CreateDeck(void);
		// free deck
		void FreeDeck(void);
		// initialize players
		void InitPlayers(void);
		// free players
		void FreePlayers(void);

		// shuffle the deck
		void Shuffle(void);
		// deal cards to player
		void DealCards(void);
		// one player plays a turn, return true if there's only one player left
		bool PlayerTurn(int player_num);
		// add card number to running total, also deals special cards
		void AddRunningTotal(int value);
		// check if the player loses (running total higher than RUNNING_TOTAL_LIMIT)
		void CheckRunningTotal(int player_num);
		// get next player
		int NextPlayer(int player_num);
	};

	// Display card
	char DisplayCard(int value);
	// Display special cards
	void DisplaySpecial(const CardList list);
	// Display AI status
	char *DisplayAI(int ai);
}

