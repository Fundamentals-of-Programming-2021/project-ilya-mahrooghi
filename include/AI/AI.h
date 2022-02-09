int free_soldiers(struct region *myregion)
{
    int ans = myregion->numofsoldiers;

    for (int j = 0; j < 200; j++)
    {
        for (int k = 0; k < 4; k++)
        {
            if (myregion->soldiers[k][j].to != NULL && myregion->soldiers[k][j].is_on == 0)
            {
                ans--;
            }
        }
    }
    return ans;
}

int attacking_tome_soldiers(struct region *myregion, struct region *otherregion)
{
    int ans = 0;
    for (int j = 0; j < 200; j++)
    {
        for (int k = 0; k < 4; k++)
        {
            if (otherregion->soldiers[j][k].to == myregion)
            {
                ans++;
            }
        }
    }
    return ans;
}

int is_someone_attacking_tome(int numofregions, struct region *regions, struct region *myregion)
{
    for (int i = 0; i < numofregions; i++)
    {
        if (myregion->side != (regions + i)->side)
        {
            if (attacking_tome_soldiers(myregion, (regions + i)))
            {
                return 1;
            }
        }
    }
    return 0;
}

int has_more_soldiers(struct region *myregion, struct region *otherregion)
{
    if (free_soldiers(myregion) > free_soldiers(otherregion))
    {
        return 1;
    }
    return 0;
}

int speedbooster_exists(struct region *myregion, struct region *otherregion, struct speedbooster *speedbooster)
{
    for (int i = 0; i < 4; i++)
    {
        if (is_between_regions(myregion->x_center, myregion->y_center, otherregion->x_center, otherregion->y_center, (speedbooster + i)->x, (speedbooster + i)->y))
        {
            return 1;
        }
    }
    return 0;
}

int freeze_exists(struct region *myregion, struct region *otherregion, struct freeze *freeze)
{
    for (int i = 0; i < 4; i++)
    {
        if (is_between_regions(myregion->x_center, myregion->y_center, otherregion->x_center, otherregion->y_center, (freeze + i)->x, (freeze + i)->y))
        {
            return 1;
        }
    }
    return 0;
}

int inf_soldiers_exists(struct region *myregion, struct region *otherregion, struct inf_soldiers *inf_soldiers)
{
    for (int i = 0; i < 4; i++)
    {
        if (is_between_regions(myregion->x_center, myregion->y_center, otherregion->x_center, otherregion->y_center, (inf_soldiers + i)->x, (inf_soldiers + i)->y))
        {
            return 1;
        }
    }
    return 0;
}

int more_soldiers_exists(struct region *myregion, struct region *otherregion, struct more_soldiers *more_soldiers)
{
    for (int i = 0; i < 4; i++)
    {
        if (is_between_regions(myregion->x_center, myregion->y_center, otherregion->x_center, otherregion->y_center, (more_soldiers + i)->x, (more_soldiers + i)->y))
        {
            return 1;
        }
    }
    return 0;
}

int pointof_other_region(struct region *myregion, struct region *otherregion, struct speedbooster *head_speedbooster, struct freeze *head_freeze, struct inf_soldiers *head_inf_soldiers, struct more_soldiers *head_more_soldiers)
{
    int ans = 0;
    if (has_more_soldiers(myregion, otherregion))
    {
        ans += 4;
    }
    if (attacking_tome_soldiers(myregion, otherregion))
    {
        if (free_soldiers(myregion) >= attacking_tome_soldiers(myregion, otherregion))
        {
            ans += 4;
        }
    }

    int sidenum = myregion->side;
    if (speedbooster[sidenum] == 0 && freeze[sidenum] == 0 && inf_soldiers[sidenum] == 0 && more_soldiers[sidenum] == 0)
    {
        if (speedbooster_exists(myregion, otherregion, head_speedbooster))
        {
            ans += 2;
        }
        else if (freeze_exists(myregion, otherregion, head_freeze))
        {
            ans += 2;
        }
        else if (more_soldiers_exists(myregion, otherregion, head_more_soldiers))
        {
            ans += 3;
        }
        else if (inf_soldiers_exists(myregion, otherregion, head_inf_soldiers))
        {
            ans += 1;
        }
    }

    return ans;
}

