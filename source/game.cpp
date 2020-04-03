/********************2048******************************************* 
程序：2048 
作者：@BOYFRIEND OF XINXIN 
时间：2017/2/1 
********************************************************************/
#include "game.h"

struct Set{															// 
	int small;
	int big;
}set[4] = { {12, 2}, { 138, 130}, { 264, 258}, {390, 386}};

int X[4] = { -1, 0, 1, 0 };											// 四个方向的增量 
int Y[4] = { 0, -1, 0, 1 }; 

class Game{
	private:
		bool end;
		int dir;
		int space;
		bool canCreat;
		int map[GAME_ROW][GAME_ROW];
		int flash[GAME_ROW][GAME_ROW];
		SDL_Surface *background;
		SDL_Surface *small;
		SDL_Surface *big;
		SDL_Surface *over;
		SDL_Rect smallClip[CARD_NUM];
		SDL_Rect bigClip[CARD_NUM];
		SDL_Rect mouseDown;
		SDL_Rect mouseUp;
		SDL_Rect exit;
		SDL_Rect restart;
		Timer fps;
	public:
		Game();
		void handle_input( const SDL_Event &event );
		void show_game();
		void show_num(int i, int j);
		bool show_end();
		void creat_card();
		int rand_num();
		void move();
		bool is_end();
		void gameRun();
		int judge_dir();
		void control_fps();
};

Game::Game()
{
	background = load_image( "..\\pic\\background.png" );
	small = load_image( "..\\pic\\small_num.png" );
	big =load_image( "..\\pic\\big_num.png" );
	over = load_image( "..\\pic\\gameover.png" );
	
	end = false;
	canCreat = false;
	dir = -1;
	space = 16;
	
	for(int i = 0; i < CARD_NUM; i++)
	{
		smallClip[i].x = SMALL_SIZE * ( i % 6 );
		smallClip[i].y = SMALL_SIZE * ( i / 6 );
		smallClip[i].w = SMALL_SIZE;
		smallClip[i].h = SMALL_SIZE;
		bigClip[i].x = BIG_SIZE * ( i % 6 );
		bigClip[i].y = BIG_SIZE * ( i / 6 );
		bigClip[i].w = BIG_SIZE;
		bigClip[i].h = BIG_SIZE;
	}
	for(int i = 0; i < GAME_ROW; i++)
	{
		for(int j = 0; j < GAME_ROW; j++)
		{
			map[i][j] = -1;
			flash[i][j] = -1;
		}
	}
	exit.x = 48 + (SCREEN_WIDTH - over->w) / 2;
	exit.y = 132 + (SCREEN_HEIGHT - over->h) / 2;
	exit.w = 110;
	exit.h = 36;
	restart.x = 222 + (SCREEN_WIDTH - over->w) / 2;
	restart.y = 132 + (SCREEN_HEIGHT - over->h) / 2;
	restart.w = 110;
	restart.h = 36;
	creat_card();
}

void Game::show_game()
{
	apply_surface( 0, 0, background, screen );
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			show_num(i, j);
		}
	}
	SDL_Flip( screen );
}

void Game::show_num( int i, int j )
{
	if( map[i][j] >= 0 )
	{
		if( flash[i][j] >= 0 )
		{
			apply_surface( set[j].big, set[i].big, big, screen, &bigClip[ map[i][j] ] );
			flash[i][j]--;
		}
		else
		{
			apply_surface( set[j].small, set[i].small, small, screen, &smallClip[ map[i][j] ] );
		}
	}
}

bool Game::show_end()
{
	apply_surface( (SCREEN_WIDTH - over->w) / 2, (SCREEN_HEIGHT - over->h) / 2, over, screen);
	SDL_Flip( screen );
	bool click = false;
	bool quit = false;
	SDL_Event event;
	
	while( !quit )
	{
		if( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_MOUSEBUTTONDOWN )
			{
				if( event.button.button == SDL_BUTTON_LEFT )
				{
					int x = event.button.x;
					int y = event.button.y;
					if( x >= exit.x && x <= exit.x + exit.w && y >= exit.y && y <= exit.y + exit.h )
					{
						return true;
					}
					if( x >= restart.x && x <= restart.x + restart.w && y >= restart.y && y <= restart.y + restart.h )
					{
						return false;
					}
				}
			}
		}
	}
}

