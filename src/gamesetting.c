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

void define_savemap(struct button *savemap)
{
    savemap->x = 65;
    savemap->y = 150;
    savemap->zoom = 0;
}

void define_savegame(struct button *savegame)
{
    savegame->x = 65;
    savegame->y = 300;
    savegame->zoom = 0;
}

void showsettings(SDL_Renderer *renderer, struct button *savemap, struct button *savegame)
{
    if (savegame->zoom == 0)
    {
        showimage(renderer, "..//photo//game//background//savegame.bmp", savegame->x, savegame->y, 280, 90);
    }
    else
    {
        double zooming = 10;
        showimage(renderer, "..//photo//game//background//savegame.bmp", savegame->x - zooming, savegame->y - zooming, 280 + 2 * zooming, 90 + 2 * zooming);
    }
    if (savemap->zoom == 0)
    {
        showimage(renderer, "..//photo//game//background//savemap.bmp", savemap->x, savemap->y, 280, 90);
    }
    else
    {
        double zooming = 10;
        showimage(renderer, "..//photo//game//background//savemap.bmp", savemap->x - zooming, savemap->y - zooming, 280 + 2 * zooming, 90 + 2 * zooming);
    }
}

int near_button_setting(struct button *button, double mouse_x, double mouse_y)
{
    if (norm(button->x + 140 - (mouse_x)) <= 140 && norm(button->y + 45 - (mouse_y)) <= 45)
    {
        return 1;
    }
    return 0;
}

////////////////////////////////////////////////////////////////
void save_map(char *playername, Uint32 color[][5])
{
    char fileaddress[500] = "..//files//map//";
    strcat(fileaddress, playername);
    strcat(fileaddress, ".txt");
    FILE *file;
    file = fopen(fileaddress, "w");
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            fprintf(file, "%x ", color[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void save_game(char *playername, struct region *head, int numofregion, int counterof_addsoldier, int counterof_AI)
{
    char fileaddress[500] = "..//files//game//";
    strcat(fileaddress, playername);
    strcat(fileaddress, ".txt");
    FILE *file;
    file = fopen(fileaddress, "w");
    // num of regions
    fprintf(file, "%d\n", numofregion);

    // mixtures
    // speedbooster
    for (int i = 0; i < 3; i++)
    {
        fprintf(file, "%d ", speedbooster[i]);
    }
    fprintf(file, "\n");

    // freeze
    for (int i = 0; i < 3; i++)
    {
        fprintf(file, "%d ", freeze[i]);
    }
    fprintf(file, "\n");

    // inf_soldiers
    for (int i = 0; i < 3; i++)
    {
        fprintf(file, "%d ", inf_soldiers[i]);
    }
    fprintf(file, "\n");

    // more_soldiers
    for (int i = 0; i < 3; i++)
    {
        fprintf(file, "%d ", more_soldiers[i]);
    }
    fprintf(file, "\n");

    // regions
    for (int i = 0; i < numofregion; i++)
    {
        // center
        fprintf(file, "%lf %lf\n", (head + i)->x_center, (head + i)->y_center);

        // numofsoldiers
        fprintf(file, "%d\n", (head + i)->numofsoldiers);

        // colors
        fprintf(file, "%x %x\n", (head + i)->maincolor, (head + i)->nowcolor);

        // side
        fprintf(file, "%d\n", (head + i)->side);

        for (int j = 0; j < 200; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                // is_on
                fprintf(file, "%d ", (head + i)->soldiers[k][j].is_on);

                // color
                fprintf(file, "%x ", (head + i)->soldiers[k][j].color);

                // centers
                fprintf(file, "%lf %lf ", (head + i)->soldiers[k][j].x_center, (head + i)->soldiers[k][j].y_center);

                // vx , vy
                fprintf(file, "%lf %lf ", (head + i)->soldiers[k][j].vx, (head + i)->soldiers[k][j].vy);

                // current vx , vy
                fprintf(file, "%lf %lf ", (head + i)->soldiers[k][j].current_vx, (head + i)->soldiers[k][j].current_vy);

                // to
                if ((head + i)->soldiers[k][j].to != NULL)
                {
                    fprintf(file, "%ld\n", (head + i)->soldiers[k][j].to - head);
                }
                else
                {
                    fprintf(file, "-1\n");
                }
            }
        }
    }
    fprintf(file, "%d %d\n", counterof_addsoldier, counterof_AI);
    fclose(file);
}