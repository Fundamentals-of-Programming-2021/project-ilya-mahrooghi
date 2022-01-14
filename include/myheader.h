#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

struct point
{
    double x, y;
};

struct region
{
    int x_center, y_center;
    int numofsoldiers;
    Uint32 color;
};

void random_color_array(Uint32 color1, Uint32 color2, Uint32 color3, Uint32 color4, Uint32 colorarray[10][10])
{
    time_t t;
    srand((unsigned)time(&t));
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            int tmp;
            tmp = rand();
            tmp %= 4;

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
                colorarray[i][j] = color4;
                break;
            }
            // colorarray[i][j] = random_color(color1, color2, color3, color4);
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

struct region *drawpolygonregion(SDL_Renderer *renderer, struct point center, double radius, Uint32 color)
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
    for (int i = 0; i < 6; i++)
    {
        double width = 4;
        thickLineRGBA(renderer, xarray[i], yarray[i], xarray[(i + 1) % 6], yarray[(i + 1) % 6], width, 0, 0, 0, 0xff);
    }

    filledPolygonColor(renderer, xarray, yarray, 6, color);
    struct region *poly = (struct region *)malloc(sizeof(struct region));
    poly->x_center = center.x;
    poly->y_center = center.y;
    poly->color = color;
    poly->numofsoldiers = 0;
    return poly;
}

struct region *polygonwindow(SDL_Renderer *renderer, struct point firstcenter, const int screen_height, const int screen_width, Uint32 color[10][10])
{
    double radius = screen_width / 7.5;
    double x_diff = 3 * radius;
    double y_diff = radius * pow(3, 0.5);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            struct point center;
            center.x = firstcenter.x + x_diff * (i - 1);
            center.y = firstcenter.y + y_diff * (j - 1);
            drawpolygonregion(renderer, center, radius, color[i][j]);
        
        }
    }
    firstcenter.x += 3 * radius / 2;
    firstcenter.y += radius * (pow(3, 0.5) / 2);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            struct point center;
            center.x = firstcenter.x + x_diff * i;
            center.y = firstcenter.y + y_diff * (j - 1);
            drawpolygonregion(renderer, center, radius, color[i + 3][j]);
            
        }
    }
}