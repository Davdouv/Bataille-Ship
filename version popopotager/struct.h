#ifndef _STRUCT_H_
#define _STRUCT_H_

/* STRUCT.H */

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

#endif