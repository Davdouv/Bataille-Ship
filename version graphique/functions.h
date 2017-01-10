#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

/* FUNCTIONS.H */

/* Declaration of structures and functions */

typedef struct {
    int line;
    int column;
} Coordinates;

typedef struct {
    char name[11];
    int length;
    int orientation; /* 0: horizontal  1: vertical */
    int life;
    Coordinates slot;
} Ship;

typedef struct {
    Ship carrier;
    Ship battleship;
    Ship cruiser;
    Ship submarine;
    Ship destroyer;
} Fleet;

char **initMap();
void createMap(char **map);
void displayMap(char **map);
Fleet* initFleet();
void createFleet(Fleet* pt_fleet);
int selectSlot(char **map, int *l, int *c, int *x, int *y);
int checkPlacement(char **map, int *l, int *c, int o, int ship_length);
int putShip(Fleet *p_fleet, MLV_Image *ship[], int length, char **map, char **map_att, int *l, int *c, int *x, int *y, int *o, int *alert_tab);
void placeShip(Fleet *p_fleet, char **map, char **map_att, int *l, int *c, Ship *p_ship, int num_ship, int *x, int *y, int *alert_tab);
void placeFleet(char **map, char **map_att, int *l, int *c, Fleet *p_fleet, int *x, int *y, int *alert_tab);
int checkHit(char **map, char **map_def, int *l, int *c);
Ship* detectShip(int *l, int *c, Fleet *p_fleet);
void shipDmg(Ship *damaged_ship);
void attackFleet(char **my_map_def, char **map_atk, char **map_def, int *l, int *c, Fleet *my_fleet, Fleet *p_fleet, int *adversary_life, int *x, int *y, int *alert_tab);
void freeGame(char **p_att, char **p_def, Fleet *p_fleet);
void displayMaps(Fleet *p_fleet, char **map_def, char **map_att, int *alert_tab);
void displayOneMap(int map, int x_corner_map);
int division(int a, int b);
void rotationImg(MLV_Image *img, int orientation);
void changePlayer( int *alert_tab);
void transitionScreen(int *alert_tab);

MLV_Image* image(char* img_name, char* img_num, char* format);

void flemme(char **map, char **map_att, int *l, int *c, Fleet *p_fleet, int *x, int *y, int *alert_tab);

void whatPlayer(char *no);

#endif