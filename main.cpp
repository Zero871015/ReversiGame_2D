/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <time.h>
#include "LTexture.h"
#include "LButton.h"
#include "Reversi.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Button constants
const int BUTTON_WIDTH = 50;
const int BUTTON_HEIGHT = 50;
const int TOTAL_BUTTONS = 64;

const int BUTTON_F_WIDTH = 200;
const int BUTTON_F_HEIGHT = 50;
const int TOTAL_F_BUTTONS = 6;

Reversi *reversi = Reversi::Instance();

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font *gFont = NULL;

SDL_Texture* Message[3];
SDL_Rect Message_rect[3]; //create a rect
SDL_Surface* surfaceMessage[3];

bool isFlip = false;

enum LButtonType
{
	BUTTON_TYPE_BLANK = 0,
	BUTTON_TYPE_WHITE = 1,
	BUTTON_TYPE_BLACK = 2,
	BUTTON_TYPE_NEWGAME = 3,
	BUTTON_TYPE_NEWGAMEAI = 4,
	BUTTON_TYPE_UNDO = 5,
	BUTTON_TYPE_REDO = 6,
	BUTTON_TYPE_FLIP = 7,
	BUTTON_TYPE_EXIT = 8,
	BUTTON_TYPE_TOTAL = 9
};

//Mouse button sprites
SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];
LTexture gButtonSpriteSheetTexture[BUTTON_TYPE_TOTAL];

//Buttons objects
LButton gButtons[TOTAL_BUTTONS];
LButton gFButtons[TOTAL_F_BUTTONS];

//The music that will be played
Mix_Music *gMusic = NULL;

bool LTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return mTexture != NULL;
}

bool init()
{
	//Initialization flag
	bool success = true;
	srand((unsigned)time(NULL));
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("Reversi_2D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprites
	if (!gButtonSpriteSheetTexture[0].loadFromFile("resourse/Blank.png"))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else if (!gButtonSpriteSheetTexture[1].loadFromFile("resourse/White.png"))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else if (!gButtonSpriteSheetTexture[2].loadFromFile("resourse/Black.png"))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else if (!gButtonSpriteSheetTexture[3].loadFromFile("resourse/NewGame.png"))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else if (!gButtonSpriteSheetTexture[4].loadFromFile("resourse/AI.png"))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else if (!gButtonSpriteSheetTexture[5].loadFromFile("resourse/UNDO.png"))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else if (!gButtonSpriteSheetTexture[6].loadFromFile("resourse/REDO.png"))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else if (!gButtonSpriteSheetTexture[7].loadFromFile("resourse/Flip.png"))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else if (!gButtonSpriteSheetTexture[8].loadFromFile("resourse/Exit.png"))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else
	{
		gMusic = Mix_LoadMUS("resourse/Fortnite_BattleBus Birthday Theme.mp3");
		if (gMusic == NULL)
		{
			printf("Failed to load music!\n");
			success = false;
		}
		gFont = TTF_OpenFont("resourse/kaiu.ttf",28);
		if (gFont == NULL)
		{
			printf("Failed to load Font!\n");
			success = false;
		}
		Message_rect[0].w = 150; // controls the width of the rect
		Message_rect[0].h = 40; // controls the height of the rect
		Message_rect[0].x = 10;  //controls the rect's x coordinate 
		Message_rect[0].y = SCREEN_HEIGHT- Message_rect[0].h; // controls the rect's y coordinte
		Message_rect[1].w = 150; // controls the width of the rect
		Message_rect[1].h = 40; // controls the height of the rect
		Message_rect[1].x = 10;  //controls the rect's x coordinate 
		Message_rect[1].y = SCREEN_HEIGHT - Message_rect[0].h * 2; // controls the rect's y coordinte
		Message_rect[2].w = 150; // controls the width of the rect
		Message_rect[2].h = 40; // controls the height of the rect
		Message_rect[2].x = 10;  //controls the rect's x coordinate 
		Message_rect[2].y = SCREEN_HEIGHT - Message_rect[0].h * 2; // controls the rect's y coordinte

		//Set sprites
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			gSpriteClips[i].x = 0;
			gSpriteClips[i].y = i * 50;
			gSpriteClips[i].w = BUTTON_WIDTH;
			gSpriteClips[i].h = BUTTON_HEIGHT;
		}
		for (int i = 0; i < TOTAL_BUTTONS; i++)
		{
			gButtons[i].BtnType = 0;
			gButtons[i].setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
		}

		for (int i = 0; i < TOTAL_F_BUTTONS; i++)
		{
			gFButtons[i].BtnType = i + 3;
			gFButtons[i].setSize(BUTTON_F_WIDTH, BUTTON_F_HEIGHT);
		}

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				gButtons[i * 8 + j].setPosition(230+50 * j, 40+50 * i);
			}
		}

		for (int i = 0; i < TOTAL_F_BUTTONS; i++)
		{
			gFButtons[i].setPosition(10, 40 + 60 * i);
		}
	}

	return success;
}

