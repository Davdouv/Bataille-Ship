#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

/* FUNCTIONS.H */

void displayMap(char **map);
int selectSlot(char **map, int *l, int *c, int *x, int *y);
int checkPlacement(char **map, int *l, int *c, int o, int ship_length);
int putShip(Fleet *p_fleet, MLV_Image *ship[], int length, char **map, char **map_att, int *l, int *c, int *x, int *y, int *o, int *alert_tab);
void placeShip(Fleet *p_fleet, char **map, char **map_att, int *l, int *c, Ship *p_ship, int num_ship, int *x, int *y, int *alert_tab);
void placeFleet(char **map, char **map_att, int *l, int *c, Fleet *p_fleet, int *x, int *y, int *alert_tab);
void freeGame(char **p_att, char **p_def, Fleet *p_fleet);
void displayMaps(Fleet *p_fleet, char **map_def, char **map_att, int *alert_tab);
void displayOneMap(int map, int x_corner_map);
void displaySettableMap (char **map, Fleet *p_fleet);
void displayFleet(Fleet *p_fleet, char **map_def, int x_corner);
void displayShots(char **map_def, char **map_att);
int division(int a, int b);
void rotationImg(MLV_Image *img, int orientation);
void shipPosition (int *x, int *y, int *p_x, int *p_y, int o, int num);
int mouseInsideGrid (int *x, int *y, int x_corner);
void changePlayer( int *alert_tab);
void transitionScreen(int *alert_tab);

MLV_Image* image(char* img_name, char* img_num, char* format);

void flemme(char **map, char **map_att, int *l, int *c, Fleet *p_fleet, int *x, int *y, int *alert_tab);

void whatPlayer(char *no);

#endif