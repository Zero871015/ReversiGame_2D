// Name: Zero871015(B10615032)
// Date: 2018/09/18
// Last Update: 2018/09/30
// Problem statement: Board control, including record system.

#include "Board.h"
#include "Reversi.h"
#include <iostream>
using namespace std;

extern int SearchAll();

Board::Board()
{
	NewGame();
}


Board::~Board()
{
}

void Board::NewGame()
{
	//Set all the cell is no piece.
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			this->cells[i][j] = 0;
		}
	}
	//Set the four pieces at game start.
	cells[3][3] = 1;
	cells[4][4] = 1;
	cells[3][4] = 2;
	cells[4][3] = 2;

	//initialize the saves.
	nowState = 0;
	for (int i = 0; i < 64; i++)
	{
		saves[i].whosTurn = 0;
	}

	Record temp;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			temp.board[i][j] = this->cells[i][j];
		}
	}
	temp.whosTurn = 1;
	saves[0] = temp;
	for (int i = 0; i < 64; ++i)
	{
		gButtons[i].BtnType = cells[i / 8][i % 8];
		gButtons[i].render();
	}
}

void Board::Draw()
{
	numW = 0, numB = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (this->cells[i][j] == 0)
			{

			}
				//cout << "¡Ð";
			else if (this->cells[i][j] == 1)
			{
				//cout << "¡´";
				numW++;
			}
			else if (this->cells[i][j] == 2)
			{
				//cout << "¡³";
				numB++;
			}
		}
		//cout << char('A' + i) << " " << Reversi::Instance()->BtnAH[i] << endl;
	}
	//cout << "1 2 3 4 5 6 7 8" << endl;
	for (int i = 0; i < 8; i++)
	{
		//cout << Reversi::Instance()->Btn18[i]<<" ";
	}
	//cout << endl << endl;

	//cout << "¡´¡G" << numW << "  ¡³¡G" << numB << endl;
}

void Board::Save()
{
	this->nowState++;
	Record temp;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			temp.board[i][j] = this->cells[i][j];
		}
	}
	temp.whosTurn = Reversi::Instance()->whosTurn;
	saves[nowState] = temp;
	saves[nowState+1].whosTurn = 0;
}

void Board::Undo()
{
	SearchAll();
	if (nowState == 0)return;
	nowState--;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			cells[i][j] = saves[nowState].board[i][j];
		}
	}
	for (int i = 0; i < 64; ++i)
	{
		gButtons[i].BtnType = cells[i / 8][i % 8];
		gButtons[i].render();
	}
	Reversi::Instance()->whosTurn = saves[nowState].whosTurn;
}

void Board::Redo()
{
	SearchAll();
	if (saves[nowState+1].whosTurn == 0)return;
	nowState++;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			cells[i][j] = saves[nowState].board[i][j];
		}
	}
	Reversi::Instance()->whosTurn = saves[nowState].whosTurn;
}
