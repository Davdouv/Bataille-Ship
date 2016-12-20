/* FUNCTIONS.C */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "functions.h"

#define NDIM 11     // Grid dimensions

/* INIT */
/* Creates the maps | Return NULL if malloc failed */
char** initMap() {
    int i;
    char **map = malloc(NDIM * sizeof(char*));
    if (map == NULL) {
        return NULL;
    }
    for (i = 0; i < NDIM; i++) {
        map[i] = malloc(NDIM * sizeof(char));
        if (map[i] == NULL) {
            return NULL;
        }
    }

    return map;
}

// Give a value to each slots
void createMap(char **map) {
    int i, j;
    for (i = 0; i < NDIM; i++) {
        for (j = 0; j < NDIM; j++) {
            if (i == 0 && j == 0) {
                map[i][j] = ' ';            // First slot
            }
            else if (i == 0 && j > 0) {
                map[i][j] = '0'+j;            // Columns
            }
            else if (i > 0 && j == 0) {
                map[i][j] = 'A'+(i-1);        // Lines
            }
            else
                map[i][j] = '.';            // Slots
        }
    }
}

/* Display the map */
void displayMap(char **map) {
    int i, j;
    for (i = 0; i < NDIM; i++) {
        for (j = 0; j < NDIM; j++) {
            if (j>9 && i == 0)
                printf("1%c", (map[i][j] - 10));    // Change the display (but not the value) for numbers >= 10
            else {
                printf("%c ", map[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

/* Creates the maps (2 for each player) */

Fleet createFleet() {
    Fleet my_fleet;
    my_fleet.carrier.length = 2;
    my_fleet.carrier.orientation = 0;
    my_fleet.carrier.life = 2;
    my_fleet.battleship.length = 3;
    my_fleet.battleship.orientation = 0;
    my_fleet.battleship.life = 3;
    my_fleet.cruiser.length = 3;
    my_fleet.cruiser.orientation = 0;
    my_fleet.cruiser.life = 3;
    my_fleet.submarine.length = 4;
    my_fleet.submarine.orientation = 0;
    my_fleet.submarine.life = 4;
    my_fleet.destroyer.length = 5;
    my_fleet.destroyer.orientation = 0;
    my_fleet.destroyer.life = 5;

    return my_fleet;
}

/* START */
/* Select a slot | put the line & the column values in variables*/
void selectSlot(char **map, int *l, int *c) {
    int i;
    char line;

    do {
        printf("Which line ?\n");
        scanf("%c", &line);
        line = toupper(line);
    } while(line<'A' || line>('A'+NDIM-2));          // Repeat until player choose a correct line

    do {
        printf("Which column ?\n");
        scanf("%d", c);
    } while(*c<1 || *c>=NDIM);                       // Repeat until player choose a correct column
    
    
    
    //printf("You have choosen %c %d\n", line, *c);
    for (i = 0; i < NDIM; i++) {
        if (map[i][0] == line)                      // Find the line number which countains the letter
         *l = i;           
    }
}

int setOrientation() {
    int o;
   // do {
        printf("Which orientation ? (0: horizontal  1: vertical)\n");
        scanf("%d",&o);
   // } while();                          // Repeat until player choose 0 or 1
   
    return o;
}

int checkPosition(char **map, int *l, int *c, int o, int ship_length) {
    int i;
    if (o == 0) {                                   // If it's horizontal
        for (i = 0; i<ship_length; i++) {
            if (map[*l][*c+i]!='.')                 // If it's not an empty space
                return 0;
        }
    }
    else {                                          // If it's vertical
        for (i = 0; i<ship_length; i++) {
            if (map[*l+i][*c]!='.')                 // If it's not an empty space
                return 0;
        }
    }
    return 1;
}

/* Players place their ships */
void placeShip(char **map, int *l, int *c, Fleet *p_fleet) {
    int i, o;
    do {
        printf("Set carrier (%d)\n", p_fleet->carrier.length);
        selectSlot(map, l, c);
        o = setOrientation();
    } while (checkPosition(map, l, c, o, p_fleet->carrier.length)==0);
    if (o == 0) {
        for (i = 0; i< p_fleet->carrier.length; i++) {
            map[*l][*c+i]='*';
        }
    }
    else {
        for (i = 0; i< p_fleet->carrier.length; i++) {
            map[*l+i][*c]='*';
        }
    }
}
/* A player is randomly chosen to start */

/* TURN */
/* Player gets the damages on his map (except for the first turn) */
/* Player attacks */
/* Player gets the result of his attack */