void close()
{
	//Free loaded images
	for (int i = 0; i < BUTTON_TYPE_TOTAL; i++)
	{
		gButtonSpriteSheetTexture[i].free();
	}

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	Mix_FreeMusic(gMusic);
	gMusic = NULL;
	//Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			Mix_PlayMusic(gMusic, 1);

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//Handle button events
					for (int i = 0; i < TOTAL_BUTTONS; ++i)
					{
						gButtons[i].handleEvent(&e,i);
					}
					for (int i = 0; i < TOTAL_F_BUTTONS; i++)
					{
						gFButtons[i].handleEvent(&e, i + TOTAL_BUTTONS);
					}
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render buttons
				for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
				{
					gSpriteClips[i].x = 0;
					gSpriteClips[i].y = i * 50;
					gSpriteClips[i].w = BUTTON_WIDTH;
					gSpriteClips[i].h = BUTTON_HEIGHT;
				}
				for (int i = 0; i < TOTAL_BUTTONS; ++i)
				{
					gButtons[i].BtnType = reversi->board.cells[i / 8][i % 8];
					gButtons[i].render();
				}
				for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
				{
					gSpriteClips[i].x = 0;
					gSpriteClips[i].y = i * 50;
					gSpriteClips[i].w = 200;
					gSpriteClips[i].h = BUTTON_HEIGHT;
				}
				for (int i = 0; i < TOTAL_F_BUTTONS; i++)
				{
					gFButtons[i].render();
				}

				if (isFlip)
				{
					for (int i = 0; i < TOTAL_BUTTONS; i++)
					{
						int x, y;
						x = gButtons[i].GetPosition().x + gButtons[i].rx;
						y = gButtons[i].GetPosition().y + gButtons[i].ry;
						gButtons[i].setPosition(x, y);
					}
				}
				else
				{
					for (int i = 0; i < 8; i++)
					{
						for (int j = 0; j < 8; j++)
						{
							gButtons[i * 8 + j].setPosition(230 + 50 * j, 40 + 50 * i);
						}
					}
				}

				reversi->UpdateScreen();

				SDL_Color Black = { 0, 0, 0 };
				if (reversi->isEnd)
				{
					std::string str;
					if (reversi->board.numB < reversi->board.numW)
						str = "White Win!";
					else if (reversi->board.numB > reversi->board.numW)
						str = "Black Win!";
					else
						str = "Draw!";
					surfaceMessage[2] = TTF_RenderText_Solid(gFont, str.c_str(), Black);
					Message[2] = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage[2]);
					SDL_RenderCopy(gRenderer, Message[2], NULL, &Message_rect[2]);
				}
				else
				{
					std::string str = "Black:" + std::to_string(reversi->board.numB);
					surfaceMessage[0] = TTF_RenderText_Solid(gFont, str.c_str(), Black);
					str = "White:" + std::to_string(reversi->board.numW);
					surfaceMessage[1] = TTF_RenderText_Solid(gFont, str.c_str(), Black);
					for (int i = 0; i < 2; i++)
					{
						Message[i] = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage[i]);
						SDL_RenderCopy(gRenderer, Message[i], NULL, &Message_rect[i]);
					}
				}

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}