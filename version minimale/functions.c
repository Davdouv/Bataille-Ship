/* FUNCTIONS.C */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "functions.h"

#define NDIM 11     // Grid dimensions

/* INIT */
/* Creates the maps | Return NULL if malloc failed */
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
                map[i][j] = '0'+j;            // Columns
            }
            else if (i > 0 && j == 0) {
                map[i][j] = 'A'+(i-1);        // Lines
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

/* Creates the fleet */

Fleet createFleet() {
    Fleet my_fleet;

    strncpy(my_fleet.carrier.name, "Carrier", 11);
    my_fleet.carrier.length = 2;
    my_fleet.carrier.orientation = 0;
    my_fleet.carrier.life = 2;

    strncpy(my_fleet.battleship.name, "Battleship", 11);
    my_fleet.battleship.length = 3;
    my_fleet.battleship.orientation = 0;
    my_fleet.battleship.life = 3;

    strncpy(my_fleet.cruiser.name, "Cruiser", 11);
    my_fleet.cruiser.length = 3;
    my_fleet.cruiser.orientation = 0;
    my_fleet.cruiser.life = 3;

    strncpy(my_fleet.submarine.name, "Submarine", 11);
    my_fleet.submarine.length = 4;
    my_fleet.submarine.orientation = 0;
    my_fleet.submarine.life = 4;

    strncpy(my_fleet.destroyer.name, "Destroyer", 11);
    my_fleet.destroyer.length = 5;
    my_fleet.destroyer.orientation = 0;
    my_fleet.destroyer.life = 5;

    return my_fleet;
}

/* START */
/* Select a slot | put the line & the column values in variables l and c */
void selectSlot(char **map, int *l, int *c) {
    int i;
    char *line;

    do {
        // printf("1: %c\n", *line); // Debug
        printf("Which line?\n");
        // printf("2: %c\n", *line); // Debug
        scanf("%c", line);
        *line = toupper(*line);
    } while(*line<'A' || *line>('A'+NDIM-2));          // Repeat until player choose a correct line

    // do {
    //     if (!line) { 
    //         printf("1: %c\n", line);
    //         printf("Which line?\n");
    //     } else {
    //         printf("2: %c\n", line);
    //         printf("Which line? (Choose a correct letter)\n");
    //         scanf("%c", &line); // prevent the loop from being repeated twice for no reason
    //     }
    //     printf("3: %c\n", line);
    //     scanf("%c", &line);
    //     line = toupper(line);
    // } while(line<'A' || line>('A'+NDIM-2));

    do {
        printf("Which column?\n");
        scanf("%d", c);
        if (*c<1 || *c>=NDIM) {
            printf("Please choose a correct column number.\n");
        }
    } while(*c<1 || *c>=NDIM);                       // Repeat until player choose a correct column
    
    for (i = 0; i < NDIM; i++) {
        if (map[i][0] == *line)                      // Find the line number which countains the letter
         *l = i;           
    }
}

int setOrientation() {
    int o;
    do {
        printf("Which orientation? (0: horizontal  1: vertical)\n");
        scanf("%d",&o);
        if (o != 0 && o != 1) {
            printf("Please choose between 0 or 1.\n");
        }
    } while(o != 0 && o != 1);                          // Repeat until player choose 0 or 1
   
    return o;
}

int checkPlacement(char **map, int *l, int *c, int o, int ship_length) {
    int i;
    if (o == 0) {                                   // If it's horizontal
        for (i = 0; i<ship_length; i++) {
            if (map[*l][*c+i]!='.')                 // If it's not an empty space
                return 0;
        }
    }
    else {                                          // If it's vertical
        for (i = 0; i<ship_length; i++) {
            if (map[*l+i][*c]!='.')                 // If it's not an empty space
                return 0;
        }
    }
    return 1;
}

/* Players place their ships */
void placeShip(char **map, int *l, int *c, Ship *p_ship) {
    int i, o, checkposition;
    do {
        selectSlot(map, l, c);
        o = setOrientation();
        checkposition = checkPosition(map, l, c, o, p_ship->length);
        // if (checkposition == 0) { // C'est supposé prévenir qu'il y a un pb, mais ces 3 lignes font planter l'exécution'
        //      printf("You must replace your ship, it is stepping over another.\n");
        // }
    } while (checkposition == 0);
    if (o == 0) {
        for (i = 0; i< p_ship->length; i++) {
            map[*l][*c+i]='*';
        }
    }
    else {
        for (i = 0; i< p_ship->length; i++) {
            map[*l+i][*c]='*';
        }
    }
}

void placeFleet(char **map, int *l, int *c, Fleet *p_fleet) {
    int i, o;
    Ship *current_ship; // pointer to the ship that is placed
    current_ship = &(p_fleet->carrier); // pointer initialized to the first ship Carrier
    for(i=0;i<5;i++) { 
        printf("Set %s (%d)\n", current_ship->name, current_ship->length);
        placeShip(map, l, c, current_ship);
        displayMap(map);
        current_ship += 1; // the pointer changes to the next ship 
    }
    
}
/* A player is randomly chosen to start */

/* TURN */
/* Player gets the damages on his map (except for the first turn) */
/* Player attacks */
int checkHit(char **map_att, char **map_def, int *l, int *c) {
    if (map_att[*l][*c] != '.') {
        printf("You already shot here. Choose again\n");
        return -1;                  // Already shot here
    }
    if (map_def[*l][*c] == '*') {
        printf("Hit !\n");
        return 1;                   // Ship
    }
        
    else if (map_def[*l][*c] == '.') {
        printf("Miss !\n");
        return 0;                   // Water
    }
}

void attackFleet(char **map_att, char **map_def, int *l, int *c, Fleet *p_fleet) {
    int i, check = 1;
    displayMap(map_att);
    printf("It's your time to attack !\n");
    do {
        do {
            selectSlot(map_att, l, c);
            check = checkHit(map_att, map_def, l, c);
        } while(check == -1);
        
        if (check == 1) {
            map_att[*l][*c] = 'X';
            printf("You can shoot again !\n");
        }

        if (check == 0) {
            map_att[*l][*c] = 'O';
        }
        displayMap(map_att);
    } while(check == 1);
} 
/* Player gets the result of his attack */