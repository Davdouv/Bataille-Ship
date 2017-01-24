/* ATTACK.C */
#define _CRT_SECURE_NO_WARNINGS
#define WIDTH 1280
#define HEIGHT 720

#include <MLV/MLV_all.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "struct.h"
#include "functions.h"
#include "attack.h"

#define NDIM 11     // Grid dimensions
#define NSHIPS 5    // Number of ships MUST NOT EXCEED 5!

#define x_corner_center WIDTH/3        // x coordinate of the top/left corner of the grid
#define x_corner_def WIDTH/5
#define x_corner_att WIDTH/1.8
#define y_corner HEIGHT/3+20       // y coordinate of the top/left corner of the grid
#define tab_dim HEIGHT/2        // Grid size (it's a square)
//#define cel_dim tab_dim/NDIM    // Cel size (square too)


// ----------- START OF FLEET ATTACKING FUNCTIONS -------------- //

// Select a slot | put the line & the column values in variables l and c and coordinates in x and y
// Return 1 if succeed, 0 if not
int selectSlot(char **map, int *l, int *c, int *x, int *y, int x_corner, int gameSize) {
    int i, j;
    int b = 0;
    int cel_dim = tab_dim/gameSize;

    *l = 1; // Reset line cursor
    *c = 1; // Reset column cursor
    MLV_wait_mouse(x, y);
    if (mouseInsideGrid(x, y, x_corner, gameSize)) {   // If mouse pressed inside the grid
        for (i=(y_corner+2*cel_dim); i<=(y_corner+tab_dim); i=i+cel_dim) {
             for (j=(x_corner+2*cel_dim); j<=(x_corner+tab_dim); j=j+cel_dim) {
                if (*y<=i && *x<=j) {     // If it's inside the last cel
                    *x=j-cel_dim;
                    *y=i-cel_dim;
                    return b = 1;
                }
                *c = *c+1;
            }
            *c=1;
            *l = *l+1;
        }
    }
    return b;       // If mouse pressed outside
}

// Check what contains the selected slot
int checkHit(char **map_att, char **map_def, int *l, int *c) {
    if (map_att[*l][*c] != '.') {
        printf("You already shot here. Choose again\n");
        return -1;                  // Already shot here
    }
    if (map_def[*l][*c] >= '1' && map_def[*l][*c] <= '5') {
        printf("Hit !\n");
        return 1;                   // Ship
    }

    else if (map_def[*l][*c] == '.') {
        printf("Miss !\n");
        return 0;                   // Water
    }

    return -1;
}

// Check all ships coordinates to find which one has the same as the attack
Ship* detectShip(int *l, int *c, Fleet *p_fleet) {
    int ship, i;
    Ship *damaged_ship;
    damaged_ship = &(p_fleet->carrier);                 // pointer initialized to the first ship Carrier
    for (ship = 0; ship<NSHIPS; ship++) {                      // check all ships
        for (i = 0; i<damaged_ship->length; i++) {         // check the whole length of the ship
            if (damaged_ship->orientation == 0) {       // if the ship is displayed horizontably
                if ((&(damaged_ship->slot))->line == *l && ((&(damaged_ship->slot))->column + i) == *c) {   // check all ship coordinates
                    return damaged_ship;
                }
            }
            else {
                if (((&(damaged_ship->slot))->line + i) == *l && (&(damaged_ship->slot))->column == *c) {   // check all ship coordinates
                    return damaged_ship;
                }
            }
        }
        damaged_ship += 1;
    }
    return NULL;        // if the function failed
}

// Manage ships life
void shipDmg(Ship *damaged_ship) {
    // Manage life
    damaged_ship->life--;
    if (damaged_ship->life == 0)
        printf("%s destroyed !\n", damaged_ship->name);
    else
        printf("%s hit ! Life = %d\n", damaged_ship->name, damaged_ship->life);
}

// Manage attacks
// Attacker's def map - Attacker's att map - Adversary's def map - l - c - adversary's fleet - adversary's life
void attackFleet(char **my_map_def, char **map_att, char **map_def, int *l, int *c, Fleet *my_fleet, Fleet *p_fleet, int *adversary_life, int *x, int *y, int *alert_tab, int gameSize, int fleetSize) {
    int check = 1;
    int select = 0;
    MLV_Music* miss = MLV_load_music("sound/splash.mp3");
    MLV_Music* hit  = MLV_load_music("sound/hit.wav");

    MLV_init_audio();

    displayMaps(my_fleet, my_map_def, map_att, alert_tab, gameSize, fleetSize);

    printf("It's your time to attack !\n");
    do {
        do {
            do {
                select = selectSlot(map_att, l, c, x, y, x_corner_att, gameSize);
            } while(select == 0);
            check = checkHit(map_att, map_def, l, c);
        } while (check == -1);

        if (check == 1) {           // If hit
            map_att[*l][*c] = 'X';
            map_def[*l][*c] = 'X';
            shipDmg(detectShip(l, c, p_fleet));
            *adversary_life -= 1;
            MLV_play_music(hit, 1.0, 1);
            if (*adversary_life != 0) {
                printf("You can shoot again !\n");
            }
            else {
                displayMaps(p_fleet, map_def, map_att, alert_tab, gameSize, fleetSize);
                break;
            }
        }

        else if (check == 0) {           // If miss
            map_att[*l][*c] = 'O';
            map_def[*l][*c] = 'O';
            MLV_play_music(miss, 1.0, 1);
        }

        displayMaps(my_fleet, my_map_def, map_att, alert_tab, gameSize, fleetSize);
        MLV_wait_seconds(1);
    } while (check == 1);            // Attack while success

    MLV_wait_seconds(1);
    MLV_free_music(hit);
    MLV_free_music(miss);
    MLV_free_audio();
}

