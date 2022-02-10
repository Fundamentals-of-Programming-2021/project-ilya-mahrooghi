void define_random(struct button *random);

void define_start(struct button *start);

void define_savedmap(struct button *savedmap);

void show_random(SDL_Renderer *renderer, struct button *random);

void show_start(SDL_Renderer *renderer, struct button *start);

void show_savedmap(SDL_Renderer *renderer, struct button *savedmap);

void zoom_resume_start(struct button *random, struct button *start, struct button *savedmap, double mouse_x, double mouse_y);

////////////////////////////////////////////////////////////////
void gotocolor(int filenum, Uint32 color[][5]);

void showmap(SDL_Renderer *renderer, int mapnum);

void changemapnum(double mouse_x, double mouse_y, int *mapnum);
////////////////////////////////
int checkIfFileExists(const char *filename);

int is_map_saved(char *playername);

void changecolor_tosaved(char *playername, Uint32 color[][5]);
