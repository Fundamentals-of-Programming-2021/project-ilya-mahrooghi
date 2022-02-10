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
extern Uint32 color1;
extern Uint32 color2;
extern Uint32 color3;
extern Uint32 backgroundColor;
extern Uint32 neutralColor;

// screen size
extern const double SCREEN_WIDTH;
extern const double SCREEN_HEIGHT;

// screen size of menu
extern const double MENU_SCREEN_WIDTH;
extern const double MENU_SCREEN_HEIGHT;

// this is num of poly in a height of map
extern int numofpoly;

// array of mixture
extern int speedbooster[3];
extern int freeze[3];
extern int inf_soldiers[3];
extern int more_soldiers[3];

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

int isgame_saved(char *playername)
{
    char fileaddress[500] = "..//files//game//";
    strcat(fileaddress, playername);
    strcat(fileaddress, ".txt");
    return checkIfFileExists(fileaddress);
}

void change_data(char *playername, struct region *head, int *numofregion, int *counterof_addsoldier, int *counterof_AI)
{
    char fileaddress[500] = "..//files//game//";
    strcat(fileaddress, playername);
    strcat(fileaddress, ".txt");
    FILE *file;
    file = fopen(fileaddress, "r");
    fscanf(file, "%d", numofregion);
    // mixtures
    // speedbooster
    for (int i = 0; i < 3; i++)
    {
        fscanf(file, "%d", &speedbooster[i]);
    }

    // freeze
    for (int i = 0; i < 3; i++)
    {
        fscanf(file, "%d", &freeze[i]);
    }

    // inf_soldiers
    for (int i = 0; i < 3; i++)
    {
        fscanf(file, "%d", &inf_soldiers[i]);
    }

    // more_soldiers
    for (int i = 0; i < 3; i++)
    {
        fscanf(file, "%d", &more_soldiers[i]);
    }

    // regions
    for (int i = 0; i < *numofregion; i++)
    {
        // center
        fscanf(file, "%lf %lf", &((head + i)->x_center), &((head + i)->y_center));

        // numofsoldiers
        fscanf(file, "%d", &((head + i)->numofsoldiers));

        // colors
        fscanf(file, "%x %x", &((head + i)->maincolor), &((head + i)->nowcolor));

        // side
        fscanf(file, "%d", &((head + i)->side));

        for (int j = 0; j < 200; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                // is_on
                fscanf(file, "%d", &((head + i)->soldiers[k][j].is_on));

                // color
                fscanf(file, "%x", &((head + i)->soldiers[k][j].color));

                // centers
                fscanf(file, "%lf %lf", &((head + i)->soldiers[k][j].x_center), &((head + i)->soldiers[k][j].y_center));

                // vx , vy
                fscanf(file, "%lf %lf ", &((head + i)->soldiers[k][j].vx), &((head + i)->soldiers[k][j].vy));

                // current vx , vy
                fscanf(file, "%lf %lf ", &((head + i)->soldiers[k][j].current_vx), &((head + i)->soldiers[k][j].current_vy));

                // to
                long int tmp;
                fscanf(file, "%ld", &tmp);
                if (tmp == -1)
                {
                    (head + i)->soldiers[k][j].to = NULL;
                }
                else
                {
                    (head + i)->soldiers[k][j].to = (head + tmp);
                }
            }
        }
    }
    fscanf(file, "%d %d", counterof_addsoldier, counterof_AI);
    fclose(file);
}
