void define_savemap(struct button *savemap);

void define_savegame(struct button *savegame);

void showsettings(SDL_Renderer *renderer, struct button *savemap, struct button *savegame);

int near_button_setting(struct button *button, double mouse_x, double mouse_y);

////////////////////////////////////////////////////////////////
void save_map(char *playername, Uint32 color[][5]);

void save_game(char *playername, struct region *head, int numofregion, int counterof_addsoldier, int counterof_AI);