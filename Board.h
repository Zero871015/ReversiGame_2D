// Name: Zero871015(B10615032)
// Date: 2018/09/18
// Last Update: 2018/09/20
// Problem statement: Board control, including record system.

#pragma once
class Reversi;	//Forward Declartion.

//To record all board and who's turn.
struct Record
{
	unsigned char board[8][8];
	int whosTurn;
};

class Board
{
public:
	Board();
	~Board();
	void NewGame();	//initialize.
	void Draw();	//print the board.
	void Save();	//record now board into array. 
	void Undo();	//go back the last state.
	void Redo();	//Undo the Undo.
	unsigned char cells[8][8];	//now board, 0 = blank, 1 = ¡´, 2 = ¡³.
	unsigned char cellPoints[8][8];	//every cell if you play, how many pieces you can get.
	int numW, numB;	//the numbers of black and white's pieces.
	int AIx, AIy;	//AI want to play where.
private:
	Record saves[64];	//the array to save the the record.
	int nowState;	//the saves' index.
};