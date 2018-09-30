// Name: Zero871015(B10615032)
// Date: 2018/09/18
// Last Update: 2018/09/20
// Problem statement: Reversi Game System

#pragma once
#include "Board.h"
class Reversi
{
public:
	static Reversi* Instance();	//Get instance(singleton pattern)
	~Reversi();
	void UpdateScreen();		//Refresh
	void NewGame();	//initialize.
	void AIGame();	//initialize and open AI.
	bool BtnAH[8],Btn18[8];	//key event bool.
	void BtnTrigger();	//key event trigger.
	void Move(int i, int j);	//Play one piece.
	friend bool Play(int x, int y, int deltaX, int deltaY, bool isFirst);	//recusive function to check 8 directions.
	friend int SearchAll();	//check all cells to update the cell point.
	friend bool SearchCell(int x, int y, int deltaX, int deltaY, bool isFirst,int &point);	//recusive function to check 8 directions.
	void AIPlay();
	int whosTurn;
	Board board;	//Chessboard Controler
	bool isEnd;
private:
	bool AImode;
	Reversi();
	static Reversi* _instance;	//Singleton
};