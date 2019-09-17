#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <list>

#include "TIcTacToeMgr.h"

using namespace std;

bool TicTacToeMgr::StartGame()
{
	list<pair<int, time_t>> write_list; //플레이한 과정을 int와 시간의 pair로 저장하는 list 
	list<pair<int, time_t>> read_list;  //플레이한 파일을 불러와 저장하는 list

	//시간 초기화
	auto end = chrono::system_clock::now();
	auto start = chrono::system_clock::now();
	auto diff = end - start;
	auto msec = chrono::duration_cast<chrono::milliseconds>(diff);
	time_t t = msec.count();

	bool game_over = false;
	turn = 0;

	InitBoard();
	player1_marker = 'O';
	player2_marker = 'X';

	system("cls");
	cout << "★틱택토 게임을 시작합니다★" << endl << endl;

	current_player = player1_marker;

	int menu;
	cout << "메뉴를 선택하세요:(1:게임플레이 및 녹화, 2:재생) ";
	cin >> menu;

	//게임 및 녹화하기 
	if (menu == 1) {
		write_list.clear();
		string filename;
		cout << "저장할 파일의 이름을 입력하세요: ";
		cin >> filename;

		//게임플레이, 녹화시작 
		while (true)
		{
			//게임 상황을 체크하여 결과를 보여주고 게임 종료 
			if (CheckWinner() == true)
			{
				ShowBoard();
				if (current_player == player2_marker)
				{
					cout << "★Player1이 승리했습니다! 축하드립니다★" << endl;
				}
				else
				{
					cout << "★Player2가 승리했습니다! 축하드립니다★" << endl;
				}
				break;
			}
			else if (turn == 9)
			{
				ShowBoard();
				cout << "★무승부 입니다!★" << endl;
				break;

			}

			system("cls");
			ShowPlayer();
			ShowBoard();

			int row, col;

			//행,열과 시간값을 각각 페어로 리스트에 저장 
			start = chrono::system_clock::now();
			cout << "마커를 놓을 행을 입력하세요: ";
			cin >> row;
			end = chrono::system_clock::now();
			diff = end - start;
			msec = chrono::duration_cast<chrono::milliseconds>(diff);
			time_t t_row = msec.count();
			write_list.insert(write_list.end(), make_pair(row, t_row));

			start = chrono::system_clock::now();
			cout << "마커를 놓을 열을 입력하세요: ";
			cin >> col;
			end = chrono::system_clock::now();
			diff = end - start;
			msec = chrono::duration_cast<chrono::milliseconds>(diff);
			time_t t_col = msec.count();
			write_list.insert(write_list.end(), make_pair(col, t_col));

			//보드에 마커를 입력하고 플레이어를 체인지하며 턴을 증가시킴 
			InputMarkerInBoard(row, col);
			change_player();
			turn++;

			system("pause");
			system("cls");
		}
		//녹화 종료 후 저장
		ofstream out(filename + ".txt");
		for (auto d : write_list)
			out << d.first << "\t" << d.second << "\t";
		out.close();
	}

	//재생하기 
	else if (menu == 2)
	{
		//불러올 파일을 지정하여 리스트에 저장 
		string filename;
		cout << "불러올 파일 이름을 입력하세요:";
		cin >> filename;
		ifstream in(filename + ".txt");

		if (in.fail()) {
			cout << "파일이 없습니다." << endl;
			return false;
		}

		read_list.clear();
		int num;
		time_t playtime;
		while (!in.eof()) {
			in >> num >> playtime;
			read_list.insert(read_list.end(), make_pair(num, playtime));
		}
		in.close();

		bool check = false; //행입력인지 열입력인지 체크하는 변수 

		auto p = read_list.begin();
		t = 0;
		start = chrono::system_clock::now();
		int row, col;
		//재생 시작 
		while (p != read_list.end())
		{
			//게임 상황을 체크하여 결과를 보여주고 재생 종료 
			if (CheckWinner() == true)
			{
				system("cls");
				ShowBoard();
				if (current_player == player2_marker)
				{

					cout << "★Player1이 승리했습니다! 축하드립니다★" << endl;
				}
				else
				{
					cout << "★Player2가 승리했습니다! 축하드립니다★" << endl;
				}
				break;
			}
			else if (turn == 9)
			{
				system("cls");
				ShowBoard();
				cout << "★무승부 입니다!★" << endl;
				break;

			}
			//경과시간을 체크하여 t에 시간을 저장 
			end = chrono::system_clock::now();
			diff = end - start;
			msec = chrono::duration_cast<chrono::milliseconds>(diff);
			t = msec.count();

			//경과시간 t가 while문을 돌면서 읽어온 리스트의 시간값을 넘어서면 행값을 저장하고 
			//p를 증가시켜 리스트의 다음 페어로 넘어가고 t를 초기화하고 check변수를 바꿔 열값 저장으로 넘어감 
			if (t >= p->second && check == false) {
				system("cls");
				ShowPlayer();
				ShowBoard();
				row = p->first;
				p++;
				t = 0;
				check = true;
				start = chrono::system_clock::now();
			}
			//t가 list의 시간값을 넘어가면 열값을 저장하고 마커입력을 해주고 턴을 증가시키고 플레이어를 바꿔준다.
			else if (t >= p->second && check == true) {
				col = p->first;
				p++;
				t = 0;
				check = false;
				InputMarkerInBoard(row, col);
				turn++;
				change_player();
				start = chrono::system_clock::now();
			}

		}

	}
	else {
		cout << "잘못 입력했습니다! 다시 입력해주세요" << endl;
		system("pause");
		return false;
	}
	//리플레이 의사를 확인한다.
	while (true) {
		int replay;
		cout << "다시 플레이 하시겠습니까?(1:Replay, 2:Quit)";
		cin >> replay;
		if (replay == 1) {
			return false;
		}
		else if (replay == 2) {
			return true;
		}
		else {
			cout << "잘못 입력했습니다!" << endl;
			continue;
		}
	}

}

