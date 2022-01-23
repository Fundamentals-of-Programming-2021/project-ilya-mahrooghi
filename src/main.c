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

#include "map.h"
#include "soldiers.h"

int main()
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0 || TTF_Init() < 0)
  {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 0;
  }

  const int FPS = 60;

  SDL_Window *sdlWindow = SDL_CreateWindow("Test_Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

  SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
  ////////////////////////////////////////////////////////////////

  int numofcolors = 50;
  Uint32 color[numofcolors][numofcolors];
  random_color_array(numofcolors, color);
  time_t t;
  srand(time(&t));
  SDL_bool shallExit = SDL_FALSE;

  // define regions
  struct point center;
  center.x = 0;
  center.y = 0;
  struct region *headregion = (struct region *)malloc(sizeof(struct region) * 100);
  int numofregions;
  headregion = polygonwindow(sdlRenderer, center, color, &numofregions);
  ////////////////////////////////////////////////////////////////////////
  struct soldier *soldier = (struct soldier *)malloc(sizeof(struct soldier));
  soldier->x_center = 500;
  soldier->y_center = 500;
  while (shallExit == SDL_FALSE)
  {
    // reset the color
    SDL_SetRenderDrawColor(sdlRenderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(sdlRenderer);
    // intialize the screen
    changecolorofregion(headregion, numofregions);
    addsoldier(sdlRenderer, headregion, numofregions);
    printregions(sdlRenderer, numofregions, headregion);

    printsoldier(sdlRenderer, soldier);
    soldier->x_center += 25;
    if (soldier->x_center > SCREEN_WIDTH)
    {
      soldier->x_center = 5;
    }
    soldier->y_center += 25;
    if (soldier->y_center > SCREEN_HEIGHT)
    {
      soldier->y_center = 5;
    }

    // render presentation and SDL_Quit=
    SDL_RenderPresent(sdlRenderer);
    SDL_Delay(300);
    SDL_Event sdlevent;
    while (SDL_PollEvent(&sdlevent))
    {
      switch (sdlevent.type)
      {
      case SDL_QUIT:
        shallExit = SDL_TRUE;
        break;
      }
    }
  }
  SDL_DestroyWindow(sdlWindow);
  printf("Hope see you soon for the next game:)\n");
  SDL_Quit();
  return 0;
}