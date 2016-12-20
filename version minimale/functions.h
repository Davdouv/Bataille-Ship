/* FUNCTIONS.H */
/* Declaration of structures and functions */

typedef struct {
    char name[11];
    int length;
    int orientation; /* 0: horizontal  1: vertical */
    int life;
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
Fleet createFleet();
void selectSlot(char **map, int *l, int *c);
int setOrientation();
int checkPosition(char **map, int *l, int *c, int o, int ship_length);
void placeShip(char **map, int *l, int *c, Ship *p_ship);
void placeFleet(char **map, int *l, int *c, Fleet *p_fleet);