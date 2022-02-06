int game(int sidenum)
{
    // deine window and renderer
    const int FPS = 60;
    SDL_Window *sdlWindow = SDL_CreateWindow("State.io", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    // define regions
    struct region *headregion = (struct region *)malloc(sizeof(struct region) * 100);
    int numofregions;
    headregion = polygonwindow(sdlRenderer, &numofregions);

    // attacking of soldiers and work with mouse
    struct region *attackfrom = (struct region *)malloc(sizeof(struct region));
    struct region *attackto = (struct region *)malloc(sizeof(struct region));
    double mouse_x, mouse_y;

    // mixtures
    struct speedbooster *head_speedbooster = (struct speedbooster *)malloc(sizeof(struct speedbooster) * 4);
    struct freeze *head_freeze = (struct freeze *)malloc(sizeof(struct freeze) * 4);
    struct inf_soldiers *head_inf_soldiers = (struct inf_soldiers *)malloc(sizeof(struct inf_soldiers) * 4);
    struct more_soldiers *head_more_soldiers = (struct more_soldiers *)malloc(sizeof(struct more_soldiers) * 4);
    define_mixtures(head_speedbooster, head_freeze, head_inf_soldiers, head_more_soldiers);

    ////////////////////////////////////////////////////////////////////////
    int counterof_addsoldier = 0;
    int counterof_AI = 0;
    int tmp = -1;
    while (1)
    {
        // reset the color
        SDL_SetRenderDrawColor(sdlRenderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(sdlRenderer);

        showimage(sdlRenderer, "..//photo//game//background//background.bmp", 0, 0, 1000, 1200);

        // intialize the screen
        updatesides(headregion, numofregions);
        changecolorofregion(headregion, numofregions);
        if (counterof_addsoldier == 5)
        {
            counterof_addsoldier = 0;
            addsoldier(sdlRenderer, headregion, numofregions);
        }
        printregions(sdlRenderer, numofregions, headregion);
        attacking(sdlRenderer, headregion, numofregions);

        // use the mixtures
        all_of_mixtures(sdlRenderer, head_speedbooster, head_freeze, head_inf_soldiers, head_more_soldiers, headregion, numofregions);

        tmp = sideofwinner(headregion, numofregions);
        if (tmp != -1)
        {
            SDL_Delay(300);
            SDL_DestroyRenderer(sdlRenderer);
            SDL_DestroyWindow(sdlWindow);
            return tmp;
        }

        // AI
        if (counterof_AI == 20)
        {
            counterof_AI = 0;
            // playbots(sidenum, headregion, numofregions);
        }

        // add counters
        counterof_AI++;
        counterof_addsoldier++;

        // render presentation
        SDL_RenderPresent(sdlRenderer);

        SDL_Delay(90);

        // events
        SDL_Event sdlevent;
        while (SDL_PollEvent(&sdlevent))
        {
            switch (sdlevent.type)
            {
            case SDL_QUIT:
                SDL_DestroyRenderer(sdlRenderer);
                SDL_DestroyWindow(sdlWindow);
                return -1;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouse_x = sdlevent.button.x;
                mouse_y = sdlevent.button.y;
                attackfrom = findnearestregion(mouse_x, mouse_y, headregion, numofregions);
                break;
            case SDL_MOUSEBUTTONUP:
                mouse_x = sdlevent.button.x;
                mouse_y = sdlevent.button.y;
                attackto = findnearestregion(mouse_x, mouse_y, headregion, numofregions);

                if (attackto != NULL && attackfrom != NULL && attackto != attackfrom)
                {
                    // if (attackfrom->side == sidenum)
                    {
                        start_of_attack(attackfrom, attackto);
                    }
                }
                break;
            }
        }
    }
}

char *getname(char *string)
{
    // window and renderer
    SDL_Window *sdlWindow = SDL_CreateWindow("State.io", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MENU_SCREEN_WIDTH, MENU_SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    // when close the window
    SDL_bool shallExit = SDL_FALSE;

    // string to return
    string[0] = '\0';

    // mouse
    double mouse_x, mouse_y;

    // pointer of last of string
    int flag = 0;

    // for scaning the string
    int capslock = 0;
    char character;
    while (shallExit == SDL_FALSE)
    {
        // reset the color
        SDL_SetRenderDrawColor(sdlRenderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(sdlRenderer);

        // show menu
        showimage(sdlRenderer, "..//photo//menu//getname.bmp", 0, 0, 1440, 810);

        // string
        int length = strlen(string);
        text(sdlRenderer, 710 - length * 10, 420, length * 30, 30, 30, 0, 0, 0, 255, string);
        // pointer of end of text
        if (flag)
        {
            vlineRGBA(sdlRenderer, 710 + length * 20, 420, 448, 0, 0, 0, 255);
        }

        // render presentation
        SDL_RenderPresent(sdlRenderer);

        // sdl events
        SDL_Event sdlevent;
        while (SDL_PollEvent(&sdlevent))
        {
            int length = strlen(string);

            switch (sdlevent.type)
            {
            case SDL_QUIT:
                SDL_DestroyRenderer(sdlRenderer);
                SDL_DestroyWindow(sdlWindow);
                free(sdlWindow);
                return NULL;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouse_x = sdlevent.button.x;
                mouse_y = sdlevent.button.y;
                if (norm(mouse_x - 900) <= 400 && norm(mouse_y - 550) <= 250)
                {
                    if (flag == 0)
                    {
                        flag = 1;
                    }
                    else
                    {
                        flag = 0;
                    }
                }
                break;

            case SDL_KEYUP:
                if (flag)
                {
                    switch (sdlevent.key.keysym.sym)
                    {
                    case SDLK_RETURN:
                        if (strlen(string) > 0)
                        {
                            SDL_DestroyRenderer(sdlRenderer);
                            SDL_DestroyWindow(sdlWindow);
                            return string;
                        }
                        break;
                    case SDLK_SPACE:
                        character = ' ';
                        break;
                    case SDLK_CAPSLOCK:
                        if (capslock == 0)
                        {
                            capslock = 1;
                        }
                        else
                        {
                            capslock = 0;
                        }
                        break;
                    case SDLK_BACKSPACE:
                        character = ' ';
                        if (length > 0)
                        {
                            string[length - 1] = '\0';
                        }
                        break;
                    default:
                        if (sdlevent.key.keysym.sym != SDLK_UP && sdlevent.key.keysym.sym != SDLK_DOWN && sdlevent.key.keysym.sym != SDLK_LEFT && sdlevent.key.keysym.sym != SDLK_RIGHT)
                        {
                            character = sdlevent.key.keysym.sym;
                        }
                        break;
                    }
                    if (strlen(string) <= 10)
                    {
                        if ((character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z') || (character >= '0' && character <= '9') || character == '-' || character == '_')
                        {
                            if (capslock)
                            {
                                string[length] = character + 'A' - 'a';
                                string[length + 1] = '\0';
                            }
                            else
                            {
                                string[length] = character;
                                string[length + 1] = '\0';
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
}

int leader_board()
{
    // window and renderer
    SDL_Window *sdlWindow = SDL_CreateWindow("State.io", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MENU_SCREEN_WIDTH, MENU_SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    // when close the window
    SDL_bool shallExit = SDL_FALSE;

    // mouse coordinates
    double mouse_x, mouse_y;

    // define home button
    struct button *home = (struct button *)malloc(sizeof(struct button));
    define_home(home);

    while (1)
    {
        SDL_RenderClear(sdlRenderer);

        // show background
        showimage(sdlRenderer, "..//photo//leaderboard//background.bmp", 0, 0, 1440, 810);

        // show rankings
        showranking(sdlRenderer);

        // show home button
        draw_homebutton(sdlRenderer, home);

        // render presentation
        SDL_RenderPresent(sdlRenderer);

        // events
        SDL_Event sdlevent;
        while (SDL_PollEvent(&sdlevent))
        {
            switch (sdlevent.type)
            {
            case SDL_QUIT:
                shallExit = SDL_TRUE;
                SDL_DestroyRenderer(sdlRenderer);
                SDL_DestroyWindow(sdlWindow);
                return 0;

            case SDL_MOUSEMOTION:
                mouse_x = sdlevent.button.x;
                mouse_y = sdlevent.button.y;
                zoominghome(mouse_x, mouse_y, home);
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (nearhome(home, mouse_x, mouse_y))
                {
                    SDL_DestroyRenderer(sdlRenderer);
                    SDL_DestroyWindow(sdlWindow);
                    return 1;
                }
            }
        }
    }
}

int menu(char *playername)
{
    // window and renderer
    SDL_Window *sdlWindow = SDL_CreateWindow("State.io", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MENU_SCREEN_WIDTH, MENU_SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    // when close the window
    SDL_bool shallExit = SDL_FALSE;

    // mouse coordinates
    double mouse_x, mouse_y;

    // side of winner of game
    int winner;

    // buttons
    struct button *newgame = (struct button *)malloc(sizeof(struct button));
    struct button *resume = (struct button *)malloc(sizeof(struct button));
    struct button *leaderboard = (struct button *)malloc(sizeof(struct button));
    define_newgame(newgame);
    define_resume(resume);
    define_leaderboard(leaderboard);

    while (shallExit == SDL_FALSE)
    {
        SDL_RenderClear(sdlRenderer);
        // show menu
        show_background(sdlRenderer);
        show_mainmenu(sdlRenderer);

        // icons
        show_newgame(sdlRenderer, newgame);
        show_resume(sdlRenderer, resume);
        show_leaderboard(sdlRenderer, leaderboard);

        // render presentation
        SDL_RenderPresent(sdlRenderer);

        // events
        SDL_Event sdlevent;
        while (SDL_PollEvent(&sdlevent))
        {
            switch (sdlevent.type)
            {
            case SDL_QUIT:
                shallExit = SDL_TRUE;
                SDL_DestroyRenderer(sdlRenderer);
                SDL_DestroyWindow(sdlWindow);
                return 1;
                break;
            case SDL_MOUSEMOTION:
                mouse_x = sdlevent.button.x;
                mouse_y = sdlevent.button.y;
                zoombuttons(newgame, resume, leaderboard, mouse_x, mouse_y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouse_x = sdlevent.button.x;
                mouse_y = sdlevent.button.y;
                if (nearnewgame(newgame, mouse_x, mouse_y))
                {
                    SDL_DestroyRenderer(sdlRenderer);
                    SDL_DestroyWindow(sdlWindow);
                    winner = game(1);
                    if (winner == 0)
                    {
                        changeleaderboard("neptune", 1);
                        changeleaderboard(playername, -1);
                        changeleaderboard("venus", -1);
                    }
                    if (winner == 1)
                    {
                        changeleaderboard("neptune", -1);
                        changeleaderboard(playername, 1);
                        changeleaderboard("venus", -1);
                    }
                    if (winner == 2)
                    {
                        changeleaderboard("neptune", -1);
                        changeleaderboard(playername, -1);
                        changeleaderboard("venus", 1);
                    }
                    return 0;
                }
                if (nearleaderboard(leaderboard, mouse_x, mouse_y))
                {
                    SDL_DestroyRenderer(sdlRenderer);
                    SDL_DestroyWindow(sdlWindow);

                    int tmp = leader_board();
                    if (tmp == 1)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
    }
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);
}