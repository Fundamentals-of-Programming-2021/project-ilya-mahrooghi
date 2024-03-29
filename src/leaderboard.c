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

void define_home(struct button *home)
{
    home->x = 683;
    home->y = 700;
    home->zoom = 0;
}

void draw_homebutton(SDL_Renderer *renderer, struct button *home)
{
    if (home->zoom == 0)
    {
        showimage(renderer, "..//photo//leaderboard//homebutton.bmp", home->x, home->y, 90, 90);
    }
    else
    {
        double zooming = 10;
        showimage(renderer, "..//photo//leaderboard//homebutton.bmp", home->x - zooming, home->y - zooming, 90 + 2 * zooming, 90 + 2 * zooming);
    }
}

int nearhome(struct button *home, double mouse_x, double mouse_y)
{
    if (norm(mouse_x - (home->x + 45)) <= 35 && norm(mouse_y - (home->y + 45)) <= 35)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void zoominghome(double mouse_x, double mouse_y, struct button *home)
{
    if (norm(mouse_x - (home->x + 45)) <= 35 && norm(mouse_y - (home->y + 45)) <= 35)
    {
        home->zoom = 1;
    }
    else
    {
        home->zoom = 0;
    }
}

void showranking(SDL_Renderer *renderer)
{
    char *users[1000];
    for (int i = 0; i < 1000; i++)
    {
        users[i] = (char *)malloc(sizeof(char) * 30);
    }
    int points[1000];
    int index = readfile(users, points);
    sort(users, points, index);
    users[index][0] = '\0';

    int x = 90;
    for (int i = 0; i < 5; i++)
    {
        if (users[i][0] != '\0')
        {
            showimage(renderer, "..//photo//leaderboard//button.bmp", 420, 170 + x * i, 600, 80);
            char *tmpstring = (char *)malloc(sizeof(char) * 30);
            // show rank num
            sprintf(tmpstring, "%d", i + 1);
            text(renderer, 452, 195 + x * i, strlen(tmpstring) * 30, 30, 32, 0, 0, 0, 255, tmpstring);

            // show playername
            sprintf(tmpstring, "%s", users[i]);
            text(renderer, 690 - strlen(tmpstring) * 13, 195 + x * i, strlen(tmpstring) * 31, 30, 32, 0, 0, 0, 255, tmpstring);

            // show points
            sprintf(tmpstring, "%d", points[i]);
            text(renderer, 953 - strlen(tmpstring) * 5, 200 + x * i, strlen(tmpstring) * 15, 35, 32, 0, 0, 0, 255, tmpstring);
            free(tmpstring);
        }
        else
        {
            break;
        }
    }
    for (int i = 0; i < 1000; i++)
    {
        free(users[i]);
    }
}