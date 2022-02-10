// define the buttons
void define_newgame(struct button *newgame);

void define_resume(struct button *resume);

void define_leaderboard(struct button *leaderboard);

// show and print
void show_background(SDL_Renderer *renderer);

void show_mainmenu(SDL_Renderer *renderer);

void show_newgame(SDL_Renderer *renderer, struct button *newgame);

void show_resume(SDL_Renderer *renderer, struct button *resume);

void show_leaderboard(SDL_Renderer *renderer, struct button *leaderboard);

////////////////////////////////////////////////////////////////
int nearnewgame(struct button *newgame, double mouse_x, double mouse_y);

int nearleaderboard(struct button *leaderboard, double mouse_x, double mouse_y);


void zoombuttons(struct button *newgame, struct button *resume, struct button *leaderboard, double mouse_x, double mouse_y);