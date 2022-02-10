#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/stat.h>

// colors
Uint32 color1 = 0x209b2d71;
Uint32 color2 = 0x20926c43;
Uint32 color3 = 0x203543c0;
Uint32 backgroundColor = 0xff7f00ff;
Uint32 neutralColor = 0x10b0adab;

// screen size
const double SCREEN_WIDTH = 1000;
const double SCREEN_HEIGHT = 1200;

// screen size of menu
const double MENU_SCREEN_WIDTH = 1440;
const double MENU_SCREEN_HEIGHT = 810;

// this is num of poly in a height of map
int numofpoly = 8;

// array of mixture
int speedbooster[3] = {0};
int freeze[3] = {0};
int inf_soldiers[3] = {0};
int more_soldiers[3] = {0};

#include "map.h"
#include "soldiers.h"
#include "potion.h"
#include "endgame.h"
#include "menu.h"
#include "leaderboard.h"
#include "choosemap.h"
#include "gamesetting.h"
#include "resume.h"
#include "AI.h"
#include "general.h"

int main()
{
  // check working
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0 || TTF_Init() < 0)
  {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 0;
  }

  // random time
  time_t t;
  srand(time(&t));

  // end of program
  int endofprogram = 0;

  // scan player name
  char *playername = (char *)malloc(sizeof(char) * 200);
  playername = getname(playername);
  if (playername != NULL)
  {
    while (!endofprogram)
    {
      endofprogram = menu(playername);
    }
  }
  free(playername);

  SDL_Quit();
  TTF_Quit();
  return 0;
}