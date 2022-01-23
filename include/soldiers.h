struct soldier
{
    struct region *from;
    struct region *to;
    double x_center, y_center;
    double vx, vy;
};

void printsoldier(SDL_Renderer *renderer, struct soldier *soldier)
{
    //Uint32 color = soldier->from->maincolor;
    Uint32 color = color3;
    Uint32 copycolor = color;
    if (color == color1)
    {
        color = 0xff007000;
    }
    if (color == color2)
    {
        color = 0xffff0000;
    }
    if (color == color3)
    {
        color = 0xff0000D0;
    }

    double height = 17, width1 = 15, width2 = 10;
    double centerx = soldier->x_center;
    double centery = soldier->y_center;

    boxColor(renderer, centerx - width1, centery - height, centerx + width1, centery + height, 0xff000000);

    boxColor(renderer, centerx - width2, centery - height, centerx + width2, centery + height, color);

    double distanceoflegs = 5, leglength = 5;
    thickLineColor(renderer, centerx - distanceoflegs, centery + height, centerx - distanceoflegs, centery + height + leglength, 4, 0xff000000);
    thickLineColor(renderer, centerx + distanceoflegs, centery + height, centerx + distanceoflegs, centery + height + leglength, 4, 0xff000000);

    struct point center = {centerx, centery};
    if (copycolor == color1)
    {
        double width = 5;
        boxColor(renderer, center.x - width, center.y - width, center.x + width, center.y + width, 0xff00ff00);
        //filledCircleRGBA(renderer, center.x, center.y, radius / 3, 0, 255, 0, 255);
    }
    if (copycolor == color2)
    {
        drawtriangle(renderer, center, 10, 0xff300000);
        //filledCircleRGBA(renderer, center.x, center.y, radius / 3, 0, 0, 0xff, 0xff);
    }
    if (copycolor == color3)
    {
        drawplus(renderer, center, 2, 7, 0xff000040);
    }

    double radius = 10;
    centery = centery - height - radius + 3;

    filledCircleColor(renderer, centerx, centery, radius, 0xff9fd0e6);

    filledCircleColor(renderer, centerx - 4, centery - 3, 3, 0xff000000);
    filledCircleColor(renderer, centerx + 4, centery - 3, 3, 0xff000000);
    thickLineColor(renderer, centerx - 3, centery + 4, centerx + 4, centery + 4, 2, 0xff000000);
}

void printsoldier2(SDL_Renderer *renderer, struct soldier *soldier)
{
    //Uint32 color = soldier->from->maincolor;
    Uint32 color = color1;
    Uint32 copycolor = color;
    if (color == color1)
    {
        color = 0xff007000;
    }
    if (color == color2)
    {
        color = 0xff800000;
    }
    if (color == color3)
    {
        color = 0xff0000A0;
    }

    double height = 17, width1 = 15, width2 = 10, radius = 15;
    double centerx = soldier->x_center;
    double centery = soldier->y_center;

    filledCircleColor(renderer, centerx, centery, radius, 0xff000000);
    filledCircleColor(renderer, centerx, centery, radius - 3, 0xff8fc0d6);

    double distanceoflegs = 5, leglength = 8;
    thickLineColor(renderer, centerx - distanceoflegs, centery + height - 4, centerx - distanceoflegs, centery + height + leglength - 4, 4, 0xff000000);
    thickLineColor(renderer, centerx + distanceoflegs, centery + height - 4, centerx + distanceoflegs, centery + height + leglength - 4, 4, 0xff000000);

    struct point center = {centerx, centery};
    if (copycolor == color1)
    {
        double width = 5;
        boxColor(renderer, center.x - width, center.y - width, center.x + width, center.y + width, 0xff00ff00);
        //filledCircleRGBA(renderer, center.x, center.y, radius / 3, 0, 255, 0, 255);
    }
    if (copycolor == color2)
    {
        drawtriangle(renderer, center, 10, 0xff300000);
        //filledCircleRGBA(renderer, center.x, center.y, radius / 3, 0, 0, 0xff, 0xff);
    }
    if (copycolor == color3)
    {
        drawplus(renderer, center, 2, 7, 0xff0000A0);
    }

    radius = 10;
    centery = centery - height - radius + 5;

    filledCircleColor(renderer, centerx, centery, radius, 0xffB0E0F6);

    filledCircleColor(renderer, centerx - 4, centery - 3, 3, 0xff000000);
    filledCircleColor(renderer, centerx + 4, centery - 3, 3, 0xff000000);
    thickLineColor(renderer, centerx - 3, centery + 4, centerx + 4, centery + 4, 2, 0xff000000);
}