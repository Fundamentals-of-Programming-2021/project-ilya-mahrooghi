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
#include "poison.h"

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
  time_t t;
  srand(time(&t));
  SDL_bool shallExit = SDL_FALSE;

  // define regions
  struct region *headregion = (struct region *)malloc(sizeof(struct region) * 100);
  int numofregions;
  headregion = polygonwindow(sdlRenderer, &numofregions);

  // attacking of soldiers and work with mouse
  struct region *attackfrom = (struct region *)malloc(sizeof(struct region));
  struct region *attackto = (struct region *)malloc(sizeof(struct region));
  double mouse_x, mouse_y;

  // poison
  struct speedbooster *speedbooster_head = (struct speedbooster *)malloc(sizeof(struct speedbooster) * 4);
  definespeedbooster(speedbooster_head);



  ////////////////////////////////////////////////////////////////////////
  while (shallExit == SDL_FALSE)
  {
    // reset the color
    SDL_SetRenderDrawColor(sdlRenderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(sdlRenderer);

    // intialize the screen
    changecolorofregion(headregion, numofregions);
    addsoldier(sdlRenderer, headregion, numofregions);
    printregions(sdlRenderer, numofregions, headregion);
    attacking(sdlRenderer, headregion, numofregions);

    all_of_speedboosters(sdlRenderer, speedbooster_head , headregion , numofregions);

    // render presentation and events
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
      case SDL_MOUSEBUTTONDOWN:
        mouse_x = sdlevent.button.x;
        mouse_y = sdlevent.button.y;
        attackfrom = findnearestregion(mouse_x, mouse_y, headregion, numofregions);
        break;
      case SDL_MOUSEBUTTONUP:
        mouse_x = sdlevent.button.x;
        mouse_y = sdlevent.button.y;
        attackto = findnearestregion(mouse_x, mouse_y, headregion, numofregions);
        if (attackto != NULL && attackfrom != NULL && attackto != attackfrom)
        {
          start_of_attack(attackfrom, attackto);
        }
        break;
      }
    }
  }
  SDL_DestroyWindow(sdlWindow);
  printf("Hope see you soon for the next game:)\n");
  SDL_Quit();
  return 0;
}