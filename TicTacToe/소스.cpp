#include "TIcTacToeMgr.h"

int main()
{
	system("title Tic Tac Toe");

	system("mode con: lines=25 cols=80");

	TicTacToeMgr game_mgr;

	bool game_end = false;

	while (game_end == false)
	{
		game_end = game_mgr.StartGame();
	}

}