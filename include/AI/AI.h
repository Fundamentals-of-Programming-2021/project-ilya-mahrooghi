int free_soldiers(struct region *myregion);

int attacking_tome_soldiers(struct region *myregion, struct region *otherregion);

int is_someone_attacking_tome(int numofregions, struct region *regions, struct region *myregion);

int has_more_soldiers(struct region *myregion, struct region *otherregion);

int speedbooster_exists(struct region *myregion, struct region *otherregion, struct speedbooster *speedbooster);

int freeze_exists(struct region *myregion, struct region *otherregion, struct freeze *freeze);

int inf_soldiers_exists(struct region *myregion, struct region *otherregion, struct inf_soldiers *inf_soldiers);

int more_soldiers_exists(struct region *myregion, struct region *otherregion, struct more_soldiers *more_soldiers);

int pointof_other_region(struct region *myregion, struct region *otherregion, struct speedbooster *head_speedbooster, struct freeze *head_freeze, struct inf_soldiers *head_inf_soldiers, struct more_soldiers *head_more_soldiers);

int pointof_neutral_region(struct region *myregion, struct region *otherregion, struct speedbooster *head_speedbooster, struct freeze *head_freeze, struct inf_soldiers *head_inf_soldiers, struct more_soldiers *head_more_soldiers);

int pointof_teammate(int numofregions, struct region *regions, struct region *myregion, struct region *otherregion, struct speedbooster *head_speedbooster, struct freeze *head_freeze, struct inf_soldiers *head_inf_soldiers, struct more_soldiers *head_more_soldiers);

void playbots(int sidenum, struct region *regions, int numofregions, struct speedbooster *head_speedbooster, struct freeze *head_freeze, struct inf_soldiers *head_inf_soldiers, struct more_soldiers *head_more_soldiers);
