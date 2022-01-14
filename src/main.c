#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <stdio.h>
#include <stdbool.h>


#include "myheader.h"


int main()
{
  /* Additional Header

  print();
  struct Node mynode;
  mynode.id = 5;
  mynode.number = 0;
  printf("node id is %d and its number is %lf\n", mynode.id, mynode.number);

  */

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
  {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 0;
  }
  const int SCREEN_WIDTH = 1000;
  const int SCREEN_HEIGHT = 1000;

  ///////////// main
  SDL_Window *sdlWindow = SDL_CreateWindow("Test_Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
  // error handling

  const int FPS = 60;

  //// Main
  SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
  SDL_bool shallexit = SDL_FALSE;
  SDL_Event sdlevent;
  while (shallexit == SDL_FALSE)
  {
	// reset the color
    SDL_SetRenderDrawColor(sdlRenderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(sdlRenderer);


	//intialize the screen
	struct point center;
	center.x = SCREEN_HEIGHT/2;
	center.y = SCREEN_HEIGHT/2;
	drawpolygonregion(sdlRenderer, center , 100 , 0xffffffff);







	// render presentation and SDL_Quit
    SDL_RenderPresent(sdlRenderer);
    SDL_PollEvent(&sdlevent);
    if (sdlevent.type == SDL_QUIT)
    {
      shallexit = SDL_TRUE;
    }
  }

  SDL_Delay(1000);
  SDL_DestroyWindow(sdlWindow);
  printf("Hello World\n");
  SDL_Quit();
  return 0;
}