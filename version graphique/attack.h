#ifndef _ATTACK_H_
#define _ATTACK_H_

/* ATTACK.H */

int selectSlot(char **map, int *l, int *c, int *x, int *y);
int checkHit(char **map, char **map_def, int *l, int *c);
Ship* detectShip(int *l, int *c, Fleet *p_fleet);
void shipDmg(Ship *damaged_ship);
void attackFleet(char **my_map_def, char **map_atk, char **map_def, int *l, int *c, Fleet *my_fleet, Fleet *p_fleet, int *adversary_life, int *x, int *y, int *alert_tab);


#endif