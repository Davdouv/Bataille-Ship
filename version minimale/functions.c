/* FUNCTIONS.C */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "functions.h"

#define NDIM 11

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
                printf("1%c", (map[i][j] - 10));
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
/* Players place their ships */
void placeShip(char **map) {
    int i, c;
    char l;
    printf("Which line ?\n");
    scanf("%c", &l);
    l = toupper(l);
    printf("Which column ?\n");
    scanf("%d", &c);
    printf("You have choosen %c %d\n", l, c);
    for (i = 0; i < NDIM; i++) {
        if (map[i][0] == l)
            map[i][c] = '*';
    }

}
/* A player is randomly chosen to start */

/* TURN */
/* Player gets the damages on his map (except for the first turn) */
/* Player attacks */
/* Player gets the result of his attack */