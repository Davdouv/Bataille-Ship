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