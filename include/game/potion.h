// struct of mixtures
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

struct inf_soldiers
{
    double x;
    double y;
    int is_on;
};

struct more_soldiers
{
    double x;
    double y;
    int is_on;
};

// functions
void define_mixtures(struct speedbooster *head_speedbooster, struct freeze *head_freeze, struct inf_soldiers *head_inf_soldiers, struct more_soldiers *head_more_soldiers)
{
    for (int i = 0; i < 4; i++)
    {
        (head_speedbooster + i)->is_on = 0;
        (head_freeze + i)->is_on = 0;
        (head_inf_soldiers + i)->is_on = 0;
        (head_more_soldiers + i)->is_on = 0;
    }
}

// show the potions
void draw_speedbooster(SDL_Renderer *renderer, struct speedbooster *speedbooster)
{
    if (speedbooster->is_on == 1)
    {
        double x_center = speedbooster->x;
        double y_center = speedbooster->y;
        showimage(renderer, "..//photo//game//potion//speedbooster.bmp", x_center, y_center, 50, 50);
    }
}

void draw_freeze(SDL_Renderer *renderer, struct freeze *freeze)
{
    if (freeze->is_on == 1)
    {
        double x_center = freeze->x;
        double y_center = freeze->y;
        showimage(renderer, "..//photo//game//potion//freeze.bmp", x_center, y_center, 50, 50);
    }
}

void draw_inf_soldiers(SDL_Renderer *renderer, struct inf_soldiers *inf_soldiers)
{
    if (inf_soldiers->is_on == 1)
    {
        double x_center = inf_soldiers->x;
        double y_center = inf_soldiers->y;
        showimage(renderer, "..//photo//game//potion//inf_soldiers.bmp", x_center, y_center, 50, 50);
    }
}

void draw_more_soldiers(SDL_Renderer *renderer, struct more_soldiers *more_soldiers)
{
    if (more_soldiers->is_on == 1)
    {
        double x_center = more_soldiers->x;
        double y_center = more_soldiers->y;
        showimage(renderer, "..//photo//game//potion//more_soldiers.bmp", x_center, y_center, 90, 90);
    }
}

void print_mixtures(SDL_Renderer *renderer, struct speedbooster *head_speedbooster, struct freeze *head_freeze, struct inf_soldiers *head_inf_soldiers, struct more_soldiers *head_more_soldiers)
{
    for (int i = 0; i < 4; i++)
    {
        draw_speedbooster(renderer, head_speedbooster + i);
        draw_freeze(renderer, head_freeze + i);
        draw_inf_soldiers(renderer, head_inf_soldiers + i);
        draw_more_soldiers(renderer, head_more_soldiers + i);
    }
}

// logic of the mixtures
int is_between_regions(double x0, double y0, double x1, double y1, double x, double y)
{
    int flag = 1;
    if (!((x > x0 && x < x1) || (x < x0 && x > x1)))
    {
        flag = 0;
    }
    if (!((y > y0 && y < y1) || (y < y0 && y > y1)))
    {
        flag = 0;
    }
    double distance_x0x1 = distance(x0, y0, x1, y1);
    double distance_x0x = distance(x0, y0, x, y);
    double distance_x1x = distance(x1, y1, x, y);
    double p = (distance_x0x1 + distance_x0x + distance_x1x) / 2;
    double area = sqrt(p * (p - distance_x0x1) * (p - distance_x1x) * (p - distance_x0x));
    if ((2 * area) / distance_x0x1 > 20)
    {
        flag = 0;
    }
    return flag;
}

