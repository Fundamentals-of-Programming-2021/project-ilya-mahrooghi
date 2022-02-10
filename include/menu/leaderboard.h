void define_home(struct button *home);

void draw_homebutton(SDL_Renderer *renderer, struct button *home);

int nearhome(struct button *home, double mouse_x, double mouse_y);

void zoominghome(double mouse_x, double mouse_y, struct button *home);

void showranking(SDL_Renderer *renderer);
