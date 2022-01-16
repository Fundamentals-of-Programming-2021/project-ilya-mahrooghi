#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

Uint32 color1 = 0x2000ff00;
Uint32 color2 = 0x20ff0000;
Uint32 color3 = 0x200000ff;
Uint32 backgroundColor = 0xffffffff;
Uint32 neutralColor = 0x80000000;
const double SCREEN_WIDTH = 1000;
const double SCREEN_HEIGHT = 1000;

int numofpoly = 20;//this is num of poly in a height of map

struct point
{
    int x, y;
};

struct region
{
    int x_center, y_center;
    int numofsoldiers;
    Uint32 maincolor, nowcolor;
};

void text(SDL_Renderer *m_renderer, int xp, int yp, int w, int h, int fontsize, int R, int G, int B, int A, const char *S)
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
            tmp %= 7;

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

void drawpolygonregion(SDL_Renderer *renderer, struct point center, double radius, Uint32 color)
{
    int x = center.x;
    int y = center.y;
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
    if (color != backgroundColor)
    {
        for (int i = 0; i < 6; i++)
        {
            double width = 4;
            thickLineRGBA(renderer, xarray[i], yarray[i], xarray[(i + 1) % 6], yarray[(i + 1) % 6], width, 0, 0, 0, 0xff);
        }
    }

    filledPolygonColor(renderer, xarray, yarray, 6, color);

    if (color != backgroundColor)
    {
        // boxColor(renderer, center.x - radius / 3, center.y - radius / 3, center.x + radius / 3, center.y + radius / 3, 0xffffffff);
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
                (head + count)->numofsoldiers = 0;
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
    return head;
}

void shownumofsoldiers(SDL_Renderer *renderer, struct region *head, int num)
{
    for (int i = 0; i < num; i++)
    {
        if ((head + i)->maincolor != backgroundColor)
        {
            int numofsoldiers = (head + i)->numofsoldiers;
            char *string = (char *)malloc(sizeof(char) * 100);
            sprintf(string, "%d", numofsoldiers);
            const char *str2 = string;
            text(renderer, (head + i)->x_center - 7, (head + i)->y_center - 5, 20, 20, 20, 0, 0, 0, 255, str2);
        }
    }
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
            Uint32 color = (head + i)->nowcolor;
            drawpolygonregion(renderer, center, radius, color);
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
        }
    }
}