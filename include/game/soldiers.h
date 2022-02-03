// functions
void printsoldier(SDL_Renderer *renderer, struct soldier *soldier)
{
    Uint32 color = soldier->color;
    double tmp = 20;
    if (color == color1)
    {
        showimage(renderer, "//home//ilya//Desktop//codes//state.io//photo//game//background//neptune.bmp", soldier->x_center, soldier->y_center, tmp, tmp);
    }
    else if (color == color2)
    {
        showimage(renderer, "//home//ilya//Desktop//codes//state.io//photo//game//background//earth.bmp", soldier->x_center, soldier->y_center, tmp, tmp);
    }
    else if (color == color3)
    {
        showimage(renderer, "//home//ilya//Desktop//codes//state.io//photo//game//background//venus.bmp", soldier->x_center, soldier->y_center, tmp, tmp);
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
                        if (k == i)
                        {
                            continue;
                        }
                        
                        if ((head + k)->maincolor != (head + i)->maincolor)
                        {
                            for (int t = 0; t < 200; t++)
                            {
                                for (int b = 0; b < 4; b++)
                                {
                                    if ((head + k)->soldiers[b][t].is_on == 1)
                                    {
                                        // define the centers of two soldiers
                                        // first center
                                        double x0 = (head + i)->soldiers[a][j].x_center;
                                        double y0 = (head + j)->soldiers[a][j].y_center;

                                        // second center
                                        double x1 = (head + k)->soldiers[b][t].x_center;
                                        double y1 = (head + k)->soldiers[b][t].y_center;

                                        // define distance and if statement
                                        // double distance = 15;
                                        // if (norm(x0 - x1) <= distance && norm(y0 - y1) <= distance)
                                        if (distance(x0, y0, x1, y1) <= 40)
                                        {
                                            // showing the collison
                                            filledCircleColor(renderer, x0, y0, 7, 0xA0000000);

                                            // make them off
                                            (head + i)->soldiers[a][j].is_on = 0;
                                            (head + k)->soldiers[b][t].is_on = 0;

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
    // define the attacking in struct
    from->toattacking = to;

    // make all off soldiers ready to attack to the enemy
    for (int i = 0; i < 200; i++)
    {
        for (int k = 0; k < 4; k++)
        {
            if (from->soldiers[k][i].is_on == 0)
            {
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
                if (speedbooster[from->side] != 0)
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
                    int tmp_side = (head + i)->side;

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
        if ((head + i)->toattacking != NULL)
        {
            // update attacking target
            start_of_attack(head + i, (head + i)->toattacking);
            // use speedbooster mixture
            if (speedbooster[(head + i)->side] != 0)
            {
                for (int j = 0; j < 200; j++)
                {
                    for (int k = 0; k < 4; k++)
                    {
                        (head + i)->soldiers[k][i].current_vx = 2.5 * (head + i)->soldiers[k][i].vx;
                        (head + i)->soldiers[k][i].current_vy = 2.5 * (head + i)->soldiers[k][i].vy;
                    }
                }
            }
        }
    }

    for (int i = 0; i < numofregion; i++)
    {
        if ((head + i)->toattacking != NULL)
        {
            int tmp_side = (head + i)->side;

            // check if the enemy freeze this side or not
            if (freeze[(tmp_side + 1) % 3] == 0 && freeze[(tmp_side + 2) % 3] == 0)
            {

                for (int j = 0; j < 200; j++)
                {
                    // flag is 1 when all of the array at j is free
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
                        // if it is free ready to attack
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
    }

    for (int i = 0; i < numofregion; i++)
    {
        // if you dont have soldier you cant attack
        if ((head + i)->numofsoldiers <= 0)
        {
            (head + i)->toattacking = NULL;
        }
    }
}
