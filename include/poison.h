struct speedbooster
{
    double x;
    double y;
    int is_on;
};

void definespeedbooster(struct speedbooster *head)
{
    for (int i = 0; i < 4; i++)
    {
        (head + i)->is_on = 0;
    }
}

void draw_speedbooster(SDL_Renderer *renderer, struct speedbooster *speedbooster)
{
    if (speedbooster->is_on == 1)
    {
        double x_center = speedbooster->x;
        double y_center = speedbooster->y;
        showimage(renderer, "//home//ilya//Desktop//codes//state.io//photo//speedbooster.bmp", x_center, y_center, 50, 50);
    }
}

void print_speedboosters(SDL_Renderer *renderer, struct speedbooster *head)
{
    for (int i = 0; i < 4; i++)
    {
        draw_speedbooster(renderer, head + i);
    }
}

void change_stateofspeedbooster(struct speedbooster *head)
{
    time_t t;
    srand(time(&t));
    for (int i = 0; i < 4; i++)
    {
        if ((head + i)->is_on == 0)
        {
            int tmp = rand() % 2;
            (head + i)->is_on = tmp;
            if (tmp == 1)
            {
                (head + i)->x = 200 + rand() % 500;
                (head + i)->y = 200 + rand() % 500;
            }
        }
    }
}

void apply_speedboosters(SDL_Renderer *renderer, struct speedbooster *head_booter, struct region *headregion, int numofregion)
{
    for (int z = 0; z < 4; z++)
    {
        if ((head_booter + z)->is_on == 1)
        {
            for (int i = 0; i < numofregion; i++)
            {
                for (int j = 0; j < 200; j++)
                {
                    for (int k = 0; k < 4; k++)
                    {
                        if ((headregion + i)->soldiers[k][j].is_on == 1)
                        {
                            double x_region = (headregion + i)->soldiers[k][j].x_center;
                            double y_region = (headregion + i)->soldiers[k][j].y_center;

                            double x_booster = (head_booter + z)->x;
                            double y_booster = (head_booter + z)->y;
                            if (norm(x_region - x_booster) < 30 && norm(y_region - y_booster) < 30)
                            {
                                if(speedbooster[(headregion + i)->side] == 0)
                                {
                                    int time_booter = 10;
                                    int tmp = (headregion + i)->side;
                                    speedbooster[tmp] = time_booter;
                                    (head_booter + z)->is_on = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void decrease_poison()
{
    for(int i=0; i<3; i++)
    {
        if(speedbooster[i] > 0)
        {
            speedbooster[i]--;
        }
    }
}

void all_of_speedboosters(SDL_Renderer *renderer, struct speedbooster *head, struct region *headregion, int numofregion)
{
    change_stateofspeedbooster(head);
    print_speedboosters(renderer, head);
    apply_speedboosters(renderer, head, headregion, numofregion);
    decrease_poison();
}