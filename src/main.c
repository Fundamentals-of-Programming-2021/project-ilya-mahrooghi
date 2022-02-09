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