int is_accessible(double x, double y, struct region *head, int numofregion)
{
    for (int i = 0; i < numofregion; i++)
    {
        for (int j = i + 1; j < numofregion; j++)
        {
            if ((head + i)->maincolor != backgroundColor && (head + j)->maincolor != backgroundColor)
            {
                if ((head + i)->maincolor != neutralColor || (head + j)->maincolor != neutralColor)
                {
                    double x0 = (head + i)->x_center;
                    double y0 = (head + i)->y_center;

                    double x1 = (head + j)->x_center;
                    double y1 = (head + j)->y_center;

                    if (is_between_regions(x0, y0, x1, y1, x, y))
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

void randomly_mixture(struct speedbooster *head_speedbooster, struct freeze *head_freeze, struct inf_soldiers *head_inf_soldiers, struct more_soldiers *head_more_soldiers, struct region *head_region, int numofregion)
{
    for (int i = 0; i < 4; i++)
    {
        int possibiity = 100;

        // speedbooster
        if ((head_speedbooster + i)->is_on == 0)
        {
            int tmp = rand();
            // the possibiity
            if (tmp % possibiity == 0)
            {

                // choose randomly x and y of mixture
                double tmp2 = abs(rand() % 1000);
                double tmp3 = abs(rand() % 1000);

                if (is_accessible(tmp2 + 25, tmp3 + 25, head_region, numofregion))
                {
                    // make it on
                    (head_speedbooster + i)->is_on = 1;
                    (head_speedbooster + i)->x = tmp2 + 25;
                    (head_speedbooster + i)->y = tmp3 + 25;
                }
            }
        }

        // freeze
        if ((head_freeze + i)->is_on == 0)
        {
            int tmp = rand();
            // the possibiity
            if (tmp % possibiity == 0)
            {

                // choose randomly x and y of mixture
                double tmp2 = abs(rand() % 1000);
                double tmp3 = abs(rand() % 1000);

                if (is_accessible(tmp2 + 25, tmp3 + 25, head_region, numofregion))
                {
                    // make it on
                    (head_freeze + i)->is_on = 1;
                    (head_freeze + i)->x = tmp2 + 25;
                    (head_freeze + i)->y = tmp3 + 25;
                }
            }
        }

        // inf_soldiers
        if ((head_inf_soldiers + i)->is_on == 0)
        {
            int tmp = rand();
            // the possibiity
            if (tmp % possibiity == 0)
            {

                // choose randomly x and y of mixture
                double tmp2 = abs(rand() % 1000);
                double tmp3 = abs(rand() % 1000);

                if (is_accessible(tmp2 + 25, tmp3 + 25, head_region, numofregion))
                {
                    // make it on
                    (head_inf_soldiers + i)->is_on = 1;
                    (head_inf_soldiers + i)->x = tmp2 + 25;
                    (head_inf_soldiers + i)->y = tmp3 + 25;
                }
            }
        }

        // more_soldiers
        if ((head_more_soldiers + i)->is_on == 0)
        {
            int tmp = rand();
            // the possibiity
            if (tmp % possibiity == 0)
            {

                // choose randomly x and y of mixture
                double tmp2 = abs(rand() % 1000);
                double tmp3 = abs(rand() % 1000);

                if (is_accessible(tmp2 + 45, tmp3 + 45, head_region, numofregion))
                {
                    // make it on
                    (head_more_soldiers + i)->is_on = 1;
                    (head_more_soldiers + i)->x = tmp2 + 45;
                    (head_more_soldiers + i)->y = tmp3 + 45;
                }
            }
        }
    }
}

void apply_mixtures(struct speedbooster *head_booster, struct freeze *head_freeze, struct inf_soldiers *head_inf_soldiers, struct more_soldiers *head_more_soldiers, struct region *headregion, int numofregion)
{
    // search for the soldiers
    for (int i = 0; i < numofregion; i++)
    {
        for (int j = 0; j < 200; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                if ((headregion + i)->soldiers[k][j].is_on == 1)
                {
                    for (int z = 0; z < 4; z++) // search for the mixtures
                    {
                        int sidenum;

                        Uint32 color = (headregion + i)->soldiers[k][j].color;
                        if (color == color1)
                        {
                            sidenum = 0;
                        }
                        if (color == color2)
                        {
                            sidenum = 1;
                        }
                        if (color == color3)
                        {
                            sidenum = 2;
                        }
                        // speedbooster
                        if ((head_booster + z)->is_on == 1)
                        {
                            // x and y of center of soldier
                            double x_soldier = (headregion + i)->soldiers[k][j].x_center;
                            double y_soldier = (headregion + i)->soldiers[k][j].y_center;

                            // x and y of booster
                            double x_booster = (head_booster + z)->x;
                            double y_booster = (head_booster + z)->y;

                            if (norm(x_soldier - x_booster) < 50 && norm(y_soldier - y_booster) < 50)
                            {
                                if (freeze[sidenum] == 0 && speedbooster[sidenum] == 0 && inf_soldiers[sidenum] == 0 && more_soldiers[sidenum] == 0)
                                {
                                    int time_booster = 40;
                                    speedbooster[sidenum] = time_booster;
                                    (head_booster + z)->is_on = 0;
                                }
                            }
                        }

                        // freeze
                        if ((head_freeze + z)->is_on == 1)
                        {
                            // x and y of soldier
                            double x_soldier = (headregion + i)->soldiers[k][j].x_center;
                            double y_soldier = (headregion + i)->soldiers[k][j].y_center;

                            // x and y of freeze
                            double x_freeze = (head_freeze + z)->x;
                            double y_freeze = (head_freeze + z)->y;

                            if (norm(x_soldier - x_freeze) < 50 && norm(y_soldier - y_freeze) < 50)
                            {
                                if (freeze[sidenum] == 0 && speedbooster[sidenum] == 0 && inf_soldiers[sidenum] == 0 && more_soldiers[sidenum] == 0)
                                {
                                    int time_booster = 20;
                                    freeze[sidenum] = time_booster;
                                    (head_freeze + z)->is_on = 0;
                                }
                            }
                        }

                        // inf_soldiers
                        if ((head_inf_soldiers + z)->is_on == 1)
                        {
                            // x and y of soldier
                            double x_soldier = (headregion + i)->soldiers[k][j].x_center;
                            double y_soldier = (headregion + i)->soldiers[k][j].y_center;

                            // x and y of freeze
                            double x_inf_soldiers = (head_inf_soldiers + z)->x;
                            double y_inf_soldiers = (head_inf_soldiers + z)->y;

                            if (norm(x_soldier - x_inf_soldiers) < 50 && norm(y_soldier - y_inf_soldiers) < 50)
                            {
                                if (freeze[sidenum] == 0 && speedbooster[sidenum] == 0 && inf_soldiers[sidenum] == 0 && more_soldiers[sidenum] == 0)
                                {
                                    int time_booster = 60;
                                    inf_soldiers[sidenum] = time_booster;
                                    (head_inf_soldiers + z)->is_on = 0;
                                }
                            }
                        }

                        // more_soldiers
                        if ((head_more_soldiers + z)->is_on == 1)
                        {
                            // x and y of soldier
                            double x_soldier = (headregion + i)->soldiers[k][j].x_center;
                            double y_soldier = (headregion + i)->soldiers[k][j].y_center;

                            // x and y of more_soldiers
                            double x_more_soldiers = (head_more_soldiers + z)->x;
                            double y_more_soldiers = (head_more_soldiers + z)->y;

                            if (norm(x_soldier - x_more_soldiers) < 50 && norm(y_soldier - y_more_soldiers) < 50)
                            {
                                if (freeze[sidenum] == 0 && speedbooster[sidenum] == 0 && inf_soldiers[sidenum] == 0 && more_soldiers[sidenum] == 0)
                                {
                                    int time_booster = 40;
                                    more_soldiers[sidenum] = time_booster;
                                    (head_more_soldiers + z)->is_on = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void decrease_timeofmixtures()
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
        if (inf_soldiers[i] > 0)
        {
            inf_soldiers[i]--;
        }
        if (more_soldiers[i] > 0)
        {
            more_soldiers[i]--;
        }
    }
}

// function to use all of above functions
void all_of_mixtures(SDL_Renderer *renderer, struct speedbooster *head_speedbooster, struct freeze *head_freeze, struct inf_soldiers *head_inf_soldiers, struct more_soldiers *head_more_soldiers, struct region *head_region, int numofregion)
{
    apply_mixtures(head_speedbooster, head_freeze, head_inf_soldiers, head_more_soldiers, head_region, numofregion);
    print_mixtures(renderer, head_speedbooster, head_freeze, head_inf_soldiers, head_more_soldiers);
    decrease_timeofmixtures();
    randomly_mixture(head_speedbooster, head_freeze, head_inf_soldiers, head_more_soldiers, head_region, numofregion);
}
