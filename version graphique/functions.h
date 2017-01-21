#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

/* FUNCTIONS.H */

MLV_Image* image(char* img_name, char* img_num, char* format);
void displayMap(char **map);
void freeGame(char **p_att, char **p_def, Fleet *p_fleet);
void displayMaps(Fleet *p_fleet, char **map_def, char **map_att, int *alert_tab);
void displayOneMap(int map, int x_corner_map);
void displaySettableMap (char **map, Fleet *p_fleet);
void displayFleet(Fleet *p_fleet, char **map_def, int x_corner);
void displayShots(char **map_def, char **map_att);
int mouseInsideGrid (int *x, int *y, int x_corner);
void changePlayer( int *alert_tab);
void transitionScreen(int *alert_tab);
void whatPlayer(char *no);


#endif