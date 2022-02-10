// functions
void printsoldier(SDL_Renderer *renderer, struct soldier *soldier);

void find_collison(SDL_Renderer *renderer, struct region *head, int numofsoldiers);

void start_of_attack(struct region *from, struct region *to);

void delete_soldiers_ifend(struct region *head, int numofregion);

void attacking(SDL_Renderer *renderer, struct region *head, int numofregion);
