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
    int line, column, p1_life, p2_life;
    char **p1_att, **p1_def;
    char **p2_att, **p2_def;
    Fleet p1_fleet; // Il faudrait créer une fonction initFleet pour alouer la mémoire comme pour initMap
    Fleet p2_fleet;

    /* Life initialisation */
    p1_life = p2_life = 17;
   
   /* Maps Initialisation */
    p1_att = initMap();
    p1_def = initMap();
    p2_att = initMap();
    p2_def = initMap();

   /* Maps Creation */
    createMap(p1_att);
    createMap(p1_def);
    createMap(p2_att);
    createMap(p2_def);

    /* Fleets Initialisation */

    /* Fleets Creation */
    p1_fleet = createFleet();
    p2_fleet = createFleet();

    /* Player 1 : Fleet placement */
    displayMap(p1_def);
    printf("Player 1 has to place their fleet.\n");
    placeFleet(p1_def, &line, &column, &p1_fleet);
    //flemme(p1_def, &line, &column, &p1_fleet);

    /* Player 2 : Fleet placement */
    displayMap(p2_def);
    printf("Player 2 has to place their fleet.\n");
    placeFleet(p2_def, &line, &column, &p2_fleet);
    //flemme(p2_def, &line, &column, &p2_fleet);

    /* GAME ON */
    while (p1_life != 0)  {  
    printf("* Player 1 *\n\n");
    attackFleet(p1_att, p1_def, &line, &column, &p1_fleet, p2_life);
    if (p2_life == 0) {break;}
    printf("* Player 2 *\n\n");
    attackFleet(p2_att, p2_def, &line, &column, &p2_fleet, p1_life);
    }

    /* End of the game */
    if (p1_life == 0) {
        printf("The winner is player 2.\nCongratulations!\n");
    } else {
        printf("The winner is player 1.\nCongratulations!\n");
    }

    return 0;
}