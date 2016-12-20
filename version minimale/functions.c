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

/* START */
/* Select a slot | put the line & the column values in variables*/
void selectSlot(char **map, int *l, int *c) {
    int i;
    char line;
    printf("Which line ?\n");
    scanf("%c", &line);
    line = toupper(line);
    printf("Which column ?\n");
    scanf("%d", c);
    printf("You have choosen %c %d\n", line, *c);
    for (i = 0; i < NDIM; i++) {
        if (map[i][0] == line)      // Find the line number which countains the letter
         *l = i;           
    }
}

/* Players place their ships */
void placeShip(char **map, int *l, int *c) {
    selectSlot(map, l, c);
    printf("l = %d c = %d\n", *l, *c);
    map[*l][*c] = '*';
}
/* A player is randomly chosen to start */

/* TURN */
/* Player gets the damages on his map (except for the first turn) */
/* Player attacks */
/* Player gets the result of his attack */