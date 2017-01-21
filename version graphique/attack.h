#ifndef _ATTACK_H_
#define _ATTACK_H_

/* ATTACK.H */

int selectSlot(char **map, int *l, int *c, int *x, int *y, int x_corner, int gameSize);
int checkHit(char **map, char **map_def, int *l, int *c);
Ship* detectShip(int *l, int *c, Fleet *p_fleet);
void shipDmg(Ship *damaged_ship);
void attackFleet(char **my_map_def, char **map_atk, char **map_def, int *l, int *c, Fleet *my_fleet, Fleet *p_fleet, int *adversary_life, int *x, int *y, int *alert_tab, int gameSize, int fleetSize);
void attackSolo(char **map_def, char **map_att, int *l, int *c, Fleet *my_fleet, int *life, int *x, int *y, int gameSize);
void attackRandomFleet(char **my_map_def, char **map_att, char **map_def, int *l, int *c, Fleet *my_fleet, Fleet *p_fleet, int *adversary_life, int *alert_tab, int gameSize);

#endif