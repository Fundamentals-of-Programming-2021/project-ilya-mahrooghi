#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>

// colors
Uint32 color1 = 0x20ee04e6;
Uint32 color2 = 0x20f0240d;
Uint32 color3 = 0x201010f7;
Uint32 backgroundColor = 0xff7f00ff;
Uint32 neutralColor = 0x10b0adab;

// screen size
const double SCREEN_WIDTH = 1000;
const double SCREEN_HEIGHT = 1000;

// this is num of poly in a height of map
int numofpoly = 8;

// array of mixture
int speedbooster[3] = {0};
int freeze[3] = {0};
int inf_soldiers[3] = {0};

// structs
struct point
{
    double x, y;
};

struct soldier
{
    int is_on; // is the soldier in game or not
    struct region *to;
    Uint32 color;
    double x_center, y_center;
    double vx, vy;
    double current_vx, current_vy;
};

struct region
{
    double x_center, y_center;
    int numofsoldiers;
    Uint32 maincolor, nowcolor;
    struct soldier soldiers[4][2000];
    struct region *toattacking;
    int side;
};

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
}

void text(SDL_Renderer *m_renderer, double xp, double yp, double w, double h, int fontsize, int R, int G, int B, int A, const char *S)
{
    TTF_Font *Sans = TTF_OpenFont("//home//ilya//Desktop//codes//state.io//fonts//GothicA1-Regular.ttf", fontsize);
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
    SDL_DestroyTexture(Message);
    TTF_CloseFont(Sans);
}

// function for color of the regions
void random_color_array(Uint32 colorarray[][5])
{
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
                colorarray[i][j] = neutralColor;
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
void drawpolygonregion(SDL_Renderer *renderer, struct point center, double radius, Uint32 nowcolor, Uint32 maincolor)
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

    if (maincolor == neutralColor)
    {
        //  0xA0ffffff
        filledCircleRGBA(renderer, center.x, center.y, radius / 3, 255, 255, 255, 0xA0);
    }
    if (maincolor == color1)
    {
        drawtriangle(renderer, center, 30, 0xff6f176c, 0);
        drawtriangle(renderer, center, 30, 0xff6f176c, 3.1415);

        // boxColor(renderer, center.x - width, center.y - width, center.x + width + 5, center.y + width + 5, 0xff6f176c);
        // filledCircleRGBA(renderer, center.x, center.y, radius / 3, 0, 255, 0, 255);
    }
    if (maincolor == color2)
    {
        drawtriangle(renderer, center, 30, 0xff300000, 0);
        // filledCircleRGBA(renderer, center.x, center.y, radius / 3, 0, 0, 0xff, 0xff);
    }
    if (maincolor == color3)
    {
        drawplus(renderer, center, 9, 23, 0xff17178D);
    }
}

// define the array of regions
struct region *polygonwindow(SDL_Renderer *renderer, int *num)
{
    Uint32 color[5][5];
    random_color_array(color);

    struct point firstcenter;
    firstcenter.x = 25;
    firstcenter.y = -20;
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
            Uint32 currentcolor = color[i - 2][j - 2];
            if (color[i - 2][j - 2] != neutralColor)
            {
                (head + count)->numofsoldiers = 10;
            }
            else
            {
                (head + count)->numofsoldiers = 50;
            }
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
            if (nowcolor == color1)
            {
                (head + count)->side = 0;
            }
            if (nowcolor == color2)
            {
                (head + count)->side = 1;
            }
            if (nowcolor == color3)
            {
                (head + count)->side = 2;
            }
            count++;
        }
    }
    *num = count;
    for (int i = 0; i < count; i++)
    {
        (head + i)->toattacking = NULL;
        for (int j = 0; j < 1000; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                ((head + i)->soldiers[k][j]).is_on = 0;
                (head + i)->soldiers[k][j].x_center = (head + i)->x_center;
                (head + i)->soldiers[k][j].y_center = (head + i)->y_center;
                (head + i)->soldiers[k][j].color = (head + i)->maincolor;
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
            int sidenum = (head + i)->side;
            if (inf_soldiers[sidenum] == 0)
            {
                if (((head + i)->numofsoldiers) < 70)
                {
                    ((head + i)->numofsoldiers) += 1;
                }
            }
            else
            {
                (head + i)->numofsoldiers += 1;
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
            drawpolygonregion(renderer, center, radius, nowcolor, maincolor);

            int numofsoldiers = (head + i)->numofsoldiers;
            char *string = (char *)malloc(sizeof(char) * 100);
            sprintf(string, " %d ", numofsoldiers);
            const char *str2 = string;
            text(renderer, (head + i)->x_center - 9, (head + i)->y_center + 52, 30, 30, 32, 0, 0, 0, 255, str2);
        }
    }
}

// find the region function
struct region *findnearestregion(double x, double y, struct region *head, int numofregions)
{
    for (int i = 0; i < numofregions; i++)
    {
        if (distance(x, y, (head + i)->x_center, (head + i)->y_center) <= 40)
        {
            return (head + i);
        }
    }
    return NULL;
}