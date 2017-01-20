#define NDIM 11     // Grid dimensions
#define NSHIPS 5    // Number of ships MUST NOT EXCEED 5!

#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "init.h"

// Creates the maps | Return the map or NULL if malloc failed 
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

// Give a value to each slots 
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

    strncpy(my_fleet.carrier.name, "Carrier", 11);
    my_fleet.carrier.length = 2;
    my_fleet.carrier.life = 2;
    my_fleet.carrier.slot.line = -1;
    my_fleet.carrier.slot.column = -1;

    strncpy(my_fleet.battleship.name, "Battleship", 11);
    my_fleet.battleship.length = 3;
    my_fleet.battleship.life = 3;
    my_fleet.battleship.slot.line = -1;
    my_fleet.battleship.slot.column = -1;

    strncpy(my_fleet.cruiser.name, "Cruiser", 11);
    my_fleet.cruiser.length = 3;
    my_fleet.cruiser.life = 3;
    my_fleet.cruiser.slot.line = -1;
    my_fleet.cruiser.slot.column = -1;

    strncpy(my_fleet.submarine.name, "Submarine", 11);
    my_fleet.submarine.length = 4;
    my_fleet.submarine.life = 4;
    my_fleet.submarine.slot.line = -1;
    my_fleet.submarine.slot.column = -1;

    strncpy(my_fleet.destroyer.name, "Destroyer", 11);
    my_fleet.destroyer.length = 5;
    my_fleet.destroyer.life = 5;
    my_fleet.destroyer.slot.line = -1;
    my_fleet.destroyer.slot.column = -1;

    *pt_fleet = my_fleet;
}