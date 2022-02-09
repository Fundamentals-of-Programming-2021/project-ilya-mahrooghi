void define_random(struct button *random)
{
    random->x = 533;
    random->y = 690;
    random->zoom = 0;
}

void define_start(struct button *start)
{
    start->x = 575;
    start->y = 510;
    start->zoom = 0;
}

void define_savedmap(struct button *savedmap)
{
    savedmap->x = 575;
    savedmap->y = 600;
    savedmap->zoom = 0;
}

void show_random(SDL_Renderer *renderer, struct button *random)
{
    if (random->zoom == 0)
    {
        showimage(renderer, "..//photo//selectmap//random_map.bmp", random->x, random->y, 380, 90);
    }
    else
    {
        double zooming = 10;
        showimage(renderer, "..//photo//selectmap//random_map.bmp", random->x - zooming, random->y - zooming, 380 + 2 * zooming, 90 + 2 * zooming);
    }
}

void show_start(SDL_Renderer *renderer, struct button *start)
{
    if (start->zoom == 0)
    {
        showimage(renderer, "..//photo//selectmap//start.bmp", start->x, start->y, 300, 70);
    }
    else
    {
        double zooming = 10;
        showimage(renderer, "..//photo//selectmap//start.bmp", start->x - zooming, start->y - zooming, 300 + 2 * zooming, 70 + 2 * zooming);
    }
}

void show_savedmap(SDL_Renderer *renderer, struct button *savedmap)
{
    if (savedmap->zoom == 0)
    {
        showimage(renderer, "..//photo//selectmap//savedmap.bmp", savedmap->x, savedmap->y, 300, 70);
    }
    else
    {
        double zooming = 10;
        showimage(renderer, "..//photo//selectmap//savedmap.bmp", savedmap->x - zooming, savedmap->y - zooming, 300 + 2 * zooming, 70 + 2 * zooming);
    }
}

void zoom_resume_start(struct button *random, struct button *start, struct button *savedmap, double mouse_x, double mouse_y)
{
    if (near_button(random, mouse_x, mouse_y))
    {
        random->zoom = 1;
    }
    else
    {
        random->zoom = 0;
    }
    if (near_button(start, mouse_x, mouse_y))
    {
        start->zoom = 1;
    }
    else
    {
        start->zoom = 0;
    }
    if (near_button(savedmap, mouse_x, mouse_y))
    {
        savedmap->zoom = 1;
    }
    else
    {
        savedmap->zoom = 0;
    }
}

////////////////////////////////////////////////////////////////
void gotocolor(int filenum, Uint32 color[][5])
{
    FILE *file;
    if (filenum == 0)
    {
        file = fopen("..//files//map//color0.txt", "r");
    }
    if (filenum == 1)
    {
        file = fopen("..//files//map//color1.txt", "r");
    }
    if (filenum == 2)
    {
        file = fopen("..//files//map//color2.txt", "r");
    }
    if (filenum == 3)
    {
        file = fopen("..//files//map//color3.txt", "r");
    }
    if (filenum == 4)
    {
        file = fopen("..//files//map//color4.txt", "r");
    }

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            fscanf(file, "%x", &color[i][j]);
        }
    }
    fclose(file);
}

void showmap(SDL_Renderer *renderer, int mapnum)
{
    if (mapnum == 0)
    {
        showimage(renderer, "..//photo//selectmap//map0.bmp", 475, 10, 490, 490);
    }
    if (mapnum == 1)
    {
        showimage(renderer, "..//photo//selectmap//map1.bmp", 475, 10, 490, 490);
    }
    if (mapnum == 2)
    {
        showimage(renderer, "..//photo//selectmap//map2.bmp", 475, 10, 490, 490);
    }
    if (mapnum == 3)
    {
        showimage(renderer, "..//photo//selectmap//map3.bmp", 475, 10, 490, 490);
    }
    if (mapnum == 4)
    {
        showimage(renderer, "..//photo//selectmap//map4.bmp", 475, 10, 490, 490);
    }
}

void changemapnum(double mouse_x, double mouse_y, int *mapnum)
{
    if (norm(mouse_x - (1003.5 + 26.5)) <= 26.5 && norm(mouse_y - (168 + 45)) <= 45)
    {
        (*mapnum)++;
        if ((*mapnum) == 5)
        {
            (*mapnum) = 0;
        }
    }
    if (norm(mouse_x - (391.5 + 26.5)) <= 26.5 && norm(mouse_y - (168 + 45)) <= 45)
    {
        (*mapnum)--;
        if ((*mapnum) == -1)
        {
            (*mapnum) = 4;
        }
    }
}
////////////////////////////////
int checkIfFileExists(const char *filename)
{
    struct stat buffer;
    int exist = stat(filename, &buffer);
    if (exist == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_map_saved(char *playername)
{
    char fileaddress[500] = "..//files//map//";
    strcat(fileaddress, playername);
    strcat(fileaddress, ".txt");
    return checkIfFileExists(fileaddress);
}

void changecolor_tosaved(char *playername, Uint32 color[][5])
{
    char fileaddress[500] = "..//files//map//";
    strcat(fileaddress, playername);
    strcat(fileaddress, ".txt");
    FILE *file;
    file = fopen(fileaddress, "r");
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            fscanf(file, "%x", &color[i][j]);
        }
    }
    fclose(file);
}