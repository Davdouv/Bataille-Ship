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
int putShip(char **map, int *l, int *c, int *x, int *y);
void placeShip(char **map, int *l, int *c, Ship *p_ship, int num_ship, int *x, int *y);
void placeFleet(char **map, int *l, int *c, Fleet *p_fleet, int *x, int *y);
int checkHit(char **map, char **map_def, int *l, int *c);
Ship* detectShip(int *l, int *c, Fleet *p_fleet);
void shipDmg(Ship *damaged_ship);
void attackFleet(char **map_atk, char **map_def, int *l, int *c, Fleet *p_fleet, int *adversary_life);
void freeGame(char **p_att, char **p_def, Fleet *p_fleet);
void displayMaps();
void displayOneMap(int map, int x_corner_map);

MLV_Image* image(char* img_name, char* img_num, char* format);

void flemme(char **map, int *l, int *c, Fleet *p_fleet, int *x, int *y);

#endif