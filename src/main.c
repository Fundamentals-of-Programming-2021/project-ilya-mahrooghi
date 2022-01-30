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
#include "mixture.h"

int main()
{
  // check working
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0 || TTF_Init() < 0)
  {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 0;
  }

  // deine window and renderer
  const int FPS = 60;
  SDL_Window *sdlWindow = SDL_CreateWindow("Test_Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
  SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

  // random time
  time_t t;
  srand(time(&t));

  // when close the window
  SDL_bool shallExit = SDL_FALSE;

  // define regions
  struct region *headregion = (struct region *)malloc(sizeof(struct region) * 100);
  int numofregions;
  headregion = polygonwindow(sdlRenderer, &numofregions);

  // attacking of soldiers and work with mouse
  struct region *attackfrom = (struct region *)malloc(sizeof(struct region));
  struct region *attackto = (struct region *)malloc(sizeof(struct region));
  double mouse_x, mouse_y;

  // mixtures
  struct speedbooster *head_speedbooster = (struct speedbooster *)malloc(sizeof(struct speedbooster) * 4);
  struct freeze *head_freeze = (struct freeze *)malloc(sizeof(struct freeze) * 4);
  struct inf_soldiers *head_inf_soldiers = (struct inf_soldiers *)malloc(sizeof(struct inf_soldiers) * 4);
  struct more_soldiers *head_more_soldiers = (struct more_soldiers *)malloc(sizeof(struct more_soldiers) * 4);
  define_mixtures(head_speedbooster, head_freeze, head_inf_soldiers, head_more_soldiers);

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

    // use the mixtures
    all_of_mixtures(sdlRenderer, head_speedbooster, head_freeze, head_inf_soldiers, head_more_soldiers, headregion, numofregions);

    // render presentation
    SDL_RenderPresent(sdlRenderer);
    SDL_Delay(300);

    // events
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

  // destroy window and quit
  SDL_DestroyWindow(sdlWindow);
  printf("Hope see you soon for the next game:)\n");
  SDL_Quit();
  return 0;
}
