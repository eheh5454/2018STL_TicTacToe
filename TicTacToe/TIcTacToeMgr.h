#include <windows.h>

class TicTacToeMgr
{
public:
	bool StartGame();
	void InitBoard();
	void ShowBoard();
	bool InputMarkerInBoard(int row, int col);
	void change_player();
	bool CheckWinner();
	void ShowPlayer();


private:
	char board[3][3];
	char player1_marker;
	char player2_marker;
	char current_player;
	int turn;
};