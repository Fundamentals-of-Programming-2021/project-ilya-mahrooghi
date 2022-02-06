// functions
void printsoldier(SDL_Renderer *renderer, struct soldier *soldier)
{
    Uint32 color = soldier->color;
    double tmp = 20;
    if (color == color1)
    {
        showimage(renderer, "..//photo//game//background//neptune.bmp", soldier->x_center, soldier->y_center, tmp, tmp);
    }
    else if (color == color2)
    {
        showimage(renderer, "..//photo//game//background//earth.bmp", soldier->x_center, soldier->y_center, tmp, tmp);
    }
    else if (color == color3)
    {
        showimage(renderer, "..//photo//game//background//venus.bmp", soldier->x_center, soldier->y_center, tmp, tmp);
    }
}

void find_collison(SDL_Renderer *renderer, struct region *head, int numofsoldiers)
{
    // start search first soldier
    for (int i = 0; i < numofsoldiers; i++)
    {
        for (int j = 0; j < 200; j++)
        {
            for (int a = 0; a < 4; a++)
            {
                if ((head + i)->soldiers[a][j].is_on == 1)
                {
                    // start for searching another one
                    for (int k = 0; k < numofsoldiers; k++)
                    {
                        for (int t = 0; t < 200; t++)
                        {
                            for (int b = 0; b < 4; b++)
                            {
                                if ((head + k)->soldiers[b][t].is_on == 1)
                                {
                                    if ((head + i)->soldiers[a][j].color != (head + k)->soldiers[b][t].color)
                                    {
                                        // define the centers of two soldiers
                                        // first center
                                        double x0 = (head + i)->soldiers[a][j].x_center;
                                        double y0 = (head + i)->soldiers[a][j].y_center;

                                        // second center
                                        double x1 = (head + k)->soldiers[b][t].x_center;
                                        double y1 = (head + k)->soldiers[b][t].y_center;

                                        if (norm(x0 - x1) < 18 && norm(y0 - y1) < 18)
                                        {
                                            // showing the collison
                                            filledCircleColor(renderer, x0 + 10, y0 + 10, 7, 0xA0000000);
                                            filledCircleColor(renderer, x1 + 10, y1 + 10, 7, 0xA0000000);

                                            // make them off
                                            (head + i)->soldiers[a][j].is_on = 0;
                                            (head + k)->soldiers[b][t].is_on = 0;
                                            (head + i)->soldiers[a][j].to = NULL;
                                            (head + k)->soldiers[b][t].to = NULL;

                                            // get them back to home
                                            // first
                                            (head + i)->soldiers[a][j].x_center = (head + i)->x_center;
                                            (head + i)->soldiers[a][j].y_center = (head + i)->y_center;

                                            // second
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
    double speed = 16; // speed of soldiers

    // numofsoldiers
    int numofsoldiers = from->numofsoldiers;
    for (int j = 0; j < 200; j++)
    {
        for (int k = 0; k < 4; k++)
        {
            if (from->soldiers[k][j].to != NULL && from->soldiers[k][j].is_on == 0)
            {
                numofsoldiers--;
            }
        }
    }
    int count = 0;

    // make all off soldiers ready to attack to the enemy
    for (int i = 0; i < 200; i++)
    {
        int flag = 1;
        for (int k = 0; k < 4; k++)
        {
            if (from->soldiers[k][i].to != NULL || from->soldiers[k][i].is_on == 1)
            {
                flag = 0;
                break;
            }
        }
        if (flag)
        {
            for (int k = 0; k < 4; k++)
            {
                if (count < numofsoldiers)
                {
                    count++;
                    // define enemy to attack
                    from->soldiers[k][i].to = to;

                    // define color
                    from->soldiers[k][i].color = from->maincolor;

                    // define difference pf x and y and distance from enemy regions
                    double delta_x = (to->x_center - from->x_center);
                    double delta_y = (to->y_center - from->y_center);
                    double distance = sqrt(delta_x * delta_x + delta_y * delta_y);

                    // define speed
                    from->soldiers[k][i].vx = speed * (delta_x / distance);
                    from->soldiers[k][i].vy = speed * (delta_y / distance);

                    // define the speed after speeedbooster
                    from->soldiers[k][i].current_vx = from->soldiers[k][i].vx;
                    from->soldiers[k][i].current_vy = from->soldiers[k][i].vy;
                    int tmp_side = from->side;

                    if (speedbooster[tmp_side] != 0)
                    {
                        from->soldiers[k][i].current_vx = 2.5 * from->soldiers[k][i].vx;
                        from->soldiers[k][i].current_vy = 2.5 * from->soldiers[k][i].vy;
                    }

                    // define the center of the soldier at the first of attack
                    //  define the angles
                    double costeta = delta_y / distance;
                    double sinteta = delta_x / distance;

                    // define the distance between the soldiers in one line
                    double distanceof_soldiers = (k - 1.5) * 25;

                    // define center
                    (from->soldiers[k][i]).x_center = from->x_center - distanceof_soldiers * costeta;
                    (from->soldiers[k][i]).y_center = from->y_center + distanceof_soldiers * sinteta;
                }
            }
        }
    }
}

void delete_soldiers_ifend(struct region *head, int numofregion)
{
    for (int i = 0; i < numofregion; i++)
    {
        if ((head + i)->numofsoldiers == 0)
        {
            for (int j = 0; j < 200; j++)
            {
                for (int k = 0; k < 4; k++)
                {
                    if ((head + i)->soldiers[k][j].is_on == 0)
                    {
                        (head + i)->soldiers[k][j].to = NULL;
                    }
                }
            }
        }
    }
}

void attacking(SDL_Renderer *renderer, struct region *head, int numofregion)
{
    // find collison
    find_collison(renderer, head, numofregion);

    // find soldiers and move them
    for (int i = 0; i < numofregion; i++)
    {
        for (int j = 0; j < 200; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                if ((head + i)->soldiers[k][j].is_on == 1)
                {
                    // printsoldier
                    printsoldier(renderer, &((head + i)->soldiers[k][j]));

                    // change the center of soldier if it is not freezed
                    int tmp_side;
                    Uint32 color = (head + i)->soldiers[k][j].color;
                    if (color == color1)
                    {
                        tmp_side = 0;
                    }
                    if (color == color2)
                    {
                        tmp_side = 1;
                    }
                    if (color == color3)
                    {
                        tmp_side = 2;
                    }

                    if (speedbooster[tmp_side] != 0)
                    {
                        (head + i)->soldiers[k][j].current_vx = 2.5 * (head + i)->soldiers[k][j].vx;
                        (head + i)->soldiers[k][j].current_vy = 2.5 * (head + i)->soldiers[k][j].vy;
                    }
                    else
                    {
                        (head + i)->soldiers[k][j].current_vx = (head + i)->soldiers[k][j].vx;
                        (head + i)->soldiers[k][j].current_vy = (head + i)->soldiers[k][j].vy;
                    }

                    // check if the enemy freeze this side or not
                    if (freeze[(tmp_side + 1) % 3] == 0 && freeze[(tmp_side + 2) % 3] == 0)
                    {
                        (head + i)->soldiers[k][j].x_center += (head + i)->soldiers[k][j].current_vx;
                        (head + i)->soldiers[k][j].y_center += (head + i)->soldiers[k][j].current_vy;
                    }

                    // check is enemy arrived to target region
                    if (distance((head + i)->soldiers[k][j].x_center, (head + i)->soldiers[k][j].y_center, (head + i)->soldiers[k][j].to->x_center, (head + i)->soldiers[k][j].to->y_center) <= 45)
                    {
                        // check that if the enemy region destroyed or not
                        if ((head + i)->soldiers[k][j].to->numofsoldiers <= 0)
                        {
                            (head + i)->soldiers[k][j].to->maincolor = (head + i)->soldiers[k][j].color;
                            (head + i)->soldiers[k][j].to->side = (head + i)->side;
                            (head + i)->soldiers[k][j].to->toattacking = NULL;
                        }

                        // check the target region is enemy or not and then change the numofsoldiers
                        if ((head + i)->soldiers[k][j].to->maincolor != (head + i)->soldiers[k][j].color)
                        {
                            (head + i)->soldiers[k][j].to->numofsoldiers -= 1;
                        }
                        else
                        {
                            (head + i)->soldiers[k][j].to->numofsoldiers += 1;
                        }

                        // turn off
                        (head + i)->soldiers[k][j].is_on = 0;
                        (head + i)->soldiers[k][j].to = NULL;

                        // get back to home
                        (head + i)->soldiers[k][j].x_center = (head + i)->x_center;
                        (head + i)->soldiers[k][j].y_center = (head + i)->y_center;
                    }
                }
            }
        }
    }

    for (int i = 0; i < numofregion; i++)
    {
        int tmp_side = (head + i)->side;

        // check if the enemy freeze this side or not
        if (freeze[(tmp_side + 1) % 3] == 0 && freeze[(tmp_side + 2) % 3] == 0)
        {
            struct region attacking[100];
            int index = 0;
            for (int j = 0; j < 200; j++)
            {
                int flag1 = 1;
                for (int k = 0; k < 4; k++)
                {
                    if ((head + i)->soldiers[k][j].is_on == 1)
                    {
                        flag1 = 0;
                        break;
                    }
                }
                int flag3 = 0;
                for (int k = 0; k < 4; k++)
                {
                    if ((head + i)->soldiers[k][j].to != NULL)
                    {
                        flag3 = 1;
                        break;
                    }
                }
                if (flag1 && flag3)
                {
                    int flag = 1;
                    for (int z = 0; z < index; z++)
                    {
                        for (int k = 0; k < 4; k++)
                        {
                            if ((head + i)->soldiers[k][j].to != NULL)
                            {
                                if ((head + i)->soldiers[k][j].to->x_center == attacking[z].x_center)
                                {
                                    if ((head + i)->soldiers[k][j].to->y_center == attacking[z].y_center)
                                    {
                                        flag = 0;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    if (flag)
                    {
                        for (int k = 0; k < 4; k++)
                        {
                            if ((head + i)->soldiers[k][j].to != NULL)
                            {
                                attacking[index].x_center = (head + i)->soldiers[k][j].to->x_center;
                                attacking[index].y_center = (head + i)->soldiers[k][j].to->y_center;
                                break;
                            }
                        }
                        index++;

                        for (int k = 0; k < 4; k++)
                        {
                            if ((head + i)->soldiers[k][j].to != NULL)
                            {
                                if ((head + i)->numofsoldiers > 0)
                                {
                                    (head + i)->soldiers[k][j].is_on = 1;
                                    (head + i)->numofsoldiers--;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    delete_soldiers_ifend(head, numofregion);
}
