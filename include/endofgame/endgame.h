// who is the winner
int is_on_soldiers(struct region *region, Uint32 color);

int numofregions_ofside(int numside, struct region *regions, int numofregions);

int sideofwinner(struct region *regions, int numofregions);

// work with file of leaderboard
void sort(char *users[], int points[], int num);

void adduser_or_change(char *user[], int points[], int *index, char *playername, int iswinner);

int readfile(char *users[], int points[]);

void changefile(char *users[], int points[], int index);

void changeleaderboard(char *playername, int iswinner);