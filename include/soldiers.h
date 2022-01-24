
double speed = 15;

void printsoldier(SDL_Renderer *renderer, struct soldier *soldier)
{
    Uint32 color = soldier->color;
    Uint32 copycolor = color;

    if (color == color1)
    {
        copycolor = 0xff00E000;
    }
    else if (color == color2)
    {
        copycolor = 0xffff0000;
    }
    else if (color == color3)
    {
        copycolor = 0xff0000ff;
    }

    double height = 17, width1 = 15, width2 = 10;
    double centerx = soldier->x_center;
    double centery = soldier->y_center;

    boxColor(renderer, centerx - width1, centery - height, centerx + width1, centery + height, 0xff000000);

    boxColor(renderer, centerx - width2, centery - height, centerx + width2, centery + height, copycolor);

    double distanceoflegs = 5, leglength = 5;
    thickLineColor(renderer, centerx - distanceoflegs, centery + height, centerx - distanceoflegs, centery + height + leglength, 4, 0xff000000);
    thickLineColor(renderer, centerx + distanceoflegs, centery + height, centerx + distanceoflegs, centery + height + leglength, 4, 0xff000000);

    struct point center = {centerx, centery};
    if (color == color1)
    {
        double width = 5;
        boxColor(renderer, center.x - width, center.y - width, center.x + width, center.y + width, 0xff00ff00);
        //filledCircleRGBA(renderer, center.x, center.y, radius / 3, 0, 255, 0, 255);
    }
    if (color == color2)
    {
        drawtriangle(renderer, center, 10, 0xff300000);
        //filledCircleRGBA(renderer, center.x, center.y, radius / 3, 0, 0, 0xff, 0xff);
    }
    if (color == color3)
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

void start_of_attack(struct region *from, struct region *to)
{
    from->toattacking = to;
    for (int i = 0; i < 1000; i++)
    {
        (from->soldiers[i]).is_on = 0;

        (from->soldiers[i]).x_center = from->x_center;
        (from->soldiers[i]).y_center = from->y_center;

        from->soldiers[i].to = to;

        from->soldiers[i].color = from->maincolor;

        from->soldiers[i].vx = (to->x_center - from->x_center) / speed;
        from->soldiers[i].vy = (to->y_center - from->y_center) / speed;
    }
    from->soldiers[0].is_on = 1;
}

void attacking(SDL_Renderer *renderer, struct region *head, int numofsoldiers)
{
    for (int i = 0; i < numofsoldiers; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            if ((head + i)->soldiers[j].is_on == 1)
            {
                printsoldier(renderer, &((head + i)->soldiers[j]));
                (head + i)->soldiers[j].x_center += (head + i)->soldiers[j].vx;
                (head + i)->soldiers[j].y_center += (head + i)->soldiers[j].vy;

                if (norm((head + i)->soldiers[j].x_center - (head + i)->soldiers[j].to->x_center) <= 2)
                {
                    if ((head + i)->soldiers[j].to->maincolor != (head + i)->maincolor)
                    {
                        (head + i)->soldiers[j].to->numofsoldiers -= 1;
                    }
                    else
                    {
                        (head + i)->soldiers[j].to->numofsoldiers += 1;
                    }

                    (head + i)->soldiers[j].is_on = 0;
                    (head + i)->soldiers[j].x_center = (head + i)->x_center;
                    (head + i)->soldiers[j].y_center = (head + i)->y_center;
                }
            }
        }
    }
    for (int i = 0; i < numofsoldiers; i++)
    {
        if ((head + i)->toattacking != NULL)
        {
            for (int j = 0; j < 100; j++)
            {
                if ((head + i)->soldiers[j].is_on == 0 && (head + i)->numofsoldiers > 0)
                {
                    (head + i)->numofsoldiers -= 1;
                    (head + i)->soldiers[j].is_on = 1;
                    break;
                }
            }
        }
    }
    for (int i = 0; i < numofsoldiers; i++)
    {
        if ((head + i)->numofsoldiers <= 0)
        {
            (head + i)->toattacking = NULL;
        }
    }
}