/* FUNCTIONS.C */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "functions.h"

/* INIT */
/* Creates the maps */
char **initMap() {
    char **map = malloc(11*sizeof(char*));
    int i;
    if (map==NULL) {}
    for(i=0;i<11;i++) {
        map[i] = malloc(11*sizeof(char));
        if(map[i]==NULL) {}
    }
    return map;
}

void createMap(char **map) {
    int i, j;
    char *ptr;

    map[0] = "  ";
    for(i=1;i<11;i++) {
        map[i] = ('A'+i-1)+" ";
    }
    for(j=1;j<11;j++) {
        (map)[j] = i+" ";
    }
    for(i=1;i<11;i++) {
        for(j=1;j<11;j++) {
        *ptr = *((char *)map + i * 11 + j);
        ptr = ". ";
        }
    }
}

/* Creates the maps (2 for each player) */
/* Creates the fleets */

/* START */
/* Players place their ships */
/* A player is randomly chosen to start */

/* TURN */
/* Player gets the damages on his map (except for the first turn) */
/* Player attacks */
/* Player gets the result of his attack */