int pointof_neutral_region(struct region *myregion, struct region *otherregion, struct speedbooster *head_speedbooster, struct freeze *head_freeze, struct inf_soldiers *head_inf_soldiers, struct more_soldiers *head_more_soldiers)
{
    int ans = 0;
    if (free_soldiers(myregion) > otherregion->numofsoldiers)
    {
        ans += 3;
    }
    int sidenum = myregion->side;
    if (speedbooster[sidenum] == 0 && freeze[sidenum] == 0 && inf_soldiers[sidenum] == 0 && more_soldiers[sidenum] == 0)
    {

        if (more_soldiers_exists(myregion, otherregion, head_more_soldiers))
        {
            ans += 3;
        }
        else if (freeze_exists(myregion, otherregion, head_freeze))
        {
            ans += 2;
        }
        else if (speedbooster_exists(myregion, otherregion, head_speedbooster))
        {
            ans += 2;
        }
        else if (inf_soldiers_exists(myregion, otherregion, head_inf_soldiers))
        {
            ans += 1;
        }
    }
    return ans;
}

int pointof_teammate(int numofregions, struct region *regions, struct region *myregion, struct region *otherregion, struct speedbooster *head_speedbooster, struct freeze *head_freeze, struct inf_soldiers *head_inf_soldiers, struct more_soldiers *head_more_soldiers)
{
    int ans = 0;
    if (is_someone_attacking_tome(numofregions, regions, otherregion))
    {
        ans += 3;
    }
    int sidenum = myregion->side;
    if (speedbooster[sidenum] == 0 && freeze[sidenum] == 0 && inf_soldiers[sidenum] == 0 && more_soldiers[sidenum] == 0)
    {
        if (speedbooster_exists(myregion, otherregion, head_speedbooster))
        {
            ans += 2;
        }
        else if (freeze_exists(myregion, otherregion, head_freeze))
        {
            ans += 2;
        }
        else if (more_soldiers_exists(myregion, otherregion, head_more_soldiers))
        {
            ans += 3;
        }
        else if (inf_soldiers_exists(myregion, otherregion, head_inf_soldiers))
        {
            ans += 1;
        }
    }
    return ans;
}

void playbots(int sidenum, struct region *regions, int numofregions, struct speedbooster *head_speedbooster, struct freeze *head_freeze, struct inf_soldiers *head_inf_soldiers, struct more_soldiers *head_more_soldiers)
{
    for (int i = 0; i < numofregions; i++)
    {
        if ((regions + i)->maincolor != neutralColor && (regions + i)->maincolor != backgroundColor)
        {
            if ((regions + i)->side != sidenum)
            {
                int max = 0;
                int index = 0;
                for (int j = 0; j < numofregions; j++)
                {
                    if (i != j && (regions + j)->maincolor != backgroundColor)
                    {
                        if ((regions + j)->side != (regions + i)->side)
                        {
                            if ((regions + j)->maincolor != neutralColor)
                            {
                                int tmp = pointof_other_region((regions + i), (regions + j), head_speedbooster, head_freeze, head_inf_soldiers, head_more_soldiers);
                                if (tmp > max)
                                {
                                    index = j;
                                    max = tmp;
                                }
                            }
                            else
                            {
                                int tmp = pointof_neutral_region((regions + i), (regions + j), head_speedbooster, head_freeze, head_inf_soldiers, head_more_soldiers);
                                if (tmp > max)
                                {
                                    index = j;
                                    max = tmp;
                                }
                            }
                        }
                        else
                        {
                            int tmp = pointof_teammate(numofregions, regions, (regions + i), (regions + j), head_speedbooster, head_freeze, head_inf_soldiers, head_more_soldiers);
                            if (tmp > max)
                            {
                                index = j;
                                max = tmp;
                            }
                        }
                    }
                    if (max >= 4)
                    {
                        start_of_attack((regions + i), (regions + index));
                    }
                }
            }
        }
    }
}
