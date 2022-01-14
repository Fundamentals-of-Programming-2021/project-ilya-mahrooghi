#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
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
  const double SCREEN_WIDTH = 1000;
  const double SCREEN_HEIGHT = 1000;

  ///////////// main
  SDL_Window *sdlWindow = SDL_CreateWindow("Test_Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
  // error handling

  const int FPS = 60;

  //// Main
  SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
  SDL_bool shallexit = SDL_FALSE;
  SDL_Event sdlevent;
  Uint32 color[10][10];
  random_color_array(0xff00ff00, 0xffff0000, 0xff0000ff, 0x50000000, color);
  time_t t;
  srand(time(&t));
  struct point center;
  center.x = 0;
  center.y = 0;
  while (shallexit == SDL_FALSE)
  {
    // reset the color
    SDL_SetRenderDrawColor(sdlRenderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(sdlRenderer);

    // intialize the screen
    polygonwindow(sdlRenderer, center, SCREEN_HEIGHT, SCREEN_WIDTH, color);

    // render presentation and SDL_Quit
    SDL_RenderPresent(sdlRenderer);
    SDL_PollEvent(&sdlevent);
    if (sdlevent.type == SDL_QUIT)
    {
      shallexit = SDL_TRUE;
    }
  }

  SDL_Delay(1000 / FPS);
  SDL_DestroyWindow(sdlWindow);
  printf("Hope see you soon for the next game:)\n");
  SDL_Quit();
  return 0;
}