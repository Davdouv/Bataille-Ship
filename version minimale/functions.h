/* FUNCTIONS.H */
/* Declaration of structures and functions */

typedef struct {
    int h;
    int v;
    int orientation; /* 0: horizontal  1: vertical */
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
void selectSlot(char **map, int *l, int *c);
void placeShip(char **map, int *l, int *c);