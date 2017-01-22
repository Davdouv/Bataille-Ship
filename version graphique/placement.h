#ifndef _PLACEMENT_H_
#define _PLACEMENT_H_

/* PLACEMENT.H */

int division(int a, int b);
void rotationImg(MLV_Image *img, int orientation);
void shipPosition (int *x, int *y, int *p_x, int *p_y, int orientation, int num, int gameSize);
int checkPlacement(char **map, int *l, int *c, int o, int ship_length, int gameSize);
int putShip(Fleet *p_fleet, MLV_Image *ship[], int length, char **map, char **map_att, int *l, int *c, int *x, int *y, int *o, int *alert_tab, int gameSize, int fleetSize);
void placeShip(Fleet *p_fleet, char **map, char **map_att, int *l, int *c, Ship *p_ship, int num_ship, int *x, int *y, int *alert_tab, int gameSize, int fleetSize);
void placeFleet(char **map, char **map_att, int *l, int *c, Fleet *p_fleet, int *x, int *y, int *alert_tab, int fleetSize, int gameSize);
void placeRandomFleet(char **map, int *l, int *c, Fleet *ai_fleet, int fleetSize, int gameSize);

#endif