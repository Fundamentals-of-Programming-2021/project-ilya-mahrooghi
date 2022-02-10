// buttons of menu
struct button
{
    double x;
    double y;
    int zoom;
};
// structs
struct point
{
    double x, y;
};

struct soldier
{
    int is_on; // is the soldier in game or not
    struct region *to;
    Uint32 color;
    double x_center, y_center;
    double vx, vy;
    double current_vx, current_vy;
};

struct region
{
    double x_center, y_center;
    int numofsoldiers;
    Uint32 maincolor, nowcolor;
    struct soldier soldiers[4][200];
    int side;
};

// functions:
double norm(double x);

double distance(double x0, double y0, double x1, double y1);

// functions of showing the image and text
void showimage(SDL_Renderer *renderer, char *image_path, int x0, int y0, int w0, int h0);

void text(SDL_Renderer *m_renderer, double xp, double yp, double w, double h, int fontsize, int R, int G, int B, int A, const char *S);

// function for color of the regions
void random_color_array(Uint32 colorarray[][5]);

// graphic functions
struct point rotate(struct point center, struct point torotate, double angle);

void drawtriangle(SDL_Renderer *renderer, struct point center, double radius, Uint32 color, double angle);

void drawplus(SDL_Renderer *renderer, struct point center, double largeradius, double smallradius, Uint32 color);
// draw region function
void drawpolygonregion(SDL_Renderer *renderer, struct point center, double radius, Uint32 nowcolor, Uint32 maincolor, int speedboosterflag);

// define the array of regions
struct region *polygonwindow(SDL_Renderer *renderer, int *num, Uint32 color[5][5], int randflag);

// change properties of regions
void addsoldier(SDL_Renderer *renderer, struct region *head, int num);

void changecolorofregion(struct region *head, int num);

void updatesides(struct region *region, int num);;
// print all of regions
void printregions(SDL_Renderer *renderer, int numofregions, struct region *head);

// find the region function
struct region *findnearestregion(double x, double y, struct region *head, int numofregions);
// reset the potions
void resetpotions();
// pause button
int near_button(struct button *button, double x, double y);
void define_pause(struct button *pause);


void show_pause(SDL_Renderer *renderer, struct button *pause);

void zoompause(struct button *pause, double mouse_x, double mouse_y);
