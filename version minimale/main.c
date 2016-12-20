/* * * * * * * * * * * * * * */
/*      BATAILLE  NAVALE     */
/*       par David Nasr      */
/*       et Daphné Rose      */
/* * * * * * * * * * * * * * */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "functions.h"

int main() {
    int line, column;
    char **p1_att, **p1_def;

    Fleet p1_fleet;
   
    p1_att = initMap();
    p1_def = initMap();
    p1_fleet = createFleet();
   
    createMap(p1_att);
    createMap(p1_def);
   
    /* Map display (this is temporary, I just want to see what I'm doing) */
    displayMap(p1_def);
    placeFleet(p1_def, &line, &column, &p1_fleet);
    attackFleet(p1_att, p1_def, &line, &column, &p1_fleet);

    return 0;
}