#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <list>

#include "TIcTacToeMgr.h"

using namespace std;

bool TicTacToeMgr::StartGame()
{
	list<pair<int, time_t>> write_list; //�÷����� ������ int�� �ð��� pair�� �����ϴ� list 
	list<pair<int, time_t>> read_list;  //�÷����� ������ �ҷ��� �����ϴ� list

	//�ð� �ʱ�ȭ
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
	cout << "��ƽ���� ������ �����մϴ١�" << endl << endl;

	current_player = player1_marker;

	int menu;
	cout << "�޴��� �����ϼ���:(1:�����÷��� �� ��ȭ, 2:���) ";
	cin >> menu;

	//���� �� ��ȭ�ϱ� 
	if (menu == 1) {
		write_list.clear();
		string filename;
		cout << "������ ������ �̸��� �Է��ϼ���: ";
		cin >> filename;

		//�����÷���, ��ȭ���� 
		while (true)
		{
			//���� ��Ȳ�� üũ�Ͽ� ����� �����ְ� ���� ���� 
			if (CheckWinner() == true)
			{
				ShowBoard();
				if (current_player == player2_marker)
				{
					cout << "��Player1�� �¸��߽��ϴ�! ���ϵ帳�ϴ١�" << endl;
				}
				else
				{
					cout << "��Player2�� �¸��߽��ϴ�! ���ϵ帳�ϴ١�" << endl;
				}
				break;
			}
			else if (turn == 9)
			{
				ShowBoard();
				cout << "�ڹ��º� �Դϴ�!��" << endl;
				break;

			}

			system("cls");
			ShowPlayer();
			ShowBoard();

			int row, col;

			//��,���� �ð����� ���� ���� ����Ʈ�� ���� 
			start = chrono::system_clock::now();
			cout << "��Ŀ�� ���� ���� �Է��ϼ���: ";
			cin >> row;
			end = chrono::system_clock::now();
			diff = end - start;
			msec = chrono::duration_cast<chrono::milliseconds>(diff);
			time_t t_row = msec.count();
			write_list.insert(write_list.end(), make_pair(row, t_row));

			start = chrono::system_clock::now();
			cout << "��Ŀ�� ���� ���� �Է��ϼ���: ";
			cin >> col;
			end = chrono::system_clock::now();
			diff = end - start;
			msec = chrono::duration_cast<chrono::milliseconds>(diff);
			time_t t_col = msec.count();
			write_list.insert(write_list.end(), make_pair(col, t_col));

			//���忡 ��Ŀ�� �Է��ϰ� �÷��̾ ü�����ϸ� ���� ������Ŵ 
			InputMarkerInBoard(row, col);
			change_player();
			turn++;

			system("pause");
			system("cls");
		}
		//��ȭ ���� �� ����
		ofstream out(filename + ".txt");
		for (auto d : write_list)
			out << d.first << "\t" << d.second << "\t";
		out.close();
	}

	//����ϱ� 
	else if (menu == 2)
	{
		//�ҷ��� ������ �����Ͽ� ����Ʈ�� ���� 
		string filename;
		cout << "�ҷ��� ���� �̸��� �Է��ϼ���:";
		cin >> filename;
		ifstream in(filename + ".txt");

		if (in.fail()) {
			cout << "������ �����ϴ�." << endl;
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

		bool check = false; //���Է����� ���Է����� üũ�ϴ� ���� 

		auto p = read_list.begin();
		t = 0;
		start = chrono::system_clock::now();
		int row, col;
		//��� ���� 
		while (p != read_list.end())
		{
			//���� ��Ȳ�� üũ�Ͽ� ����� �����ְ� ��� ���� 
			if (CheckWinner() == true)
			{
				system("cls");
				ShowBoard();
				if (current_player == player2_marker)
				{

					cout << "��Player1�� �¸��߽��ϴ�! ���ϵ帳�ϴ١�" << endl;
				}
				else
				{
					cout << "��Player2�� �¸��߽��ϴ�! ���ϵ帳�ϴ١�" << endl;
				}
				break;
			}
			else if (turn == 9)
			{
				system("cls");
				ShowBoard();
				cout << "�ڹ��º� �Դϴ�!��" << endl;
				break;

			}
			//����ð��� üũ�Ͽ� t�� �ð��� ���� 
			end = chrono::system_clock::now();
			diff = end - start;
			msec = chrono::duration_cast<chrono::milliseconds>(diff);
			t = msec.count();

			//����ð� t�� while���� ���鼭 �о�� ����Ʈ�� �ð����� �Ѿ�� �ప�� �����ϰ� 
			//p�� �������� ����Ʈ�� ���� ���� �Ѿ�� t�� �ʱ�ȭ�ϰ� check������ �ٲ� ���� �������� �Ѿ 
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
			//t�� list�� �ð����� �Ѿ�� ������ �����ϰ� ��Ŀ�Է��� ���ְ� ���� ������Ű�� �÷��̾ �ٲ��ش�.
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
		cout << "�߸� �Է��߽��ϴ�! �ٽ� �Է����ּ���" << endl;
		system("pause");
		return false;
	}
	//���÷��� �ǻ縦 Ȯ���Ѵ�.
	while (true) {
		int replay;
		cout << "�ٽ� �÷��� �Ͻðڽ��ϱ�?(1:Replay, 2:Quit)";
		cin >> replay;
		if (replay == 1) {
			return false;
		}
		else if (replay == 2) {
			return true;
		}
		else {
			cout << "�߸� �Է��߽��ϴ�!" << endl;
			continue;
		}
	}

}

//�÷��̾ ��ü�ϴ� �Լ� 
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

//���忡 �÷��̾��� ��Ŀ�� �Է��ϴ� �Լ� 
bool TicTacToeMgr::InputMarkerInBoard(int row, int col)
{
	if (row - 1 < 0 || row - 1 > 2 || col - 1 < 0 || col - 1 > 2)
	{
		cout << "��� ���� 1~3������ �����մϴ�" << endl;
		cout << "���� �÷��̾�� ���ʸ� �ѱ�ϴ�." << endl;
		return false;
	}
	if (board[row - 1][col - 1] == ' ')
	{
		board[row - 1][col - 1] = current_player;
	}
	else
	{
		cout << "�� �ڸ��� �Է��� �� �����ϴ�" << endl;
		cout << "���� �÷��̾�� ���ʰ� �Ѿ�ϴ�." << endl;
		return false;
	}

	cout << "(" << row << ", " << col << ")�� �Է��߽��ϴ�." << endl;
	return true;
}

//���Ӻ��带 �ʱ�ȭ�ϴ� �Լ� 
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

//���� �������� �����ִ� �Լ� 
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

//�÷��̾��� ���� �����ִ� �Լ� 
void TicTacToeMgr::ShowPlayer()
{
	system("cls");
	if (current_player == player1_marker)
		cout << "Player1�� �����Դϴ�." << endl << endl;
	else if (current_player == player2_marker)
		cout << "Player2�� �����Դϴ�." << endl << endl;
}

//�¸��� �����ϴ� �Լ� 
bool TicTacToeMgr::CheckWinner()
{
	//���ι��� ����
	for (int i = 0; i < 3; ++i)
	{
		if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2])
		{
			return true;
		}
	}

	//���ι��� ����
	for (int i = 0; i < 3; ++i)
	{
		if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i])
		{
			return true;
		}
	}

	//���� ������ ������ �Ʒ����� �밢�� ���� 
	if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2])
	{
		return true;
	}

	//������ ������ ���� �Ʒ����� �밢�� ���� 
	if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
	{
		return true;
	}

	return false;

}