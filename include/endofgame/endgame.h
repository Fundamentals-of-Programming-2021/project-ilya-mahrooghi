// who is the winner
int is_on_soldiers(struct region *region, Uint32 color)
{
    for (int i = 0; i < 200; i++)
    {
        for (int k = 0; k < 4; k++)
        {
            if (region->soldiers[i][k].is_on == 1)
            {
                if (region->soldiers[i][k].color == color)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int numofregions_ofside(int numside, struct region *regions, int numofregions)
{
    Uint32 color;
    if (numside == 0)
    {
        color = color1;
    }
    if (numside == 1)
    {
        color = color2;
    }
    if (numside == 2)
    {
        color = color3;
    }
    int ans = 0;
    for (int i = 0; i < numofregions; i++)
    {
        if ((regions + i)->maincolor != backgroundColor && (regions + i)->maincolor != neutralColor)
        {
            if ((regions + i)->side == numside || is_on_soldiers((regions + i), color))
            {
                ans++;
            }
        }
    }
    return ans;
}

int sideofwinner(struct region *regions, int numofregions)
{
    int numofsides[4];
    numofsides[0] = numofregions_ofside(0, regions, numofregions);
    numofsides[1] = numofregions_ofside(1, regions, numofregions);
    numofsides[2] = numofregions_ofside(2, regions, numofregions);

    if (numofsides[0] == 0 && numofsides[1] == 0)
    {
        return 2;
    }
    else if (numofsides[1] == 0 && numofsides[2] == 0)
    {
        return 0;
    }
    else if (numofsides[2] == 0 && numofsides[0] == 0)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// work with file of leaderboard
void sort(char *users[], int points[], int num)
{
    for (int i = 0; i < num; i++)
    {
        for (int j = i + 1; j < num; j++)
        {
            if (points[j] > points[i])
            {
                int copy = points[i];
                points[i] = points[j];
                points[j] = copy;

                char *strcopy = (char *)malloc(sizeof(char) * 30);
                strcpy(strcopy, users[i]);
                users[i] = users[j];
                users[j] = strcopy;
            }
        }
    }
}

void adduser_or_change(char *user[], int points[], int *index, char *playername, int iswinner)
{
    for (int i = 0; i < (*index); i++)
    {
        if (!strcmp(user[i], playername))
        {
            if (iswinner == 1)
            {
                points[i] += 2;
            }
            else
            {
                points[i] += -1;
            }
            return;
        }
    }
    user[(*index)] = playername;
    if (iswinner == 1)
    {
        points[(*index)] = 2;
    }
    else
    {
        points[(*index)] = -1;
    }
    (*index)++;
}

int readfile(char *users[], int points[])
{
    FILE *file;
    file = fopen("..//files//leaderboard.txt", "r");
    int index = 0;
    while (!feof(file))
    {
        fscanf(file, "%s", users[index]);
        fscanf(file, "%d", &points[index]);
        index++;
    }
    if (index == 0)
    {
        index = 1;
    }
    fclose(file);
    return index;
}

void changefile(char *users[], int points[], int index)
{
    FILE *file;
    file = fopen("..//files//leaderboard.txt", "w");
    for (int i = 0; i < index; i++)
    {
        fprintf(file, "%s %d", users[i], points[i]);
        if (i != (index - 1))
        {
            fprintf(file, "\n");
        }
    }
    fclose(file);
}

void changeleaderboard(char *playername, int iswinner)
{
    char *users[1000];
    for (int i = 0; i < 1000; i++)
    {
        users[i] = (char *)malloc(sizeof(char) * 30);
    }
    int points[1000];
    int index = readfile(users, points);

    adduser_or_change(users, points, &index, playername, iswinner);
    sort(users, points, index);
    changefile(users, points, index);
}