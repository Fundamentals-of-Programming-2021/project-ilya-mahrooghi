int gamesetting(char *playername, Uint32 color[][5], struct region *headregion, int numofregions, int counterof_addsoldier, int counterof_AI);

int game(int sidenum, Uint32 color[5][5], int randomflag, char *playername, int resumeflag);

char *getname(char *string);

int leader_board();

int choose_map(char *playername);

int menu(char *playername);