//플레이어를 교체하는 함수 
void TicTacToeMgr::change_player()
{
	if (current_player == player1_marker)
	{
		current_player = player2_marker;
	}
	else
	{
		current_player = player1_marker;
	}
}

//보드에 플레이어의 마커를 입력하는 함수 
bool TicTacToeMgr::InputMarkerInBoard(int row, int col)
{
	if (row - 1 < 0 || row - 1 > 2 || col - 1 < 0 || col - 1 > 2)
	{
		cout << "행과 열은 1~3까지만 가능합니다" << endl;
		cout << "다음 플레이어로 차례를 넘깁니다." << endl;
		return false;
	}
	if (board[row - 1][col - 1] == ' ')
	{
		board[row - 1][col - 1] = current_player;
	}
	else
	{
		cout << "그 자리는 입력할 수 없습니다" << endl;
		cout << "다음 플레이어에게 차례가 넘어갑니다." << endl;
		return false;
	}

	cout << "(" << row << ", " << col << ")에 입력했습니다." << endl;
	return true;
}

//게임보드를 초기화하는 함수 
void TicTacToeMgr::InitBoard()
{
	for (int row = 0; row < 3; ++row)
	{
		for (int col = 0; col < 3; ++col)
		{
			board[row][col] = ' ';
		}
	}
}

//게임 보드판을 보여주는 함수 
void TicTacToeMgr::ShowBoard()
{
	cout << " |1|2|3|" << endl;
	for (int row = 0; row < 3; ++row)
	{
		cout << " -------" << endl;
		cout << row + 1 << "|";
		for (int col = 0; col < 3; ++col)
		{
			cout << board[row][col] << "|";
		}
		cout << endl;
	}
	cout << " -------" << endl;
}

//플레이어의 턴을 보여주는 함수 
void TicTacToeMgr::ShowPlayer()
{
	system("cls");
	if (current_player == player1_marker)
		cout << "Player1의 차례입니다." << endl << endl;
	else if (current_player == player2_marker)
		cout << "Player2의 차례입니다." << endl << endl;
}

//승리를 판정하는 함수 
bool TicTacToeMgr::CheckWinner()
{
	//가로방향 판정
	for (int i = 0; i < 3; ++i)
	{
		if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2])
		{
			return true;
		}
	}

	//세로방향 판정
	for (int i = 0; i < 3; ++i)
	{
		if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i])
		{
			return true;
		}
	}

	//왼쪽 위에서 오른쪽 아래방향 대각선 판정 
	if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2])
	{
		return true;
	}

	//오른쪽 위에서 왼쪽 아래방향 대각선 판정 
	if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
	{
		return true;
	}

	return false;

}