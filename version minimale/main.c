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
    Fleet *p1_fleet;
    Fleet *p2_fleet;

    /* MODE FLEMME */
    /* 0 pour oui, 1 pour non */
    int f = 1;

    /* Life initialisation */
    if (f == 1) {
        p1_life = p2_life = 17;
    }
    else {
        p1_life = p2_life = 2;
    }

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
    p1_fleet = initFleet();
    p2_fleet = initFleet();

    /* Fleets Creation */
    createFleet(p1_fleet);
    createFleet(p2_fleet);

    /* Player 1 : Fleet placement */
    displayMap(p1_def);
    printf("Player 1 has to place their fleet.\n");
    if (f == 1) {
        placeFleet(p1_def, &line, &column, p1_fleet);
    }
    else {
        flemme(p1_def, &line, &column, p1_fleet);
    }


    /* Player 2 : Fleet placement */
    displayMap(p2_def);
    printf("Player 2 has to place their fleet.\n");
    if (f == 1) {
        placeFleet(p2_def, &line, &column, p2_fleet);
    }
    else {
        flemme(p2_def, &line, &column, p2_fleet);
    }

    /* GAME ON */
    while (p1_life != 0) {
        printf("* Player 1 *\n\n");
        attackFleet(p1_att, p2_def, &line, &column, p2_fleet, &p2_life);
        if (p2_life == 0) { break; }
        printf("* Player 2 *\n\n");
        attackFleet(p2_att, p1_def, &line, &column, p1_fleet, &p1_life);
    }

    /* End of the game */
    if (p1_life == 0) {
        printf("\n***\nThe winner is player 2.\nCongratulations!\n***\n");
    }
    else {
        printf("\n***\nThe winner is player 1.\nCongratulations!\n***\n");
    }

    /* Free Memory */
    freeGame(p1_att, p1_def, p1_fleet);
    freeGame(p2_att, p2_def, p2_fleet);

    return 0;
}