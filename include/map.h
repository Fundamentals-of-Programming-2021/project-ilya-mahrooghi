#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>

Uint32 color1 = 0x2000A000;
Uint32 color2 = 0x20E00000;
Uint32 color3 = 0x200000ff;
Uint32 backgroundColor = 0xffffffff;
Uint32 neutralColor = 0x10000000;
const double SCREEN_WIDTH = 1000;
const double SCREEN_HEIGHT = 1000;

int numofpoly = 11; // this is num of poly in a height of map

struct point
{
    double x, y;
};

struct soldier
{
    int is_on; // is the soldier game or not
    struct region *to;
    Uint32 color;
    double x_center, y_center;
    double vx, vy;
};

struct region
{
    double x_center, y_center;
    int numofsoldiers;
    Uint32 maincolor, nowcolor;
    struct soldier soldiers[1000];
    struct region *toattacking;
};

double norm(double x)
{
    if (x < 0)
    {
        x = -x;
    }
    return x;
}

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

void random_color_array(int num, Uint32 colorarray[num][num])
{
    time_t t;
    srand((unsigned)time(&t));
    for (int i = 0; i < num; i++)
    {
        for (int j = 0; j < num; j++)
        {
            int tmp;
            tmp = rand();
            tmp %= 6;

            switch (tmp)
            {
            case 0:
                colorarray[i][j] = color1;
                break;
            case 1:
                colorarray[i][j] = color2;
                ;
                break;
            case 2:

                colorarray[i][j] = color3;
                break;
            case 3:
                colorarray[i][j] = backgroundColor;
                break;
            case 4:
                colorarray[i][j] = neutralColor;
            case 5:
                colorarray[i][j] = neutralColor;
            case 6:
                colorarray[i][j] = neutralColor;
            }
        }
    }
}

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

void drawtriangle(SDL_Renderer *renderer, struct point center, double radius, Uint32 color)
{
    double x = center.x;
    double y = center.y;
    Sint16 *xarray = (Sint16 *)malloc(sizeof(Sint16) * 3);
    Sint16 *yarray = (Sint16 *)malloc(sizeof(Sint16) * 3);
    struct point firstpoint;
    firstpoint.x = x;
    firstpoint.y = y + radius;
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
        double width = radius / 5;
        boxColor(renderer, center.x - width, center.y - width, center.x + width + 5, center.y + width + 5, 0xff00ff00);
        //filledCircleRGBA(renderer, center.x, center.y, radius / 3, 0, 255, 0, 255);
    }
    if (maincolor == color2)
    {
        drawtriangle(renderer, center, 30, 0xff300000);
        //filledCircleRGBA(renderer, center.x, center.y, radius / 3, 0, 0, 0xff, 0xff);
    }
    if (maincolor == color3)
    {
        drawplus(renderer, center, 10, 30, 0xff000040);
    }
}

struct region *polygonwindow(SDL_Renderer *renderer, struct point firstcenter, Uint32 color[][40], int *num)
{
    struct region *head = (struct region *)malloc(sizeof(struct region) * 1000);
    int count = 0;
    double radius = (SCREEN_WIDTH / numofpoly) + 1;
    double x_diff = 3 * radius;
    double y_diff = radius * pow(3, 0.5);
    for (int i = 1; i < numofpoly / 2; i++)
    {
        for (int j = 0; j < numofpoly; j++)
        {
            struct point center;
            center.x = firstcenter.x + x_diff * (i - 1);
            center.y = firstcenter.y + y_diff * (j - 1);
            if (center.x <= radius || center.y <= radius || center.x >= SCREEN_WIDTH - radius || center.y >= SCREEN_HEIGHT - radius)
            {
                color[i][j] = backgroundColor;
            }
            (head + count)->x_center = center.x;
            (head + count)->y_center = center.y;
            (head + count)->maincolor = color[i][j];
            (head + count)->nowcolor = color[i][j];
            if (color[i][j] != neutralColor)
            {
                (head + count)->numofsoldiers = 10;
            }
            else
            {
                (head + count)->numofsoldiers = 50;
            }
            count++;
        }
    }
    firstcenter.x += 3 * radius / 2;
    firstcenter.y += radius * (pow(3, 0.5) / 2);
    for (int i = 0; i < numofpoly / 2; i++)
    {
        for (int j = 0; j < numofpoly; j++)
        {
            struct point center;
            center.x = firstcenter.x + x_diff * i;
            center.y = firstcenter.y + y_diff * (j - 1);
            if (center.x <= radius || center.y <= radius || center.x >= SCREEN_WIDTH - radius - 2 || center.y >= SCREEN_HEIGHT - radius - 2)
            {
                color[i + numofpoly / 2][j] = backgroundColor;
            }
            (head + count)->x_center = center.x;
            (head + count)->y_center = center.y;
            (head + count)->maincolor = color[i + numofpoly / 2][j];
            (head + count)->nowcolor = color[i + numofpoly / 2][j];
            if (color[i + numofpoly / 2][j] != neutralColor)
            {
                (head + count)->numofsoldiers = 10;
            }
            else
            {
                (head + count)->numofsoldiers = 50;
            }
            count++;
        }
    }
    *num = count;
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < 1000; j++)
        {
            ((head + i)->soldiers[j]).is_on = 0;
            (head + i)->toattacking = NULL;
        }
    }
    return head;
}

void addsoldier(SDL_Renderer *renderer, struct region *head, int num)
{
    for (int i = 0; i < num; i++)
    {
        if ((head + i)->maincolor != backgroundColor)
            if (((head + i)->numofsoldiers) < 50)
            {
                ((head + i)->numofsoldiers) += 1;
            }
    }
}

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

void changecolorofregion(struct region *head, int num)
{
    for (int i = 0; i < num; i++)
    {
        if ((head + i)->maincolor != backgroundColor)
        {
            int numofsoldiers = (head + i)->numofsoldiers;

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
