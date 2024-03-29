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

// functions:
double norm(double x)
{
    if (x < 0)
    {
        x *= -1;
    }
    return x;
}

double distance(double x0, double y0, double x1, double y1)
{
    double tmp1 = x0 - x1;
    double tmp2 = y0 - y1;
    return sqrt(tmp1 * tmp1 + tmp2 * tmp2);
}

// functions of showing the image and text
void showimage(SDL_Renderer *renderer, char *image_path, int x0, int y0, int w0, int h0)
{
    SDL_Surface *image = SDL_LoadBMP(image_path);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);

    SDL_FreeSurface(image);
    image = NULL;

    SDL_Rect texture_rect = {.x = x0, .y = y0, .w = w0, .h = h0};
    SDL_RenderCopy(renderer, texture, NULL, &texture_rect);
    SDL_DestroyTexture(texture);
}

void text(SDL_Renderer *m_renderer, double xp, double yp, double w, double h, int fontsize, int R, int G, int B, int A, const char *S)
{
    TTF_Font *Sans = TTF_OpenFont("..//fonts//GothicA1-Regular.ttf", fontsize);
    SDL_Color textcolor = {R, G, B, A};
    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(Sans, S, textcolor);
    SDL_Texture *Message = SDL_CreateTextureFromSurface(m_renderer, surfaceMessage);
    SDL_FreeSurface(surfaceMessage);
    SDL_Rect Message_rect;
    Message_rect.x = xp;
    Message_rect.y = yp;
    Message_rect.w = w;
    Message_rect.h = h;
    SDL_RenderCopy(m_renderer, Message, NULL, &Message_rect);
    TTF_CloseFont(Sans);
    SDL_DestroyTexture(Message);
}

// function for color of the regions
void random_color_array(Uint32 colorarray[][5])
{
    // define all of them neutralColor or backgroundColor
    for (int i = 0; i <= 3; i++)
    {

        for (int j = 0; j <= 3; j++)
        {
            int tmp = rand() % 5;

            switch (tmp)
            {
            case 0:
                colorarray[i][j] = neutralColor;
                break;
            case 1:
                colorarray[i][j] = backgroundColor;
                break;
            case 2:
                colorarray[i][j] = backgroundColor;
                break;
            case 3:
                colorarray[i][j] = neutralColor;
                break;
            case 4:
                colorarray[i][j] = neutralColor;
                break;
            case 5:
                colorarray[i][j] = neutralColor;
                break;
            }
        }
    }

    // for each of side have on color in map
    for (int i = 0; i < 3; i++)
    {
        Uint32 color;
        switch (i)
        {
        case 0:
            color = color1;
            break;
        case 1:
            color = color2;
            break;
        case 2:
            color = color3;
            break;
        }

        int row = rand() % 4;
        int column;
        if (row <= 1)
        {
            column = rand() % 4;
        }
        else
        {
            column = rand() % 3;
        }
        while (1)
        {
            row = rand() % 4;
            column;
            if (row <= 1)
            {
                column = rand() % 4;
            }
            else
            {
                column = rand() % 3;
            }
            if (colorarray[row][column] == neutralColor || colorarray[row][column] == backgroundColor)
            {
                colorarray[row][column] = color;
                break;
            }
        }
    }
}

// graphic functions
struct point rotate(struct point center, struct point torotate, double angle)
{
    struct point ans;
    double tx = torotate.x - center.x;
    double ty = torotate.y - center.y;
    ans.x = tx * cos(angle) - ty * sin(angle);
    ans.y = tx * sin(angle) + ty * cos(angle);
    ans.x += center.x;
    ans.y += center.y;
    return ans;
}

void drawtriangle(SDL_Renderer *renderer, struct point center, double radius, Uint32 color, double angle)
{
    double x = center.x;
    double y = center.y;
    Sint16 *xarray = (Sint16 *)malloc(sizeof(Sint16) * 3);
    Sint16 *yarray = (Sint16 *)malloc(sizeof(Sint16) * 3);
    struct point firstpoint;
    firstpoint.x = x;
    firstpoint.y = y + radius;

    firstpoint = rotate(center, firstpoint, angle);
    for (int i = 0; i < 3; i++)
    {
        xarray[i] = firstpoint.x;
        yarray[i] = firstpoint.y;

        firstpoint = rotate(center, firstpoint, 2.094333); // 1.256637 is 2*pi /5;
    }
    filledPolygonColor(renderer, xarray, yarray, 3, color);
}

