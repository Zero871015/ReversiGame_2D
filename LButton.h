// Name: Zero871015 (B10615032)
// Date: 2018/09/30
// Last Update: 2018/09/30
// Problem statement: SDL Button Class
//-----------------------------------------------------------------
/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/
//-----------------------------------------------------------------

#pragma once
#include <SDL.h>
#include <string>
#include <SDL_image.h>
#include "LTexture.h"


enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

extern LTexture gButtonSpriteSheetTexture[10];
extern SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];
extern bool isFlip;

//The mouse button
class LButton
{
public:
	//Initializes internal variables
	LButton();

	//Sets top left position
	void setPosition(int x, int y);

	//Handles mouse event
	void handleEvent(SDL_Event* e,int i);

	//Shows button sprite
	void render();

	void setSize(int w, int h);

	int BtnType;

	SDL_Point GetPosition();

	int rx, ry;
private:
	//Top left position
	SDL_Point mPosition;

	//Currently used global sprite
	LButtonSprite mCurrentSprite;

	int width;
	int height;
};
