struct region *find_region_with_less_soldiers(struct region *regions, int numofregions, struct region *head)
{
    int numofsoldier = head->numofsoldiers;
    struct region *ans = NULL;
    for (int i = 0; i < numofregions; i++)
    {
        if ((regions + i)->maincolor != backgroundColor)
        {
            if ((regions + i)->numofsoldiers < numofsoldier)
            {
                numofsoldier = (regions + i)->numofsoldiers;
                ans = (regions + i);
            }
        }
    }
    return ans;
}

void playbots(int sidenum, struct region *regions, int numofregions)
{
    for (int i = 0; i < numofregions; i++)
    {
        if ((regions + i)->maincolor != neutralColor && (regions + i)->maincolor != backgroundColor)
        {
            if ((regions + i)->side != sidenum && (regions + i)->toattacking == NULL)
            {
                struct region *tmp = find_region_with_less_soldiers(regions, numofregions, (regions + i));
                if (tmp != NULL)
                {
                    start_of_attack((regions + i), tmp);
                }
            }
        }
    }
}

