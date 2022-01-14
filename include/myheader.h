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

struct point rotate(struct point center, struct point torotate, double angle)
{
    struct point ans;
    double radius = (torotate.x - center.x) * (torotate.x - center.x) + (torotate.y - center.y) * (torotate.y - center.y);
    radius = pow(radius , 0.5);
    ans.x = radius * cos(angle) + torotate.x;
    ans.y = radius * sin(angle) + torotate.y;
    return ans;
}

struct region *drawpolygonregion(SDL_Renderer *renderer, struct point center, int radius, Uint32 color)
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
        
        firstpoint = rotate(center , firstpoint , 1.04);
    }

    filledPolygonColor(renderer  ,xarray, yarray ,6 , color);

}
