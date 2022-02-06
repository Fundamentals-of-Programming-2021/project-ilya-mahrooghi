const double MENU_SCREEN_WIDTH = 1440;
const double MENU_SCREEN_HEIGHT = 810;

struct button
{
    double x;
    double y;
    int zoom;
};

// define the buttons
void define_newgame(struct button *newgame)
{
    newgame->x = 580;
    newgame->y = 220;
    newgame->zoom = 0;
}

void define_resume(struct button *resume)
{
    resume->x = 615;
    resume->y = 360;
    resume->zoom = 0;
}

void define_leaderboard(struct button *leaderboard)
{
    leaderboard->x = 615;
    leaderboard->y = 480;
    leaderboard->zoom = 0;
}

// show and print
void show_background(SDL_Renderer *renderer)
{
    showimage(renderer, "..//photo//menu//background.bmp", 0, 0, 1440, 810);
}

void show_mainmenu(SDL_Renderer *renderer)
{
    showimage(renderer, "..//photo//menu//main-menu.bmp", 0, 0, 1440, 810);
}

void show_newgame(SDL_Renderer *renderer, struct button *newgame)
{
    double width = 300;
    double height = 100;
    double zooming = 20;
    double x_center = newgame->x;
    double y_center = newgame->y;
    if (newgame->zoom == 1)
    {
        x_center -= zooming;
        y_center -= zooming;
        width += 2 * zooming;
        height += 2 * zooming;
    }
    showimage(renderer, "..//photo//menu//new_game.bmp", x_center, y_center, width, height);
}

void show_resume(SDL_Renderer *renderer, struct button *resume)
{
    double width = 230;
    double height = 80;
    double zooming = 20;
    double x_center = resume->x;
    double y_center = resume->y;
    if (resume->zoom == 1)
    {
        x_center -= zooming;
        y_center -= zooming;
        width += 2 * zooming;
        height += 2 * zooming;
    }
    showimage(renderer, "..//photo//menu//resume.bmp", x_center, y_center, width, height);
}

void show_leaderboard(SDL_Renderer *renderer, struct button *leaderboard)
{
    double width = 230;
    double height = 80;
    double zooming = 20;
    double x_center = leaderboard->x;
    double y_center = leaderboard->y;
    if (leaderboard->zoom == 1)
    {
        x_center -= zooming;
        y_center -= zooming;
        width += 2 * zooming;
        height += 2 * zooming;
    }
    showimage(renderer, "..//photo//menu//leaderboard.bmp", x_center, y_center, width, height);
}

////////////////////////////////////////////////////////////////
int nearnewgame(struct button *newgame, double mouse_x, double mouse_y)
{
    if (norm(mouse_x - (newgame->x + 150)) <= 150 && norm(mouse_y - (newgame->y + 50)) <= 50)
    {
        return 1;
    }
    return 0;
}

int nearleaderboard(struct button *leaderboard, double mouse_x, double mouse_y)
{
    if (norm(mouse_x - (leaderboard->x + 150)) <= 150 && norm(mouse_y - (leaderboard->y + 50)) <= 50)
    {
        return 1;
    }
    return 0;
}

void zoombuttons(struct button *newgame, struct button *resume, struct button *leaderboard, double mouse_x, double mouse_y)
{
    // newgame
    if (norm(mouse_x - (newgame->x + 150)) <= 150 && norm(mouse_y - (newgame->y + 50)) <= 50)
    {
        newgame->zoom = 1;
    }
    else
    {
        newgame->zoom = 0;
    }

    // resume
    if (norm(mouse_x - (resume->x + 150)) <= 150 && norm(mouse_y - (resume->y + 50)) <= 50)
    {
        resume->zoom = 1;
    }
    else
    {
        resume->zoom = 0;
    }

    // leaderboard
    if (norm(mouse_x - (leaderboard->x + 150)) <= 150 && norm(mouse_y - (leaderboard->y + 50)) <= 50)
    {
        leaderboard->zoom = 1;
    }
    else
    {
        leaderboard->zoom = 0;
    }
}
