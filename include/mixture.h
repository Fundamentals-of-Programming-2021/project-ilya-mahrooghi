struct speedbooster
{
    double x;
    double y;
    int is_on;
};

struct freeze
{
    double x;
    double y;
    int is_on;
};

void define_mixtures(struct speedbooster *head, struct freeze *head2)
{
    for (int i = 0; i < 4; i++)
    {
        (head + i)->is_on = 0;
        (head2 + i)->is_on = 0;
    }
}
/////////////////////////////////
void draw_speedbooster(SDL_Renderer *renderer, struct speedbooster *speedbooster)
{
    if (speedbooster->is_on == 1)
    {
        double x_center = speedbooster->x;
        double y_center = speedbooster->y;
        showimage(renderer, "//home//ilya//Desktop//codes//state.io//photo//speedbooster.bmp", x_center, y_center, 50, 50);
    }
}

void draw_freeze(SDL_Renderer *renderer, struct freeze *freeze)
{
    if (freeze->is_on == 1)
    {
        double x_center = freeze->x;
        double y_center = freeze->y;
        showimage(renderer, "//home//ilya//Desktop//codes//state.io//photo//freeze.bmp", x_center, y_center, 50, 50);
    }
}

void print_mixtures(SDL_Renderer *renderer, struct speedbooster *head_speedbooster, struct freeze *head_freeze)
{
    for (int i = 0; i < 4; i++)
    {
        draw_speedbooster(renderer, head_speedbooster + i);
        draw_freeze(renderer, head_freeze + i);
    }
}
/////////////////////////////////
void randomly_mixture(struct speedbooster *head_speedbooster, struct freeze *head_freeze)
{
    for (int i = 0; i < 4; i++)
    {
        if ((head_speedbooster + i)->is_on == 0)
        {
            int tmp = rand();

            if (tmp % 10 == 1)
            {
                (head_speedbooster + i)->is_on = 1;
                int tmp2 = abs(rand() % 500);
                int tmp3 = abs(rand() % 500);
                (head_speedbooster + i)->x = 250 + tmp2;
                (head_speedbooster + i)->y = 250 + tmp3;
            }
        }
        if ((head_freeze + i)->is_on == 0)
        {
            int tmp = rand();

            if (tmp % 50 == 1)
            {
                (head_freeze + i)->is_on = 1;
                int tmp2 = abs(rand() % 500);
                int tmp3 = abs(rand() % 500);
                (head_freeze + i)->x = 250 + tmp2;
                (head_freeze + i)->y = 250 + tmp3;
            }
        }
    }
}

void apply_mixtures(struct speedbooster *head_booster, struct freeze *head_freeze, struct region *headregion, int numofregion)
{
    for (int i = 0; i < numofregion; i++)
    {
        for (int j = 0; j < 200; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                if ((headregion + i)->soldiers[k][j].is_on == 1)
                {
                    for (int z = 0; z < 4; z++)
                    {
                        if ((head_booster + z)->is_on == 1)
                        {
                            double x_region = (headregion + i)->soldiers[k][j].x_center;
                            double y_region = (headregion + i)->soldiers[k][j].y_center;

                            double x_booster = (head_booster + z)->x;
                            double y_booster = (head_booster + z)->y;
                            if (norm(x_region - x_booster) < 70 && norm(y_region - y_booster) < 70)
                            {
                                if (freeze[(headregion + i)->side] == 0 && speedbooster[(headregion + i)->side] == 0)
                                {
                                    int time_booster = 10;
                                    int tmp = (headregion + i)->side;
                                    speedbooster[tmp] = time_booster;
                                    (head_booster + z)->is_on = 0;
                                }
                            }
                        }
                        /////////////////////////////////
                        if ((head_freeze + z)->is_on == 1)
                        {
                            double x_region = (headregion + i)->soldiers[k][j].x_center;
                            double y_region = (headregion + i)->soldiers[k][j].y_center;

                            double x_freeze = (head_freeze + z)->x;
                            double y_freeze = (head_freeze + z)->y;
                            if (norm(x_region - x_freeze) < 70 && norm(y_region - y_freeze) < 70)
                            {
                                if (freeze[(headregion + i)->side] == 0 && speedbooster[(headregion + i)->side] == 0)
                                {
                                    int time_booster = 10;
                                    int tmp = (headregion + i)->side;
                                    freeze[tmp] = time_booster;
                                    (head_freeze + z)->is_on = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void decrease_timeofmixture()
{
    for (int i = 0; i < 3; i++)
    {
        if (speedbooster[i] > 0)
        {
            speedbooster[i]--;
        }
        if (freeze[i] > 0)
        {
            freeze[i]--;
        }
    }
}

void all_of_mixtures(SDL_Renderer *renderer, struct speedbooster *head_speedbooster, struct freeze *head_freeze, struct region *head_region, int numofregion)
{
    apply_mixtures(head_speedbooster, head_freeze, head_region, numofregion);
    print_mixtures(renderer, head_speedbooster, head_freeze);
    decrease_timeofmixture();
    randomly_mixture(head_speedbooster, head_freeze);
}
