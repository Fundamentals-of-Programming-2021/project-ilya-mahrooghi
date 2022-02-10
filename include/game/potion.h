// struct of mixtures
struct speedbooster
{
    double x;
    double y;
    int is_on;
};

struct freeze
{
    double x;
    double y;
    int is_on;
};

struct inf_soldiers
{
    double x;
    double y;
    int is_on;
};

struct more_soldiers
{
    double x;
    double y;
    int is_on;
};

// functions
void define_mixtures(struct speedbooster *head_speedbooster, struct freeze *head_freeze, struct inf_soldiers *head_inf_soldiers, struct more_soldiers *head_more_soldiers);

// show the potions
void draw_speedbooster(SDL_Renderer *renderer, struct speedbooster *speedbooster);

void draw_freeze(SDL_Renderer *renderer, struct freeze *freeze);

void draw_inf_soldiers(SDL_Renderer *renderer, struct inf_soldiers *inf_soldiers);

void draw_more_soldiers(SDL_Renderer *renderer, struct more_soldiers *more_soldiers);

void print_mixtures(SDL_Renderer *renderer, struct speedbooster *head_speedbooster, struct freeze *head_freeze, struct inf_soldiers *head_inf_soldiers, struct more_soldiers *head_more_soldiers);

// logic of the mixtures
int is_between_regions(double x0, double y0, double x1, double y1, double x, double y);

int is_accessible(double x, double y, struct region *head, int numofregion);

void randomly_mixture(struct speedbooster *head_speedbooster, struct freeze *head_freeze, struct inf_soldiers *head_inf_soldiers, struct more_soldiers *head_more_soldiers, struct region *head_region, int numofregion);

void apply_mixtures(struct speedbooster *head_booster, struct freeze *head_freeze, struct inf_soldiers *head_inf_soldiers, struct more_soldiers *head_more_soldiers, struct region *headregion, int numofregion);

void decrease_timeofmixtures();

// function to use all of above functions
void all_of_mixtures(SDL_Renderer *renderer, struct speedbooster *head_speedbooster, struct freeze *head_freeze, struct inf_soldiers *head_inf_soldiers, struct more_soldiers *head_more_soldiers, struct region *head_region, int numofregion);