void drawplus(SDL_Renderer *renderer, struct point center, double largeradius, double smallradius, Uint32 color)
{
    double x = center.x;
    double y = center.y;
    boxColor(renderer, center.x - smallradius, center.y + largeradius, center.x + smallradius, center.y - largeradius, color);
    boxColor(renderer, center.x - largeradius, center.y + smallradius, center.x + largeradius, center.y - smallradius, color);
}

// draw region function
void drawpolygonregion(SDL_Renderer *renderer, struct point center, double radius, Uint32 nowcolor, Uint32 maincolor, int speedboosterflag)
{
    double x = center.x;
    double y = center.y;
    Sint16 *xarray = (Sint16 *)malloc(sizeof(Sint16) * 6);
    Sint16 *yarray = (Sint16 *)malloc(sizeof(Sint16) * 6);
    struct point firstpoint;
    firstpoint.x = x + radius;
    firstpoint.y = y;
    for (int i = 0; i < 6; i++)
    {
        xarray[i] = firstpoint.x;
        yarray[i] = firstpoint.y;

        firstpoint = rotate(center, firstpoint, 1.0471975); // 1.0471975 is pi /2;
    }
    if (maincolor != backgroundColor)
    {
        for (int i = 0; i < 6; i++)
        {
            double width = 2.5;
            thickLineRGBA(renderer, xarray[i], yarray[i], xarray[(i + 1) % 6], yarray[(i + 1) % 6], width, 0, 0, 0, 0xff);
        }
    }

    filledPolygonColor(renderer, xarray, yarray, 6, nowcolor);

    if (speedboosterflag)
    {
        showimage(renderer, "..//photo//game//potion//fire.bmp", center.x - 47, center.y - 39, 100, 100);
    }

    if (maincolor == neutralColor)
    {
        showimage(renderer, "..//photo//game//background//moon.bmp", center.x - 34, center.y - 26, 76, 76);
    }
    if (maincolor == color1)
    {
        showimage(renderer, "..//photo//game//background//neptune.bmp", center.x - 34, center.y - 26, 76, 76);
    }
    if (maincolor == color2)
    {
        showimage(renderer, "..//photo//game//background//earth.bmp", center.x - 34, center.y - 26, 76, 76);
    }
    if (maincolor == color3)
    {
        showimage(renderer, "..//photo//game//background//venus.bmp", center.x - 34, center.y - 26, 76, 76);
    }
    free(xarray);
    free(yarray);
}

// define the array of regions
struct region *polygonwindow(SDL_Renderer *renderer, int *num, Uint32 color[5][5], int randflag)
{
    if (randflag == 1)
    {
        random_color_array(color);
    }

    struct point firstcenter;
    firstcenter.x = 25;
    firstcenter.y = 10;
    struct region *head = (struct region *)malloc(sizeof(struct region) * 1000);
    int count = 0;
    double radius = (SCREEN_WIDTH / (numofpoly)) + 1;
    double x_diff = 3 * radius;
    double y_diff = radius * pow(3, 0.5);
    for (int i = 2; i < 4; i++)
    {
        for (int j = 2; j < 6; j++)
        {
            struct point center;
            center.x = firstcenter.x + x_diff * (i - 1);
            center.y = firstcenter.y + y_diff * (j - 1);

            (head + count)->x_center = center.x;
            (head + count)->y_center = center.y;
            (head + count)->maincolor = color[i - 2][j - 2];
            (head + count)->nowcolor = color[i - 2][j - 2];
            if (color[i - 2][j - 2] != neutralColor)
            {
                (head + count)->numofsoldiers = 10;
            }
            else
            {
                (head + count)->numofsoldiers = 50;
            }

            Uint32 currentcolor = (head + count)->maincolor;
            if (currentcolor == color1)
            {
                (head + count)->side = 0;
            }
            else if (currentcolor == color2)
            {
                (head + count)->side = 1;
            }
            else if (currentcolor == color3)
            {
                (head + count)->side = 2;
            }
            else
            {
                (head + count)->side = -1;
            }
            count++;
        }
    }

