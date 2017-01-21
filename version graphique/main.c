/* * * * * * * * * * * * * * */
/*      BATAILLE  NAVALE     */
/*       par David Nasr      */
/*       et Daphné Rose      */
/* * * * * * * * * * * * * * */

#define _CRT_SECURE_NO_WARNINGS
#define WIDTH 1280
#define HEIGHT 720

#include <MLV/MLV_all.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "menu.h"
#include "struct.h"
#include "init.h"
#include "functions.h"
#include "placement.h"
#include "attack.h"

int main( int argc, char *argv[] ){
    int i;
    int game;
    int line, column;
    int x, y;
    int p1_life = 0, p2_life = 0;
    char **p1_att, **p1_def;
    char **p2_att, **p2_def;
    Fleet *p1_fleet;
    Fleet *p2_fleet;
    int *alert_tab = malloc(10 * sizeof(int));

    int fleet_size = 1;

    /* INITIALISATION */
    /* Maps Initialisation */
    p1_att = initMap();
    p1_def = initMap();
    p2_att = initMap();
    p2_def = initMap();

    /* Fleets Initialisation */
    p1_fleet = initFleet();
    p2_fleet = initFleet();

    /* Initalisation alert_tab */
    for (i = 0; i < 10; i++) {
        alert_tab[i] = 0;
    }
    alert_tab[0] = 0;

    /* Window creation and display */
    MLV_create_window( "Fire in the hole !", "jeu", WIDTH, HEIGHT);

    // MENU
    game = displayMenu(&x, &y);

    /* Creating players */
    createPlayer(&p1_life, p1_def, p1_att, p1_fleet, fleet_size);
    if (game != 1) {    // If not solo
        createPlayer(&p2_life, p2_def, p2_att, p2_fleet, fleet_size);
    }
    
    /* Player 1 : Fleet placement */
    printf("Player 1 has to place his fleet.\n");
    transitionScreen(alert_tab);
    placeFleet(p1_def, p1_att, &line, &column, p1_fleet, &x, &y, alert_tab, fleet_size);

    MLV_wait_seconds(1);
    
    // Player 2 : Fleet placement //
    if (game != 1) {    // If not solo
        changePlayer(alert_tab);
        transitionScreen(alert_tab);
        changePlayer(alert_tab);
        printf("Player 2 has to place his fleet.\n");
        placeFleet(p2_def, p2_att, &line, &column, p2_fleet, &x, &y, alert_tab, fleet_size);
    }
    
    // GAME ON //
    if (game == 1) {
        transitionScreen(alert_tab);
        while (p1_life != 0) {
            attackSolo(p1_def, p1_att, &line, &column, p1_fleet, &p1_life, &x, &y);
        }
    }

    if (game != 1) {
        while (p1_life != 0) {
            transitionScreen(alert_tab);
            changePlayer(alert_tab);
            printf("* Player 1 *\n\n");
            alert_tab[0] = 1;
            alert_tab[1] = 0;
            attackFleet(p1_def, p1_att, p2_def, &line, &column, p1_fleet, p2_fleet, &p2_life, &x, &y, alert_tab);

            if (p2_life == 0) { break; }

            transitionScreen(alert_tab);
                
            changePlayer(alert_tab);
            printf("* Player 2 *\n\n");
            attackFleet(p2_def, p2_att, p1_def, &line, &column, p2_fleet, p1_fleet, &p1_life, &x, &y, alert_tab);
        }
    }

    // End of the game //
    if (game == 1) {
        printf("\n***\nCongratulations! You Destroyed all your fleet\n***\n");
    }
    if (game != 1) {
        if (p1_life == 0) {
        printf("\n***\nThe winner is player 2.\nCongratulations!\n***\n");
        }
        else {
            printf("\n***\nThe winner is player 1.\nCongratulations!\n***\n");
        }
    }

    // Wait 5 secondes
    MLV_wait_seconds(5);

    // Close Window
    MLV_free_window();

    // Free Memory //
    freeGame(p1_att, p1_def, p1_fleet);
    freeGame(p2_att, p2_def, p2_fleet);

    return 0;
}