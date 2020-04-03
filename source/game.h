#ifndef _GAME_H_ 
#define _GAME_H_

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Timer.h"
#include <string>
#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 516;
const int SCREEN_HEIGHT = 516;
const int SCREEN_BPP = 32;

const int FPS = 80;

const int FLASH_TIME = 30;

const int SMALL_SIZE = 114;
const int BIG_SIZE = 126;
const int INTERVAL = 11;

const int GAME_ROW = 4;
const int GAME_COL = 4;

const int CARD_NUM = 12;

const int UP = 0;
const int LEFT = 1;
const int DOWN = 2;
const int RIGHT = 3;

SDL_Surface *screen;


SDL_Surface *load_image( std::string filename )
{
    SDL_Surface* loadedImage = NULL;

    SDL_Surface* optimizedImage = NULL;

    loadedImage = IMG_Load( filename.c_str() );

    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormat( loadedImage );

        SDL_FreeSurface( loadedImage );

        if( optimizedImage != NULL )
        {
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0, 0 ) );
        }
    }

    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination , SDL_Rect *clip = NULL )
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    if( screen == NULL )
    {
        return false;
    }
    
    SDL_WM_SetCaption( "2048", NULL );

    return true;
}

int random( int min, int max )
{
	return rand() % ( max - min + 1 ) + min;
}

#endif