    firstcenter.x += 3 * radius / 2;
    firstcenter.y += radius * (pow(3, 0.5) / 2);
    for (int i = 0; i < 2; i++)
    {
        for (int j = 2; j < 5; j++)
        {
            struct point center;
            center.x = firstcenter.x + x_diff * i;
            center.y = firstcenter.y + y_diff * (j - 1);
            // if (center.x <= radius || center.y <= radius || center.x >= SCREEN_WIDTH - radius - 2 || center.y >= SCREEN_HEIGHT - radius - 2)
            // {
            //     color[i + numofpoly / 2][j] = backgroundColor;
            // }
            (head + count)->x_center = center.x;
            (head + count)->y_center = center.y;
            (head + count)->maincolor = color[i + 2][j - 2];
            (head + count)->nowcolor = color[i + 2][j - 2];
            Uint32 nowcolor = color[i + 2][j - 2];
            if (color[i + 2][j - 2] != neutralColor)
            {
                (head + count)->numofsoldiers = 10;
            }
            else
            {
                (head + count)->numofsoldiers = 50;
            }

            Uint32 currentcolor = (head + count)->maincolor;
            if (currentcolor == color1)
            {
                (head + count)->side = 0;
            }
            if (currentcolor == color2)
            {
                (head + count)->side = 1;
            }
            if (currentcolor == color3)
            {
                (head + count)->side = 2;
            }
            else
            {
                (head + count)->side = -1;
            }
            count++;
        }
    }
    *num = count;
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < 200; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                ((head + i)->soldiers[k][j]).is_on = 0;
                (head + i)->soldiers[k][j].x_center = (head + i)->x_center;
                (head + i)->soldiers[k][j].y_center = (head + i)->y_center;
                (head + i)->soldiers[k][j].color = (head + i)->maincolor;
                (head + i)->soldiers[k][j].to = NULL;
            }
        }
    }
    return head;
}

// change properties of regions
void addsoldier(SDL_Renderer *renderer, struct region *head, int num)
{
    for (int i = 0; i < num; i++)
    {
        if ((head + i)->maincolor != backgroundColor && (head + i)->maincolor != neutralColor)
        {
            // num of soldiers that we should add
            int plus = 1;

            // num of side of the region
            int sidenum = (head + i)->side;

            if (more_soldiers[sidenum] > 0)
            {
                plus *= 3;
            }

            if (inf_soldiers[sidenum] == 0)
            {
                if (((head + i)->numofsoldiers) < 70)
                {
                    ((head + i)->numofsoldiers) += plus;

                    // if it has more soldiers
                    if ((head + i)->numofsoldiers > 70)
                    {
                        (head + i)->numofsoldiers = 70;
                    }
                }
            }
            if (inf_soldiers[sidenum] > 0)
            {
                (head + i)->numofsoldiers += plus;
            }
        }
    }
}

void changecolorofregion(struct region *head, int num)
{
    for (int i = 0; i < num; i++)
    {
        if ((head + i)->maincolor != backgroundColor)
        {
            int numofsoldiers = (head + i)->numofsoldiers;

            if (numofsoldiers > 50)
            {
                numofsoldiers = 50;
            }

            if ((head + i)->maincolor == color1)
            {
                (head + i)->nowcolor = (head + i)->maincolor + numofsoldiers * 0x01000000 * 4;
            }
            else if ((head + i)->maincolor == color2)
            {
                (head + i)->nowcolor = (head + i)->maincolor + numofsoldiers * 0x01000000 * 4;
            }
            else if ((head + i)->maincolor == color3)
            {
                (head + i)->nowcolor = (head + i)->maincolor + numofsoldiers * 0x01000000 * 4;
            }
            else if ((head + i)->nowcolor = neutralColor)
            {
                (head + i)->nowcolor = (head + i)->maincolor + numofsoldiers * 0x01000000 * 3;
            }
        }
    }
}

void updatesides(struct region *region, int num)
{
    for (int i = 0; i < num; i++)
    {
        Uint32 color = (region + i)->maincolor;
        if (color == color1)
        {
            (region + i)->side = 0;
        }
        if (color == color2)
        {
            (region + i)->side = 1;
        }
        if (color == color3)
        {
            (region + i)->side = 2;
        }
    }
}

