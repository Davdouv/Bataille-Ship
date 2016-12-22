/* FUNCTIONS.C */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "functions.h"

#define NDIM 11     // Grid dimensions
#define NSHIPS 5    // Number of ships /!\ MUST NOT EXCEED 5 ! /!\

/* MAP FUNCTIONS */
/* INIT */
/* Creates the maps | Return the map or NULL if malloc failed */
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

/* Give a value to each slots */
void createMap(char **map) {
    int i, j;
    for (i = 0; i < NDIM; i++) {
        for (j = 0; j < NDIM; j++) {
            if (i == 0 && j == 0) {
                map[i][j] = ' ';            // First slot
            }
            else if (i == 0 && j > 0) {
                map[i][j] = '0' + j;            // Columns
            }
            else if (i > 0 && j == 0) {
                map[i][j] = 'A' + (i - 1);        // Lines
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
            if (j > 9 && i == 0)
                printf("1%c", (map[i][j] - 10));    // Change the display (but not the value) for numbers >= 10
            else {
                printf("%c ", map[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

/* FLEET FUNCTIONS */
/* INIT */
/* Return the fleet or NULL if malloc failed */
Fleet* initFleet() {
    Fleet *fleet = malloc(sizeof(Fleet));
    if (fleet == NULL) {
        return NULL;
    }

    return fleet;
}

/* Creates the fleet | Return fleet */
void createFleet(Fleet* pt_fleet) {
    Fleet my_fleet;

    strncpy(my_fleet.carrier.name, "Carrier", 11);
    my_fleet.carrier.length = 2;
    my_fleet.carrier.life = 2;

    strncpy(my_fleet.battleship.name, "Battleship", 11);
    my_fleet.battleship.length = 3;
    my_fleet.battleship.life = 3;

    strncpy(my_fleet.cruiser.name, "Cruiser", 11);
    my_fleet.cruiser.length = 3;
    my_fleet.cruiser.life = 3;

    strncpy(my_fleet.submarine.name, "Submarine", 11);
    my_fleet.submarine.length = 4;
    my_fleet.submarine.life = 4;

    strncpy(my_fleet.destroyer.name, "Destroyer", 11);
    my_fleet.destroyer.length = 5;
    my_fleet.destroyer.life = 5;

    *pt_fleet = my_fleet;

    //return my_fleet;
}

/* START */
/* Make scanf() works */
void purge(void)
{
    char c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

/* Select a slot | put the line & the column values in variables l and c */
void selectSlot(char **map, int *l, int *c) {
    int i;
    char letter = ' ';
    char *line = &letter;

    do {
        printf("Which line?\n");
        scanf(" %c", line);
        purge();
        *line = toupper(*line);
        if (*line<'A' || *line>('A' + NDIM - 2)) {
            printf("Please choose a correct line letter.\n");
        }
    } while (*line<'A' || *line>('A' + NDIM - 2));          // Repeat until player choose a correct line

    do {
        printf("Which column?\n");
        scanf("%d", c);
        purge();
        if (*c<1 || *c >= NDIM) {
            printf("Please choose a correct column number.\n");
        }
    } while (*c<1 || *c >= NDIM);                       // Repeat until player choose a correct column

    for (i = 0; i < NDIM; i++) {
        if (map[i][0] == *line)                      // Find the line number which countains the letter
            *l = i;
    }
}

/* Check if orientation is set correctly and return 0 or 1 */
int setOrientation() {
    int o;
    do {
        printf("Which orientation? (0: horizontal  1: vertical)\n");
        scanf("%d", &o);
        purge();
        if (o != 0 && o != 1) {
            printf("Please choose between 0 or 1.\n");
        }
    } while (o != 0 && o != 1);                          // Repeat until player choose 0 or 1

    return o;
}

/* Check if slots are free. Return 1 if it is, 0 if it's not */
int checkPlacement(char **map, int *l, int *c, int o, int ship_length) {
    int i;
    if (o == 0) {                                   // If it's horizontal
        for (i = 0; i<ship_length; i++) {
            if (*c + i < NDIM) {
                if (map[*l][*c + i] != '.') {         // If it's not an empty space
                    return 0;
                }
            }
            else                                    // If it's out of the grid
                return 0;
        }
    }
    else {                                          // If it's vertical
        for (i = 0; i<ship_length; i++)
        {
            if (*l + i < NDIM) {
                if (map[*l + i][*c] != '.') {         // If it's not an empty space
                    return 0;
                }
            }
            else                                    // If it's out of the grid
                return 0;
        }
    }
    return 1;                                       // If no problem
}

/* Players place their ships */
void placeShip(char **map, int *l, int *c, Ship *p_ship, int num_ship) {
    int i, o, checkposition;
    // Select Position
    do {
        selectSlot(map, l, c);
        o = setOrientation();
        checkposition = checkPlacement(map, l, c, o, p_ship->length);
        if (checkposition == 0) {
            printf("You must replace your ship, it is stepping over another or it's out of the map.\n");
        }
    } while (checkposition == 0);

    // Save the position and orientation in the Ship
    p_ship->orientation = o;
    p_ship->slot.line = *l;
    p_ship->slot.column = *c;

    // If set then change map value
    if (o == 0) {
        for (i = 0; i< p_ship->length; i++) {
            map[*l][*c + i] = '1'+num_ship;
        }
    }
    else {
        for (i = 0; i< p_ship->length; i++) {
            map[*l + i][*c] = '1'+num_ship;
        }
    }
}

/* Place all ships */
void placeFleet(char **map, int *l, int *c, Fleet *p_fleet) {
    int i;
    Ship *current_ship; // pointer to the ship that is placed
    current_ship = &(p_fleet->carrier); // pointer initialized to the first ship Carrier
    for (i = 0; i<NSHIPS; i++) {
        printf("Set %s (%d)\n", current_ship->name, current_ship->length);
        placeShip(map, l, c, current_ship, i);
        displayMap(map);
        current_ship += 1; // the pointer changes to the next ship
    }
}

void flemme(char **map, int *l, int *c, Fleet *p_fleet) { // pour placer qu'un bateau parce que sinon c'est relou
    Ship *current_ship; // pointer to the ship that is placed
    current_ship = &(p_fleet->carrier); // pointer initialized to the first ship Carrier
    printf("Set %s (%d)\n", current_ship->name, current_ship->length);
    placeShip(map, l, c, current_ship, 0);
    displayMap(map);
}

/* A player is randomly chosen to start */
int whoBegins() {
    srand(time(NULL));
    return rand() % 2;
}

/* TURN */
/* Player gets the damages on his map (except for the first turn) */
void getDamages(char **map_def, int *l, int *c) {

}

/* Player attacks */

/* Check what contains the selected slot */
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
}

/* Check all ships coordinates to find which one has the same as the attack */
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

/* Manage ships life */
void shipDmg(Ship *damaged_ship) {
    // Manage life
    damaged_ship->life--;
    if (damaged_ship->life == 0)
        printf("%s destroyed !\n", damaged_ship->name);
    else
        printf("%s hit ! Life = %d\n", damaged_ship->name, damaged_ship->life);
}

/* Manage attacks */
/* Attacker's att map - Adversary's def map - l - c - adversary's fleet - adversary's life */
void attackFleet(char **map_att, char **map_def, int *l, int *c, Fleet *p_fleet, int *adversary_life) {
    int i, check = 1;

    displayMap(map_att);
    printf("It's your time to attack !\n");
    do {
        do {
            selectSlot(map_att, l, c);
            check = checkHit(map_att, map_def, l, c);
        } while (check == -1);

        if (check == 1) {           // If hit
            map_att[*l][*c] = 'X';
            shipDmg(detectShip(l, c, p_fleet));
            *adversary_life -= 1;
            if (*adversary_life != 0) {
                printf("You can shoot again !\n");
            }
            else {
                displayMap(map_def);
                break;
            }
        }

        else if (check == 0) {           // If miss
            map_att[*l][*c] = 'O';
        }

        displayMap(map_att);
    } while (check == 1);            // Attack while success
}

/* Free memory */
void freeGame(char **p_att,char **p_def, Fleet *p_fleet) {
    free(p_att);
    free(p_def);
    free(p_fleet);
}