int Game::rand_num()
{
	int x = random( 0, 2 );
	if( x == 0 )
	{
		return 1;
	}
	return 0;
}

void Game::creat_card()
{	
	if( space <= 0 ) return;
	
	int x = random( 0, 3 );
	int y = random( 0, 3 );
	while( map[x][y] != -1 )
	{
		x = random( 0, 3 );
		y = random( 0, 3 );
	}
	map[x][y] = rand_num();
	flash[x][y] = FLASH_TIME;
	space--;
	SDL_Delay( 1000 / FPS );
}

void Game::handle_input( const SDL_Event &event )
{
	if( event.type == SDL_KEYDOWN )
	{
		switch( event.key.keysym.sym )
		{
			case SDLK_UP: dir = UP; break;
			case SDLK_LEFT: dir = LEFT; break;
			case SDLK_DOWN: dir = DOWN; break;
			case SDLK_RIGHT: dir = RIGHT; break;
		}
	}
	else if( event.type == SDL_MOUSEBUTTONDOWN )
	{
		if( event.button.button == SDL_BUTTON_LEFT )
		{
			mouseDown.x = event.button.x;
			mouseDown.y = event.button.y;
		}
	}
	else if( event.type == SDL_MOUSEBUTTONUP )
	{
		if( event.button.button == SDL_BUTTON_LEFT )
		{
			mouseUp.x = event.button.x;
			mouseUp.y = event.button.y;
			judge_dir();
		}
	}
	else if( event.type == SDL_QUIT )
	{
		end = true;
	}
}

int Game::judge_dir()
{
	if( mouseUp.x < mouseDown.x )
	{
		if( mouseUp.y < mouseDown.y )					
		{
			if( mouseUp.y - mouseDown.y < mouseUp.x - mouseDown.x )
			{
				dir = UP;
			} 
			else
			{
				dir = LEFT;
			}
		}
		else											 
		{
			if( mouseUp.y - mouseDown.y > mouseDown.x - mouseUp.x )
			{
				dir = DOWN;
			} 
			else
			{
				dir = LEFT;
			}
		}
	}
	else
	{
		if( mouseUp.y < mouseDown.y )						
		{
			if( mouseUp.y - mouseDown.y < mouseDown.x - mouseUp.x )
			{
				dir = UP;
			} 
			else
			{
				dir = RIGHT;
			}
		}
		else											
		{
			if( mouseUp.y - mouseDown.y > mouseUp.x - mouseDown.x )
			{
				dir = DOWN;
			} 
			else
			{
				dir = RIGHT;
			}
		}
	}
}

