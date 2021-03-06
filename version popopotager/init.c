/* INIT.C */
#define NDIM 11     // Grid dimensions
#define NSHIPS 5    // Number of ships MUST NOT EXCEED 5!

#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "init.h"

// Creates the maps | Return the map or NULL if malloc failed 
char** initMap(int gameSize) {
    int i;
    char **map = malloc(gameSize * sizeof(char*));
    if (map == NULL) {
        return NULL;
    }
    for (i = 0; i < gameSize; i++) {
        map[i] = malloc(gameSize * sizeof(char));
        if (map[i] == NULL) {
            return NULL;
        }
    }
    return map;
}

// Give a value to each slots 
void createMap(char **map, int gameSize) {
	int i, j;
	for (i = 0; i < gameSize; i++) {
		for (j = 0; j < gameSize; j++) {
			if (i == 0 && j == 0) {
				map[i][j] = ' ';            // First slot
			}
			else if (i == 0 && j > 0) {
				map[i][j] = '0' + j;            // Columns
			}
			else if (i > 0 && j == 0) {
				map[i][j] = 'A' + (i - 1);        // Lines
			}
			else {
				map[i][j] = '.';            // Slots				
			}
		}
	}
}

// FLEET FUNCTIONS //
// INIT //
// Return the fleet or NULL if malloc failed
Fleet* initFleet() {
    Fleet *fleet = malloc(sizeof(Fleet));
    if (fleet == NULL) {
        return NULL;
    }

    return fleet;
}

// Creates the fleet | Return fleet
void createFleet(Fleet* pt_fleet) {
    Fleet my_fleet;

    strncpy(my_fleet.carrier.name, "ail", 11);
    my_fleet.carrier.length = 2;
    my_fleet.carrier.life = 2;
    my_fleet.carrier.slot.line = -1;
    my_fleet.carrier.slot.column = -1;

    strncpy(my_fleet.battleship.name, "asperge", 11);
    my_fleet.battleship.length = 3;
    my_fleet.battleship.life = 3;
    my_fleet.battleship.slot.line = -1;
    my_fleet.battleship.slot.column = -1;

    strncpy(my_fleet.cruiser.name, "radis", 11);
    my_fleet.cruiser.length = 3;
    my_fleet.cruiser.life = 3;
    my_fleet.cruiser.slot.line = -1;
    my_fleet.cruiser.slot.column = -1;

    strncpy(my_fleet.submarine.name, "carotte", 11);
    my_fleet.submarine.length = 4;
    my_fleet.submarine.life = 4;
    my_fleet.submarine.slot.line = -1;
    my_fleet.submarine.slot.column = -1;

    strncpy(my_fleet.destroyer.name, "poireau", 11);
    my_fleet.destroyer.length = 5;
    my_fleet.destroyer.life = 5;
    my_fleet.destroyer.slot.line = -1;
    my_fleet.destroyer.slot.column = -1;

    *pt_fleet = my_fleet;
}

void createPlayer(int* life, char** def_map, char** att_map, Fleet* fleet, int fleet_size, int gameSize) {
    Ship *current_ship; // pointer to the ship that is placed
    int i;

    createMap(att_map, gameSize);
    createMap(def_map, gameSize);
    createFleet(fleet);

    current_ship = &(fleet->carrier); // pointer initialized to the first ship Carrier
    for (i = 0; i<fleet_size; i++) {
        *life += current_ship->life;
        current_ship += 1; // the pointer changes to the next ship
    }
}