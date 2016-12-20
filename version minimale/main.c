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
    int line, column;
    char **p1_att, **p1_def, **p2_att, **p2_def;

    Fleet p1_fleet, p2_fleet;
   
    p1_att = p1_def = p2_att = p2_def = initMap();
    //p1_fleet.carrier = {1,0,1};
    p1_fleet = createFleet();
   
    createMap(p1_att);
    createMap(p1_def);
    createMap(p2_att);
    createMap(p2_def);
   
    /* Map display (this is temporary, I just want to see what I'm doing) */
    displayMap(p1_def);
   
    placeShip(p1_def, &line, &column, &p1_fleet);
    displayMap(p1_def);

    return 0;
}