void Game::move()
{
	if( dir < 0 ) return;
	
	switch( dir )
	{
		case UP:{
			for(int p = 0; p < 4; p++)
			{
				for(int i = 0; i < 3; i++)
				{
					for(int j = 0; j < GAME_ROW; j++)
					{
						if( map[i][j] == -1 )
						{
							if( map[i+1][j] != -1 )
							{
								map[i][j] = map[i+1][j];
								map[i+1][j] = -1;
								canCreat = true;
								show_game();
							}
						}
						else if( map[i][j] == map[i+1][j] && p == 3 )
						{
							map[i][j]++;
							if( map[i][j] >= 12 )
							{
								map[i][j]--;
								end = true;
								return;
							}
							flash[i][j] = FLASH_TIME;
							map[i+1][j] = -1;
							space++;
							canCreat = true;
							show_game();
						}
					}
				}
			}
			break;
		}
		case LEFT:{
			for(int p = 0; p < 4; p++)
			{
				for(int i = 0; i < 3; i++)
				{
					for(int j = 0; j < GAME_ROW; j++)
					{
						if( map[j][i] == -1 )
						{
							if( map[j][i+1] != -1 )
							{
								map[j][i] = map[j][i+1];
								map[j][i+1] = -1;
								canCreat = true;
								show_game();
							}	
						}
						else if( map[j][i] == map[j][i+1] && p == 3 )
						{
							map[j][i]++;
							if( map[j][i] >= 12 )
							{
								map[j][i]--;
								end = true;
								return;
							}
							flash[j][i] = FLASH_TIME;
							map[j][i+1] = -1;
							space++;
							canCreat = true;
							show_game();
						}
					}
				}
			}
			break;
		}
		case DOWN:{
			for(int p = 0; p < 4; p++)
			{
				for(int i = 3; i > 0; i--)
				{
					for(int j = 0; j < GAME_ROW; j++)
					{
						if( map[i][j] == -1 )
						{
							if( map[i-1][j] != -1 )
							{
								map[i][j] = map[i-1][j];
								map[i-1][j] = -1;
								canCreat = true;
								show_game();
							}	
						}
						else if( map[i][j] == map[i-1][j] && p == 3 )
						{
							map[i][j]++;
							if( map[i][j] >= 12 )
							{
								map[i][j]--;
								end = true;
								return;
							}
							flash[i][j] = FLASH_TIME;
							map[i-1][j] = -1;
							space++;
							canCreat = true;
							show_game();
						}
					}
				}
			}
			break;
		}
		case RIGHT:{
			for(int p = 0; p < 4; p++)
			{
				for(int i = 3; i > 0; i--)
				{
					for(int j = 0; j < GAME_ROW; j++)
					{
						if( map[j][i] == -1 )
						{
							if( map[j][i-1] != -1 )
							{
								map[j][i] = map[j][i-1];
								map[j][i-1] = -1;
								canCreat = true;
								show_game();
							}
						}
						else if( map[j][i] == map[j][i-1] && p ==3 )
						{
							map[j][i]++;
							if( map[j][i] >= 12 )
							{
								map[j][i]--;
								end = true;
								return;
							}
							flash[j][i] = FLASH_TIME;
							map[j][i-1] = -1;
							space++;
							canCreat = true;
							show_game();
						}
					}
				}
			}
			break;
		}
	}
}

void Game::control_fps()
{
	if( fps.get_ticks() < 1000 / FPS )
    {
        SDL_Delay( ( 1000 / FPS ) - fps.get_ticks() );
    }
}

bool Game::is_end()
{
	if( end ) return end;
	
	if( space <= 0 )
	{
		for(int i = 0; i < GAME_ROW; i++)
		{
			for(int j = 0; j < GAME_ROW; j++)
			{
				for(int p = 0; p < 4; p++)
				{
					int x = i + X[p];
					int y = j + Y[p];
					if( x < 0 || x >= GAME_ROW || y < 0 || y >= GAME_ROW )
						continue;
					if( map[x][y] == map[i][j] )
					{
						return false;
					}
				}
			}
		}
	}
	else
	{
		return false;
	}
	
	return true;
}

void Game::gameRun()
{
	fps.start();
	
	move();
	
	if( dir >= 0 && canCreat )
	{
		creat_card();
		dir = -1;
		canCreat = false;
	}
		
	show_game();
	
	control_fps();
}

int main( int argc, char *args[] )
{
	srand( unsigned(time(0)));
	
	init();
	
	bool quit = false;
	SDL_Event event;
	
	
	while( !quit )
	{
		Game game;
		while( !game.is_end() )
		{
			if( SDL_PollEvent( &event ) )
			{
				if( event.type == SDL_QUIT )
				{
					quit = true;
					break;
				}
				game.handle_input( event );
			}
			game.gameRun();
		}
		if( !quit )
		{
			if( game.show_end() )
			{
				quit = true;
			}
		}
	}
	
	return 0;
} 
