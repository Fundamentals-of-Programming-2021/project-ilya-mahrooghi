void printsoldier(SDL_Renderer *renderer, struct soldier *soldier)
{
    Uint32 color = soldier->color;
    Uint32 copycolor = color;

    if (color == color1)
    {
        copycolor = 0xffee04e6;
    }
    else if (color == color2)
    {
        copycolor = 0xfff0240d;
    }
    else if (color == color3)
    {
        copycolor = 0xff1010f7;
    }

    double height = 10, width1 = 12, width2 = 7;
    double centerx = soldier->x_center;
    double centery = soldier->y_center;

    boxColor(renderer, centerx - width1, centery - height, centerx + width1, centery + height, 0xff303030);

    boxColor(renderer, centerx - width2, centery - height, centerx + width2, centery + height, copycolor);

    double distanceoflegs = 5, leglength = 5;
    thickLineColor(renderer, centerx - distanceoflegs, centery + height, centerx - distanceoflegs, centery + height + leglength, 4, 0xff000000);
    thickLineColor(renderer, centerx + distanceoflegs, centery + height, centerx + distanceoflegs, centery + height + leglength, 4, 0xff000000);

    struct point center = {centerx, centery};
    if (color == color1)
    {
        drawtriangle(renderer, center, 5, 0xff6f176c, 0);
        drawtriangle(renderer, center, 5, 0xff6f176c, 3.1415);
    }
    if (color == color2)
    {
        drawtriangle(renderer, center, 5, 0xff300000, 0);
        //filledCircleRGBA(renderer, center.x, center.y, radius / 3, 0, 0, 0xff, 0xff);
    }
    if (color == color3)
    {
       drawplus(renderer, center, 2, 5, 0xff17178D);
    }

    double radius = 10;
    centery = centery - height - radius + 3;

    filledCircleColor(renderer, centerx, centery, radius, 0xff9fd0e6);

    filledCircleColor(renderer, centerx - 4, centery - 3, 3, 0xff000000);
    filledCircleColor(renderer, centerx + 4, centery - 3, 3, 0xff000000);
    thickLineColor(renderer, centerx - 3, centery + 4, centerx + 4, centery + 4, 2, 0xff000000);
}

void start_of_attack(struct region *from, struct region *to)
{
    double speed = 10;
    from->toattacking = to;
    for (int i = 0; i < 1000; i++)
    {
        if (from->soldiers[i].is_on == 0)
        {

            (from->soldiers[i]).x_center = from->x_center;
            (from->soldiers[i]).y_center = from->y_center;

            from->soldiers[i].to = to;

            from->soldiers[i].color = from->maincolor;

            from->soldiers[i].vx = (to->x_center - from->x_center) / speed;
            from->soldiers[i].vy = (to->y_center - from->y_center) / speed;
        }
    }
}

void attacking(SDL_Renderer *renderer, struct region *head, int numofsoldiers)
{
    for (int i = 0; i < numofsoldiers; i++)
    {
        for (int j = 0; j < 1000; j++)
        {
            if ((head + i)->soldiers[j].is_on == 1)
            {
                if ((head + i)->soldiers[j].to->numofsoldiers <= 0)
                {
                    (head + i)->soldiers[j].to->maincolor = (head + i)->maincolor;
                }
                printsoldier(renderer, &((head + i)->soldiers[j]));
                (head + i)->soldiers[j].x_center += (head + i)->soldiers[j].vx;
                (head + i)->soldiers[j].y_center += (head + i)->soldiers[j].vy;

                if (norm((head + i)->soldiers[j].x_center - (head + i)->soldiers[j].to->x_center) <= 1 && norm((head + i)->soldiers[j].y_center - (head + i)->soldiers[j].to->y_center) <= 1)
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
            for (int j = 0; j < 1000; j++)
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