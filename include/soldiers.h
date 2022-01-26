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

    double height = 7, width1 = 4, width2 = 6;
    double centerx = soldier->x_center;
    double centery = soldier->y_center;

    boxColor(renderer, centerx - width1, centery - height, centerx + width1, centery + height, 0xff303030);

    boxColor(renderer, centerx - width2, centery - height, centerx + width2, centery + height, copycolor);

    double distanceoflegs = 5, leglength = 5;
    thickLineColor(renderer, centerx - distanceoflegs, centery + height, centerx - distanceoflegs, centery + height + leglength, 2, 0xff000000);
    thickLineColor(renderer, centerx + distanceoflegs, centery + height, centerx + distanceoflegs, centery + height + leglength, 2, 0xff000000);

    struct point center = {centerx, centery};
    if (color == color1)
    {
        drawtriangle(renderer, center, 4, 0xff6f176c, 0);
        drawtriangle(renderer, center, 4, 0xff6f176c, 3.1415);
    }
    if (color == color2)
    {
        drawtriangle(renderer, center, 4, 0xff300000, 0);
        // filledCircleRGBA(renderer, center.x, center.y, radius / 3, 0, 0, 0xff, 0xff);
    }
    if (color == color3)
    {
        drawplus(renderer, center, 2, 3, 0xff17178D);
    }

    double radius = 5;
    centery = centery - height - radius + 3;

    filledCircleColor(renderer, centerx, centery, radius, 0xff9fd0e6);

    filledCircleColor(renderer, centerx - 2, centery - 2, 2, 0xff000000);
    filledCircleColor(renderer, centerx + 2, centery - 2, 2, 0xff000000);
    thickLineColor(renderer, centerx - 3, centery + 4, centerx + 4, centery + 4, 2, 0xff000000);
}

void find_collison(SDL_Renderer *renderer, struct region *head, int numofsoldiers)
{
    for (int i = 0; i < numofsoldiers; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            for (int a = 0; a < 4; a++)
            {
                if ((head + i)->soldiers[a][j].is_on == 1)
                {
                    for (int k = 0; k < numofsoldiers; k++)
                    {
                        if (k == i)
                        {
                            continue;
                        }
                        else
                        {
                            for (int t = 0; t < 100; t++)
                            {
                                for (int b = 0; b < 4; b++)
                                {
                                    if ((head + k)->soldiers[b][t].is_on == 1)
                                    {
                                        double x0 = (head + i)->soldiers[a][j].x_center;
                                        double y0 = (head + j)->soldiers[a][j].y_center;
                                        double x1 = (head + k)->soldiers[b][t].x_center;
                                        double y1 = (head + k)->soldiers[b][t].y_center;
                                        double distance = 20;
                                        if (x0 - x1 <= distance && x1 - x0 <= distance && y0 - y1 <= distance && y1 - y0 <= distance)
                                        {
                                            //printf("+");
                                            filledCircleColor(renderer, x0, y0, 7, 0xA0000000);
                                            (head + i)->soldiers[a][j].is_on = 0;
                                            (head + k)->soldiers[b][t].is_on = 0;
                                            (head + i)->soldiers[a][j].x_center = (head + i)->x_center;
                                            (head + i)->soldiers[a][j].y_center = (head + i)->y_center;
                                            (head + k)->soldiers[b][t].x_center = (head + k)->x_center;
                                            (head + k)->soldiers[b][t].y_center = (head + k)->y_center;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void start_of_attack(struct region *from, struct region *to)
{
    double speed = 50;
    from->toattacking = to;
    for (int i = 0; i < 1000; i++)
    {
        for (int k = 0; k < 4; k++)
        {
            if (from->soldiers[k][i].is_on == 0)
            {
                from->soldiers[k][i].to = to;

                from->soldiers[k][i].color = from->maincolor;
                double delta_x = (to->x_center - from->x_center);
                double delta_y = (to->y_center - from->y_center);

                double distance = sqrt(delta_x * delta_x + delta_y * delta_y);

                from->soldiers[k][i].vx = speed * delta_x / distance;
                from->soldiers[k][i].vy = speed * delta_y / distance;
                double costeta = delta_y / distance;
                double sinteta = delta_x / distance;

                double difference = (k - 1.5) * 25;

                (from->soldiers[k][i]).x_center = from->x_center - difference * costeta;
                (from->soldiers[k][i]).y_center = from->y_center + difference * sinteta;
            }
        }
    }
}

void attacking(SDL_Renderer *renderer, struct region *head, int numofregion)
{
    find_collison(renderer, head, numofregion);
    for (int i = 0; i < numofregion; i++)
    {
        for (int j = 0; j < 1000; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                if ((head + i)->soldiers[k][j].is_on == 1)
                {
                    if ((head + i)->soldiers[k][j].to->numofsoldiers <= 0)
                    {
                        (head + i)->soldiers[k][j].to->maincolor = (head + i)->maincolor;
                    }
                    printsoldier(renderer, &((head + i)->soldiers[k][j]));
                    (head + i)->soldiers[k][j].x_center += (head + i)->soldiers[k][j].vx;
                    (head + i)->soldiers[k][j].y_center += (head + i)->soldiers[k][j].vy;

                    if (norm((head + i)->soldiers[k][j].x_center - (head + i)->soldiers[k][j].to->x_center) <= 45 && norm((head + i)->soldiers[k][j].y_center - (head + i)->soldiers[k][j].to->y_center) <= 45)
                    {
                        if ((head + i)->soldiers[k][j].to->maincolor != (head + i)->maincolor)
                        {
                            (head + i)->soldiers[k][j].to->numofsoldiers -= 1;
                        }
                        else
                        {
                            (head + i)->soldiers[k][j].to->numofsoldiers += 1;
                        }
                        (head + i)->soldiers[k][j].is_on = 0;
                        (head + i)->soldiers[k][j].x_center = (head + i)->x_center;
                        (head + i)->soldiers[k][j].y_center = (head + i)->y_center;
                    }
                }
            }
        }
    }

    for (int i = 0; i < numofregion; i++)
    {
        if ((head + i)->toattacking != NULL)
        {
            start_of_attack(head + i, (head + i)->toattacking);
            for (int j = 0; j < 1000; j++)
            {
                int flag = 1;
                for (int k = 0; k < 4; k++)
                {
                    if ((head + i)->soldiers[k][j].is_on == 1)
                    {
                        flag = 0;
                        break;
                    }
                }
                if (flag == 1)
                {
                    for (int k = 0; k < 4; k++)
                    {
                        if ((head + i)->numofsoldiers > 0)
                        {
                            (head + i)->soldiers[k][j].is_on = 1;
                            (head + i)->numofsoldiers--;
                        }
                    }
                    break;
                }
            }
        }
    }
    for (int i = 0; i < numofregion; i++)
    {
        if ((head + i)->numofsoldiers <= 0)
        {
            (head + i)->toattacking = NULL;
        }
    }
}