// Solo Attack function
void attackSolo(char **map_def, char **map_att, int *l, int *c, Fleet *my_fleet, int *life, int *x, int *y, int *alert_tab, int gameSize, int fleetSize) {
    int check = 1;
    int select = 0;
    MLV_Music* miss = MLV_load_music("sound/splash.mp3");
    MLV_Music* hit  = MLV_load_music("sound/hit.wav");

    MLV_init_audio();

    displayAttackMap(map_att, gameSize);

    printf("It's your time to attack !\n");
    do {
        do {
            do {
                select = selectSlot(map_att, l, c, x, y, x_corner_center, gameSize);
            } while(select == 0);
            check = checkHit(map_att, map_def, l, c);
        } while (check == -1);

        if (check == 1) {           // If hit
            map_att[*l][*c] = 'X';
            map_def[*l][*c] = 'X';
            shipDmg(detectShip(l, c, my_fleet));
            *life -= 1;
            MLV_play_music(hit, 1.0, 1);
            if (*life != 0) {
                printf("You can shoot again !\n");
            }
            else {
                displayMaps(my_fleet, map_def, map_att, alert_tab, gameSize, fleetSize);
                break;
            }
        }

        else if (check == 0) {           // If miss
            map_att[*l][*c] = 'O';
            map_def[*l][*c] = 'O';
            MLV_play_music(miss, 1.0, 1);
        }

        displayAttackMap(map_att, gameSize);
    } while (check == 1);            // Attack while success

    MLV_wait_seconds(2);
    MLV_free_music(hit);
    MLV_free_music(miss);
    MLV_free_audio();
}

// AI Attacks
void attackRandomFleet(char **my_map_def, char **map_att, char **map_def, int *l, int *c, Fleet *my_fleet, Fleet *p_fleet, int *adversary_life, int *alert_tab, int gameSize, int fleetSize) {
    int check = 1;
    int ligne, column;
    int AI = 0;
    MLV_Music* miss = MLV_load_music("sound/splash.mp3");
    MLV_Music* hit  = MLV_load_music("sound/hit.wav");

    MLV_init_audio();

    //displayMaps(my_fleet, my_map_def, map_att, alert_tab);
    displayAttackMap(map_att, gameSize);

    printf("It's your time to attack !\n");
    do {
        do {
            if (AI == 0) {
                *l = randomNumber(1, gameSize);
                *c = randomNumber(1, gameSize);
            }
            else {
                column = *c+1;
                if (checkHit(map_att, map_def, l, &column) != -1)
                    *c=*c+1;
                else {
                    column = *c-1;
                    if (checkHit(map_att, map_def, l, &column) != -1)
                        *c=*c-1;
                    else {
                        ligne = *l+1;
                        if (checkHit(map_att, map_def, &ligne, c) != -1)
                            *l=*l+1;
                        else {
                            ligne = *l-1;
                            if (checkHit(map_att, map_def, &ligne, c) != -1)
                                *l=*l-1;
                            else {
                                *l = randomNumber(1, gameSize);
                                *c = randomNumber(1, gameSize);
                            }
                        }
                    }
                }
            }
            check = checkHit(map_att, map_def, l, c);

        } while (check == -1);

        if (check == 1) {           // If hit
            AI = 1;
            map_att[*l][*c] = 'X';
            map_def[*l][*c] = 'X';
            shipDmg(detectShip(l, c, p_fleet));
            *adversary_life -= 1;
            MLV_play_music(hit, 1.0, 1);
            if (*adversary_life != 0) {
                printf("You can shoot again !\n");
            }
            else {
                displayMaps(p_fleet, map_def, map_att, alert_tab, gameSize, fleetSize);
                break;
            }
        }

        else if (check == 0) {           // If miss
            map_att[*l][*c] = 'O';
            map_def[*l][*c] = 'O';
            MLV_play_music(miss, 1.0, 1);
        }

        //displayMaps(my_fleet, my_map_def, map_att, alert_tab);
        displayAttackMap(map_att, gameSize);
        MLV_wait_seconds(1);
    } while (check == 1);            // Attack while success

    MLV_wait_seconds(2);
    MLV_free_music(hit);
    MLV_free_music(miss);
    MLV_free_audio();
}