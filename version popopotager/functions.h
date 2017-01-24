#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

/* FUNCTIONS.H */

MLV_Image* image(char* img_name, char* img_num, char* format, int gameSize);
void background();
void displayMap(char **map);
void freeGame(char **p_att, char **p_def, Fleet *p_fleet);
void displayMaps(Fleet *p_fleet, char **map_def, char **map_att, int *alert_tab, int gameSize, int fleetSize);
void displayAttackMap(char **map_att, int gameSize);
void displayOneMap(int map, int x_corner_map, int gameSize);
void displaySettableMap(char **map, Fleet *p_fleet, int gameSize, int fleetSize);
void displayFleet(Fleet *p_fleet, char **map_def, int x_corner, int fleetSize, int gameSize);
void displayShots(char **map, int x_corner, int gameSize);
int mouseInsideGrid (int *x, int *y, int x_corner, int gameSize);
void changePlayer(int *alert_tab);
void transitionScreen(int *alert_tab);
void whatPlayer(char *no);
int randomNumber(int a, int b);
void winner(int game, int *p1_life);
int restart(int *x, int *y);

#endif