// print all of regions
void printregions(SDL_Renderer *renderer, int numofregions, struct region *head)
{
    double radius = (SCREEN_WIDTH / numofpoly) + 1;
    for (int i = 0; i < numofregions; i++)
    {
        if ((head + i)->maincolor != backgroundColor)
        {
            struct point center;
            center.x = (head + i)->x_center;
            center.y = (head + i)->y_center;
            Uint32 nowcolor = (head + i)->nowcolor;
            Uint32 maincolor = (head + i)->maincolor;
            drawpolygonregion(renderer, center, radius, nowcolor, maincolor, 0);
            int tmpside = (head + i)->side;
            if ((head + i)->maincolor != neutralColor && freeze[tmpside] > 0)
            {
                showimage(renderer, "..//photo//game//potion//freezed.bmp", center.x - 40, center.y - 36, 90, 90);
            }
            if ((head + i)->maincolor != neutralColor && speedbooster[tmpside] > 0)
            {
                // showimage(renderer, "..//photo//game//potion//fire.bmp", center.x - 47, center.y - 37, 100, 100);
                drawpolygonregion(renderer, center, radius, nowcolor, maincolor, 1);
            }
            if ((head + i)->maincolor != neutralColor && inf_soldiers[tmpside] > 0)
            {
                // drawpolygonregion(renderer, center, radius, nowcolor, maincolor);
                showimage(renderer, "..//photo//game//potion//waves.bmp", center.x - 47, center.y - 37, 100, 100);
            }
            if ((head + i)->maincolor != neutralColor && more_soldiers[tmpside] > 0)
            {
                // drawpolygonregion(renderer, center, radius, nowcolor, maincolor);
                showimage(renderer, "..//photo//game//potion//more.bmp", center.x - 47, center.y - 40, 100, 100);
            }

            int numofsoldiers = (head + i)->numofsoldiers;
            char *string = (char *)malloc(sizeof(char) * 100);
            sprintf(string, " %d ", numofsoldiers);
            const char *str2 = string;
            text(renderer, (head + i)->x_center - 15, (head + i)->y_center + 52, 40, 30, 32, 0, 0, 0, 255, str2);
            free(string);
        }
    }
}

// find the region function
struct region *findnearestregion(double x, double y, struct region *head, int numofregions)
{
    for (int i = 0; i < numofregions; i++)
    {
        if (distance(x, y, (head + i)->x_center, (head + i)->y_center) <= 70)
        {
            if ((head + i)->maincolor != backgroundColor)
            {
                return (head + i);
            }
        }
    }
    return NULL;
}

// reset the potions
void resetpotions()
{
    for (int i = 0; i < 4; i++)
    {
        speedbooster[i] = 0;
    }
    for (int i = 0; i < 4; i++)
    {
        freeze[i] = 0;
    }
    for (int i = 0; i < 4; i++)
    {
        inf_soldiers[i] = 0;
    }
    for (int i = 0; i < 4; i++)
    {
        more_soldiers[i] = 0;
    }
}

// pause button
int near_button(struct button *button, double x, double y)
{
    if (norm(x - (button->x + 190)) <= 190 && norm(y - (button->y + 45)) <= 45)
    {
        return 1;
    }
    return 0;
}

void define_pause(struct button *pause)
{
    pause->x = 360;
    pause->y = 30;
    pause->zoom = 0;
}

void show_pause(SDL_Renderer *renderer, struct button *pause)
{
    if (pause->zoom == 0)
    {
        showimage(renderer, "..//photo//game//background//pause.bmp", pause->x, pause->y, 300, 70);
    }
    else
    {
        double zooming = 10;
        showimage(renderer, "..//photo//game//background//pause.bmp", pause->x - zooming, pause->y - zooming, 300 + 2 * zooming, 70 + 2 * zooming);
    }
}

void zoompause(struct button *pause, double mouse_x, double mouse_y)
{
    if (near_button(pause, mouse_x, mouse_y))
    {
        pause->zoom = 1;
    }
    else
    {
        pause->zoom = 0;
    }
}
