/* * * * * * * * * * * * * * */
/*      BATAILLE  NAVALE     */
/*       par David Nasr      */
/*       et Daphné Rose      */
/* * * * * * * * * * * * * * */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "functions.h"

int main() {
    int line;
    int column;
    char **p1_att;
    char **p1_def;
    char **p2_att;
    char **p2_def;
    p1_att = p1_def = p2_att = p2_def = initMap();
   
    createMap(p1_att);
    createMap(p1_def);
    createMap(p2_att);
    createMap(p2_def);
   
    /* Map display (this is temporary, I just want to see what I'm doing) */
    displayMap(p1_def);
   
    placeShip(p1_def, &line, &column);
    displayMap(p1_def);

    return 0;
}