// Name: Zero871015(B10615032)
// Date: 2018/09/18
// Last Update: 2018/09/20
// Problem statement: Reversi Game System

#include "Reversi.h"
#include <iostream>
using namespace std;

#define BLACK 1
#define WHITE 2

Reversi::Reversi()
{
	NewGame();
}

Reversi* Reversi::_instance = 0;

Reversi* Reversi::Instance()
{
	if (_instance == 0)
		_instance = new Reversi;
	return _instance;
}

Reversi::~Reversi(void)
{
}

void Reversi::UpdateScreen()
{
	system("CLS");
	this->board.Draw();
	/*
	//Turn tip
	cout << endl << "Now¡G";
	if (whosTurn == BLACK)
		cout << "¡´" << endl;
	else if (whosTurn == WHITE)
		cout << "¡³" << endl;

	//AI tip
	if (AImode)
	{
		cout << endl << "¡°This is AI mode." << endl;
	}
	cout << endl;
	//Key tips
	cout << "NewGame¡G\tEnter" << endl;
	cout << "NewGame(AI)¡G\tCtrl" << endl;
	cout << "Undo¡G\t\tBackspace" << endl;
	cout << "Redo¡G\t\tTab" << endl;
	*/
	if (board.numB == 0 || board.numW == 0 || board.numB + board.numW == 64)
		isEnd = true;
	else
		isEnd = false;

	//Winner tip
	/*
	if (isEnd)
	{
		if (board.numB < board.numW)
			cout << "¡´ Win!" << endl;
		else if (board.numB > board.numW)
			cout << "¡³ Win!" << endl;
		else
			cout << "Draw" << endl;
	}
	*/
}

void Reversi::NewGame()
{
	this->AImode = false;
	this->isEnd = false;
	this->whosTurn = BLACK;
	this->board.NewGame();
	for (int i = 0; i < 8; i++)
	{
		Btn18[i] = false;
		BtnAH[i] = false;
	}
}

void Reversi::AIGame()
{
	this->AImode = true;
	this->isEnd = false;
	this->whosTurn = BLACK;
	this->board.NewGame();
	for (int i = 0; i < 8; i++)
	{
		Btn18[i] = false;
		BtnAH[i] = false;
	}
}

void Reversi::BtnTrigger()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (BtnAH[i] && Btn18[j])
			{
				Move(i, j);
				return;
			}
		}
	}
}
bool Play(int x, int y, int deltaX, int deltaY, bool isFirst)
{
	Reversi *reversi = Reversi::Instance();
	x += deltaX;
	y += deltaY;
	if (x < 0 || x>7 || y < 0 || y > 7) return false;

	if (isFirst)
	{
		if (reversi->board.cells[x][y] == reversi->whosTurn)
			return false;
		else if (reversi->board.cells[x][y] == 0)
			return false;
		else if (Play(x, y, deltaX, deltaY, false))
		{
			reversi->board.cells[x][y] = reversi->whosTurn;
			return true;
		}
		else
			return false;
	}
	else
	{
		if (reversi->board.cells[x][y] == reversi->whosTurn)
		{
			reversi->board.cells[x][y] = reversi->whosTurn;
			return true;
		}
		else if (reversi->board.cells[x][y] == 0)
			return false;
		else if (Play(x, y, deltaX, deltaY, false))
		{
			reversi->board.cells[x][y] = reversi->whosTurn;
			return true;
		}
		else
			return false;
	}
}
bool SearchCell(int x, int y, int deltaX, int deltaY, bool isFirst, int &point)
{
	Reversi *reversi = Reversi::Instance();
	x += deltaX;
	y += deltaY;
	if (x < 0 || x>7 || y < 0 || y > 7) return false;

	if (isFirst)
	{
		if (reversi->board.cells[x][y] == reversi->whosTurn)
			return false;
		else if (reversi->board.cells[x][y] == 0)
			return false;
		else if (SearchCell(x, y, deltaX, deltaY, false, point))
		{
			point++;
			return true;
		}
		else
			return false;
	}
	else
	{
		if (reversi->board.cells[x][y] == reversi->whosTurn)
		{
			reversi->board.cells[x][y] = reversi->whosTurn;
			return true;
		}
		else if (reversi->board.cells[x][y] == 0)
			return false;
		else if (SearchCell(x, y, deltaX, deltaY, false, point))
		{
			point++;
			return true;
		}
		else
			return false;
	}
}

int SearchAll()
{
	Reversi *reversi = Reversi::Instance();
	int maxPoint = 0;
	int total = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (reversi->board.cells[i][j] != 0)
			{
				reversi->board.cellPoints[i][j] = 0;
				continue;
			}
			int sum = 0;
			SearchCell(i, j, -1, -1, 1, sum);
			SearchCell(i, j, 0, -1, 1, sum);
			SearchCell(i, j, 1, -1, 1, sum);
			SearchCell(i, j, -1, 0, 1, sum);
			SearchCell(i, j, 1, 0, 1, sum);
			SearchCell(i, j, -1, 1, 1, sum);
			SearchCell(i, j, 0, 1, 1, sum);
			SearchCell(i, j, 1, 1, 1, sum);
			reversi->board.cellPoints[i][j] = sum;
			if (maxPoint < sum)
			{
				maxPoint = sum;
				reversi->board.AIx = i;
				reversi->board.AIy = j;
			}
			total += sum;
		}
	}
	return total;
}

void Reversi::Move(int i, int j)
{
	bool temp = false;
	if (board.cells[i][j] != 0) return;
	else
	{
		temp |= Play(i, j, -1, -1, 1);
		temp|= Play(i, j, 0, -1, 1);
		temp|= Play(i, j, 1, -1, 1);
		temp|= Play(i, j, -1, 0, 1);
		temp|= Play(i, j, 1, 0, 1);
		temp|= Play(i, j, -1, 1, 1);
		temp|= Play(i, j, 0, 1, 1);
		temp|= Play(i, j, 1, 1, 1);
	}
	if (temp)
	{
		board.cells[i][j] = whosTurn;
		whosTurn = whosTurn == BLACK ? WHITE : BLACK;
		
		if(SearchAll()==0)	//someone can not play any piece
			whosTurn = whosTurn == BLACK ? WHITE : BLACK;
			
		UpdateScreen();
		if (AImode&&whosTurn == WHITE)
		{
			AIPlay();
		}
		else
			board.Save();
	}
}

void Reversi::AIPlay()
{
	Reversi *reversi = Reversi::Instance();
	reversi->Move(reversi->board.AIx, reversi->board.AIy);
}



