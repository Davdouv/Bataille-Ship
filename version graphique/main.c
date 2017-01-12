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
#include "functions.h"

int main( int argc, char *argv[] ){
    int i;
    int game;
    int line, column;
    int x, y;
    int p1_life, p2_life;
    char **p1_att, **p1_def;
    char **p2_att, **p2_def;
    Fleet *p1_fleet;
    Fleet *p2_fleet;
    int *alert_tab = malloc(10 * sizeof(int));

    /* Initalisation alert_tab */
    for (i = 0; i < 10; i++) {
        alert_tab[i] = 0;
    }
    alert_tab[0] = 1;

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

    /* Window creation and display */
    MLV_create_window( "nom_du_jeu", "jeu", WIDTH, HEIGHT);

    // MENU
    game = displayMenu(&x, &y);
    if (game == 1) {

    }
    else if (game == 2) {

    }
    else if (game == 3) {

    }
    
    /* Player 1 : Fleet placement */
    //displayMaps(p1_fleet, p1_def, p1_att, alert_tab);
    printf("Player 1 has to place their fleet.\n");
    if (f == 1) {
        placeFleet(p1_def, p1_att, &line, &column, p1_fleet, &x, &y, alert_tab);
    }
    else {
        flemme(p1_def, p1_att, &line, &column, p1_fleet, &x, &y, alert_tab);
    }

    MLV_wait_seconds(2);
    
    transitionScreen(alert_tab);

    // Player 2 : Fleet placement //
    changePlayer(alert_tab);
    displayMaps(p2_fleet, p2_def, p2_att, alert_tab);
    printf("Player 2 has to place their fleet.\n");
    if (f == 1) {
        placeFleet(p2_def, p2_att, &line, &column, p2_fleet, &x, &y, alert_tab);
    }
    else {
        flemme(p2_def, p2_att, &line, &column, p2_fleet, &x, &y, alert_tab);
    }

    

    // GAME ON //
    while (p1_life != 0) {

        transitionScreen(alert_tab);
        
        changePlayer(alert_tab);
        printf("* Player 1 *\n\n");
        alert_tab[0] = 1;
        alert_tab[1] = 0;
        attackFleet(p1_def, p1_att, p2_def, &line, &column, p1_fleet, p2_fleet, &p2_life, &x, &y, alert_tab);
        if (p2_life == 0) { break; }

        MLV_draw_text_box(
                 80, 80, 250, 70,
                 "PLAYER 2 ATTACKS !", 9,
                 MLV_COLOR_RED, MLV_COLOR_RED, MLV_COLOR_WHITE,
                 MLV_TEXT_LEFT, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
             );

        transitionScreen(alert_tab);
        
        changePlayer(alert_tab);
        printf("* Player 2 *\n\n");
        attackFleet(p2_def, p2_att, p1_def, &line, &column, p2_fleet, p1_fleet, &p1_life, &x, &y, alert_tab);
    }

    // End of the game //
    if (p1_life == 0) {
        printf("\n***\nThe winner is player 2.\nCongratulations!\n***\n");
    }
    else {
        printf("\n***\nThe winner is player 1.\nCongratulations!\n***\n");
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