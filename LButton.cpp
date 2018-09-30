// Name: Zero871015 (B10615032)
// Date: 2018/09/30
// Last Update: 2018/09/30
// Problem statement: SDL Button Class
//-----------------------------------------------------------------
/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/
//-----------------------------------------------------------------


#include "LButton.h"
#include "Reversi.h"
LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
	rx = (rand() % 5) - 2;
	ry = (rand() % 5) - 2;
}

void LButton::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::handleEvent(SDL_Event* e,int i)
{
	Reversi *reversi = Reversi::Instance();
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if (x < mPosition.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > mPosition.x + this->width)
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < mPosition.y)
		{
			inside = false;
		}
		//Mouse below the button
		else if (y > mPosition.y + this->height)
		{
			inside = false;
		}

		//Mouse is outside button
		if (!inside)
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;

			case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;

			case SDL_MOUSEBUTTONUP:
				if (i < 64)
				{
					reversi->Btn18[i % 8] = true;
					reversi->BtnAH[i / 8] = true;
					reversi->BtnTrigger();
					reversi->Btn18[i % 8] = false;
					reversi->BtnAH[i / 8] = false;
				}
				else if (i == 64)
				{
					isFlip = false;
					reversi->NewGame();
				}
				else if (i == 65)
				{
					isFlip = false;
					reversi->AIGame();
				}
				else if (i == 66)
				{
					reversi->board.Undo();
				}
				else if (i == 67)
				{
					reversi->board.Redo();
				}
				else if (i == 68)
				{
					isFlip = true;
				}
				else if (i == 69)
				{
					e->type = SDL_QUIT;
				}
				mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				break;
			}
		}
	}
}

void LButton::render()
{
	//Show current button sprite
	gButtonSpriteSheetTexture[this->BtnType].render(mPosition.x, mPosition.y, &gSpriteClips[mCurrentSprite]);
}

void LButton::setSize(int w, int h)
{
	this->width = w;
	this->height = h;
}

SDL_Point LButton::GetPosition()
{
	return this->mPosition;
}
