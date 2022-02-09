int gamesetting(char *playername, Uint32 color[][5], struct region *headregion, int numofregions, int counterof_addsoldier, int counterof_AI)
{
    double settingwidth = 400;
    double settingheight = 600;
    SDL_Window *settingWindow = SDL_CreateWindow("settings", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, settingwidth, settingheight, SDL_WINDOW_OPENGL);
    SDL_Renderer *settingRenderer = SDL_CreateRenderer(settingWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_GL_SwapWindow(settingWindow);

    // work with mouse
    double mouse_x, mouse_y;

    // buttons
    struct button *savemap = (struct button *)malloc(sizeof(struct button));
    struct button *savegame = (struct button *)malloc(sizeof(struct button));
    define_savemap(savemap);
    define_savegame(savegame);

    while (1)
    {
        SDL_RenderClear(settingRenderer);

        // background
        showimage(settingRenderer, "..//photo//menu//background.bmp", 0, 0, 400, 600);

        showsettings(settingRenderer, savemap, savegame);

        SDL_RenderPresent(settingRenderer);

        // events
        SDL_Event sdlevent;
        while (SDL_PollEvent(&sdlevent))
        {
            switch (sdlevent.type)
            {
            case SDL_WINDOWEVENT:
                if (sdlevent.window.event == SDL_WINDOWEVENT_CLOSE)
                {
                    SDL_DestroyRenderer(settingRenderer);
                    SDL_DestroyWindow(settingWindow);
                    free(savemap);
                    free(savegame);
                    return -1;
                }
                break;
            case SDL_MOUSEMOTION:
                mouse_x = sdlevent.button.x;
                mouse_y = sdlevent.button.y;
                if (near_button_setting(savegame, mouse_x, mouse_y))
                {
                    savegame->zoom = 1;
                }
                else
                {
                    savegame->zoom = 0;
                }
                if (near_button_setting(savemap, mouse_x, mouse_y))
                {
                    savemap->zoom = 1;
                }
                else
                {
                    savemap->zoom = 0;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouse_x = sdlevent.button.x;
                mouse_y = sdlevent.button.y;
                if (near_button_setting(savemap, mouse_x, mouse_y))
                {
                    save_map(playername, color);
                }
                if (near_button_setting(savegame, mouse_x, mouse_y))
                {
                    save_game(playername, headregion, numofregions, counterof_addsoldier, counterof_AI);
                }
            }
        }
    }
}

int game(int sidenum, Uint32 color[5][5], int randomflag, char *playername, int resumeflag)
{
    // deine window and renderer
    const int FPS = 60;
    SDL_Window *sdlWindow = SDL_CreateWindow("State.io", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    // pause button
    struct button *pause = (struct button *)malloc(sizeof(struct button));
    define_pause(pause);
    int pauseflag = 1;

    // define regions
    struct region *headregion = (struct region *)malloc(sizeof(struct region) * 30);
    int numofregions;
    headregion = polygonwindow(sdlRenderer, &numofregions, color, randomflag);

    // attacking of soldiers and work with mouse
    struct region *attackfrom = (struct region *)malloc(sizeof(struct region));
    struct region *attackto = (struct region *)malloc(sizeof(struct region));
    attackfrom = NULL;
    attackto = NULL;
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
    if (resumeflag)
    {
        change_data(playername, headregion, &numofregions, &counterof_addsoldier, &counterof_AI);
    }

    int totcount = 0;
    while (1)
    {
        totcount++;
        if (pauseflag)
        {
            // reset the color
            SDL_SetRenderDrawColor(sdlRenderer, 0xff, 0xff, 0xff, 0xff);
            SDL_RenderClear(sdlRenderer);

            // background
            showimage(sdlRenderer, "..//photo//game//background//background.bmp", 0, 0, 1000, 1200);

            // pause button
            show_pause(sdlRenderer, pause);

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

            // AI
            // if (counterof_AI == 20)
            {
                counterof_AI = 0;
                playbots(sidenum, headregion, numofregions, head_speedbooster, head_freeze, head_inf_soldiers, head_more_soldiers);
            }

            // add counters
            counterof_AI++;
            counterof_addsoldier++;

            // render presentation
            SDL_RenderPresent(sdlRenderer);

            SDL_Delay(90);

            // is the end of game or not?
            tmp = sideofwinner(headregion, numofregions);
            if (tmp != -1)
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
                SDL_RenderPresent(sdlRenderer);
                resetpotions();
                SDL_Delay(300);

                SDL_DestroyRenderer(sdlRenderer);
                SDL_DestroyWindow(sdlWindow);
                ////////////////////////////////////////////////////////////////
                free(pause);
                free(head_speedbooster);
                free(head_inf_soldiers);
                free(head_freeze);
                free(head_more_soldiers);
                free(headregion);
                ////////////////////////////////////////////////////////////////
                return tmp;
            }
        }
        if (resumeflag && totcount == 1)
        {
            pauseflag = 0;
        }
        // events
        SDL_Event sdlevent;
        while (SDL_PollEvent(&sdlevent))
        {
            switch (sdlevent.type)
            {
            case SDL_QUIT:
                resetpotions();
                SDL_DestroyRenderer(sdlRenderer);
                SDL_DestroyWindow(sdlWindow);
                ////////////////////////////////////////////////////////////////
                free(pause);
                free(head_speedbooster);
                free(head_inf_soldiers);
                free(head_freeze);
                free(head_more_soldiers);
                free(headregion);
                ////////////////////////////////////////////////////////////////
                return -1;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouse_x = sdlevent.button.x;
                mouse_y = sdlevent.button.y;
                if (near_button(pause, mouse_x, mouse_y))
                {
                    if (pauseflag == 0)
                    {
                        pauseflag = 1;
                    }
                    else
                    {
                        gamesetting(playername, color, headregion, numofregions, counterof_addsoldier, counterof_AI);
                        pauseflag = 0;
                    }
                }
                if (pauseflag)
                {
                    attackfrom = findnearestregion(mouse_x, mouse_y, headregion, numofregions);
                }
                break;
            case SDL_MOUSEBUTTONUP:
                mouse_x = sdlevent.button.x;
                mouse_y = sdlevent.button.y;
                if (pauseflag)
                {
                    attackto = findnearestregion(mouse_x, mouse_y, headregion, numofregions);

                    if (attackto != NULL && attackfrom != NULL && attackto != attackfrom)
                    {
                        // if (attackfrom->side == sidenum)
                        {
                            start_of_attack(attackfrom, attackto);
                        }
                    }
                }
                break;
            case SDL_MOUSEMOTION:
                mouse_x = sdlevent.button.x;
                mouse_y = sdlevent.button.y;
                zoompause(pause, mouse_x, mouse_y);
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
                free(string);
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
                free(home);
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
                    free(home);
                    return 1;
                }
            }
        }
    }
}

int choose_map(char *playername)
{
    // window and renderer
    SDL_Window *sdlWindow = SDL_CreateWindow("State.io", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MENU_SCREEN_WIDTH, MENU_SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    // work with mouse
    double mouse_x, mouse_y;

    // color array
    Uint32 color[5][5];
    int mapnum = 0;

    struct button *random = (struct button *)malloc(sizeof(struct button));
    struct button *start = (struct button *)malloc(sizeof(struct button));
    struct button *savedmap = (struct button *)malloc(sizeof(struct button));
    define_random(random);
    define_start(start);
    define_savedmap(savedmap);
    while (1)
    {
        SDL_RenderClear(sdlRenderer);

        // show background
        showimage(sdlRenderer, "..//photo//selectmap//background.bmp", 0, 0, 1440, 810);

        // show menu
        show_random(sdlRenderer, random);
        show_start(sdlRenderer, start);
        show_savedmap(sdlRenderer, savedmap);

        // show default maps
        showmap(sdlRenderer, mapnum);

        // render presentation
        SDL_RenderPresent(sdlRenderer);

        // events
        SDL_Event sdlevent;
        while (SDL_PollEvent(&sdlevent))
        {
            switch (sdlevent.type)
            {
            case SDL_QUIT:
                SDL_DestroyRenderer(sdlRenderer);
                SDL_DestroyWindow(sdlWindow);
                free(random);
                free(start);
                free(savedmap);
                return -1;
                break;
            case SDL_MOUSEMOTION:
                mouse_x = sdlevent.button.x;
                mouse_y = sdlevent.button.y;
                zoom_resume_start(random, start, savedmap, mouse_x, mouse_y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouse_x = sdlevent.button.x;
                mouse_y = sdlevent.button.y;
                changemapnum(mouse_x, mouse_y, &mapnum);
                if (near_button(random, mouse_x, mouse_y))
                {
                    SDL_DestroyRenderer(sdlRenderer);
                    SDL_DestroyWindow(sdlWindow);
                    free(random);
                    free(start);
                    free(savedmap);
                    int tmp = game(1, color, 1, playername, 0);
                    return tmp;
                }
                if (near_button(start, mouse_x, mouse_y))
                {
                    gotocolor(mapnum, color);
                    SDL_DestroyRenderer(sdlRenderer);
                    SDL_DestroyWindow(sdlWindow);
                    free(random);
                    free(start);
                    free(savedmap);
                    int tmp = game(1, color, 0, playername, 0);
                    return tmp;
                }
                if (near_button(savedmap, mouse_x, mouse_y))
                {
                    if (is_map_saved(playername))
                    {
                        SDL_DestroyRenderer(sdlRenderer);
                        SDL_DestroyWindow(sdlWindow);
                        free(random);
                        free(start);
                        free(savedmap);
                        changecolor_tosaved(playername, color);
                        int tmp = game(1, color, 0, playername, 0);
                        return tmp;
                    }
                }
            }
        }
    }
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);
    free(random);
    free(start);
    free(savedmap);
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
                free(newgame);
                free(resume);
                free(leaderboard);
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
                    free(newgame);
                    free(resume);
                    free(leaderboard);
                    winner = choose_map(playername);
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
                    
                    free(newgame);
                    free(resume);
                    free(leaderboard);

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

                if (near_button(resume, mouse_x, mouse_y))
                {
                    if (isgame_saved(playername))
                    {
                        SDL_DestroyRenderer(sdlRenderer);
                        SDL_DestroyWindow(sdlWindow);
                        free(newgame);
                        free(resume);
                        free(leaderboard);
                        Uint32 color[5][5];
                        winner = game(1, color, 0, playername, 1);
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
                }
            }
        }
    }
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);
    free(newgame);
    free(resume);
    free(leaderboard);
}