#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "myheader.h"

int main()
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
  {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 0;
  }
  TTF_Init();

  ///////////// main
  SDL_Window *sdlWindow = SDL_CreateWindow("Test_Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
  // error handling

  const int FPS = 60;

  //// Main
  SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
  SDL_Event sdlevent;
  int numofcolors = 50;
  Uint32 color[numofcolors][numofcolors];
  random_color_array(numofcolors, color);
  time_t t;
  srand(time(&t));
  struct point center;
  center.x = 0;
  center.y = 0;

  // define regions
  struct region *headregion = (struct region *)malloc(sizeof(struct region) * 100);
  int numofregions;
  headregion = polygonwindow(sdlRenderer, center, color, &numofregions);
  ////////////////////////////////////////////////////////////////////////
  while (1)
  {
    // reset the color
    SDL_SetRenderDrawColor(sdlRenderer, 0x90, 0x90, 0x90, 0xff);
    SDL_RenderClear(sdlRenderer);
    // intialize the screen
    changecolorofregion(headregion , numofregions);
    printregions(sdlRenderer, numofregions, headregion);
    shownumofsoldiers(sdlRenderer, headregion, numofregions);
    addsoldier(sdlRenderer, headregion, numofregions);

    // render presentation and SDL_Quit=
    SDL_Delay(500);
    SDL_RenderPresent(sdlRenderer);
    SDL_PollEvent(&sdlevent);
    if (sdlevent.type == SDL_QUIT)
    {
      break;
    }
  }

  SDL_DestroyWindow(sdlWindow);
  printf("Hope see you soon for the next game:)\n");
  SDL_Quit();
  return 0;
}