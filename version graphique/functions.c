/* FUNCTIONS.C */
#define _CRT_SECURE_NO_WARNINGS
#define WIDTH 1280
#define HEIGHT 720

#include <MLV/MLV_all.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "functions.h"

#define NDIM 11     // Grid dimensions
#define NSHIPS 5    // Number of ships MUST NOT EXCEED 5!

#define x_corner WIDTH/3        // x coordinate of the top/left corner of the grid
#define x_corner_def WIDTH/5
#define x_corner_att WIDTH/1.8
#define y_corner HEIGHT/4       // y coordinate of the top/left corner of the grid
#define tab_dim HEIGHT/2        // Grid size (it's a square)
#define cel_dim tab_dim/NDIM    // Cel size (square too)

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
			else {
				map[i][j] = '.';            // Slots				
			}
		}
	}
}

/* Make declaration of image of the size of a cell easier */
MLV_Image* image(char* img_name, char* img_num, char* format) {
    char *file = malloc(40 * sizeof(char));
    MLV_Image *my_image;

    strcpy(file, "img/");
    strcat(file, img_name);
    strcat(file, img_num);
    strcat(file, ".");
    strcat(file, format);

    my_image = MLV_load_image(file);
    free(file);
    MLV_resize_image_with_proportions(my_image, cel_dim, cel_dim);
    return my_image;
}

/* Display a single map */
void displayOneMap(int map, int x_corner_map) {
    int i, j;
    char *num = malloc(3 * sizeof(char));
    char *letter = malloc(3 * sizeof(char));
    strncpy(num, "1 \0", 3);
    strncpy(letter, "A \0", 3);
    MLV_Image *water;

    if (map == 0) {
        water = image("water","","jpg");
    } else {
        water = image("water_att","","jpg");
    }

    for (i = 0; i < NDIM; i++) {
		for (j = 0; j < NDIM; j++) {
			if (i == 0 && j == 0) {         // First slot
				//map[i][j] = ' ';            
                MLV_draw_filled_rectangle(x_corner_map, y_corner, cel_dim, cel_dim, MLV_COLOR_RED);
			}
			else if (i == 0 && j > 0) {     // Columns
                sprintf(num,"%d",j);          
				//MLV_draw_filled_rectangle(x_corner+(j*cel_dim), y_corner+(i*cel_dim), cel_dim, cel_dim, MLV_COLOR_YELLOW);
                MLV_draw_text_box(x_corner_map+(j*cel_dim), y_corner+(i*cel_dim), cel_dim, cel_dim, num, 10, MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_YELLOW, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
			}
			else if (i > 0 && j == 0) {     // Lines
                letter[0]= 'A'+i-1+' ';
				//MLV_draw_filled_rectangle(x_corner+(j*cel_dim), y_corner+(i*cel_dim), cel_dim, cel_dim, MLV_COLOR_YELLOW);
                MLV_draw_text_box(x_corner_map+(j*cel_dim), y_corner+(i*cel_dim), cel_dim, cel_dim, letter, 10, MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_YELLOW, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
			}
			else {                          // Slots            
			    //MLV_draw_filled_rectangle(x_corner+(j*cel_dim), y_corner+(i*cel_dim), cel_dim, cel_dim, MLV_COLOR_BLUE);
                //MLV_draw_text_box(x_corner+(j*cel_dim), y_corner+(i*cel_dim), cel_dim, cel_dim, ".", 10, MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_BLUE, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
                MLV_draw_image (water, x_corner_map+(j*cel_dim), y_corner+(i*cel_dim));
            }
            MLV_draw_line(x_corner_map+(j*cel_dim), y_corner, x_corner_map+(j*cel_dim), y_corner+tab_dim, MLV_COLOR_BLACK);
		}
        MLV_draw_line(x_corner_map+(j*cel_dim), y_corner, x_corner_map+(j*cel_dim), y_corner+tab_dim, MLV_COLOR_BLACK);
        MLV_draw_line(x_corner_map, y_corner+(i*cel_dim), x_corner_map+tab_dim, y_corner+(i*cel_dim), MLV_COLOR_BLACK);
	}
    MLV_draw_line(x_corner_map, y_corner+(i*cel_dim), x_corner_map+tab_dim, y_corner+(i*cel_dim), MLV_COLOR_BLACK);

    MLV_free_image(water);
    free(num);
    free(letter);
}

/* Display the 2 maps */
void displayMaps(Fleet *p_fleet, char **map_def, char **map_att, int player, int *alert_tab) {
    int i, j, k, l;
    Ship *current_ship; // pointer to the ship that is placed
    current_ship = &(p_fleet->carrier); // pointer initialized to the first ship Carrier
    MLV_Image *ship_img[5];
    MLV_Image *flamme, *splash;
    char *file_name = malloc(16 * sizeof(char));
    char *num = malloc(3 * sizeof(char));

    flamme = image("flamme","","png");
    splash = image("splash","","png");

    // Hide the previous display
	MLV_clear_window(MLV_COLOR_BLACK);

    // Defensive Map
    MLV_draw_text_box(
         x_corner_def, y_corner-20, tab_dim, 20,
        "Defensive Map", 9,
         MLV_COLOR_BLACK, MLV_COLOR_BLUE, MLV_COLOR_WHITE,
         MLV_TEXT_LEFT, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
    );
	displayOneMap(0, x_corner_def);

    // Offensive Map
    MLV_draw_text_box(
         x_corner_att, y_corner-20, tab_dim, 20,
        "Offensive Map", 9,
         MLV_COLOR_BLACK, MLV_COLOR_BLUE, MLV_COLOR_WHITE,
         MLV_TEXT_LEFT, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
    );
	displayOneMap(1, x_corner_att);

    // Display player's ships

    for (i = 0; i<NSHIPS; i++) {
        if(current_ship->slot.line != -1 && current_ship->slot.column != -1) { // if position had been defined

            // Load the image files of the ship
            strcpy(file_name, "fleet/");
            strcat(file_name, current_ship->name);         
            for (j = 0; j < current_ship->length; j++) {
                sprintf(num,"%d",j+1);
                ship_img[j] = image(file_name,num,"png");       // Load each img of the actual ship
                MLV_rotate_image(ship_img[j], current_ship->orientation*(-90));
            }

            // Draw the ship in the right orientation
            if (current_ship->orientation == 0) {
                for (k = 0; k < current_ship->length; k++) {
                    MLV_draw_image (ship_img[k], x_corner_def+(current_ship->slot.column)*cel_dim+k*cel_dim, y_corner+(current_ship->slot.line)*cel_dim);
                }
            }
            else if (current_ship->orientation == 1) {
                for (k = 0; k < current_ship->length; k++) {
                    MLV_draw_image (ship_img[k], x_corner_def+(current_ship->slot.column)*cel_dim, y_corner+(current_ship->slot.line)*cel_dim+k*cel_dim);
                }
            }

            // Free the image files
            for (l = 0; l < current_ship->length; l++) {
                MLV_free_image(ship_img[l]);
            }

        }
        
        current_ship += 1; // the pointer changes to the next ship
    }

            // Display shots in defensive map
            for (i = 0; i < NDIM; i++) {
                for (j = 0; j < NDIM; j++) {
                    if (map_def[i][j] == 'X') {
                        MLV_draw_image (flamme, x_corner_def+j*cel_dim, y_corner+i*cel_dim);
                    }
                    if (map_def[i][j] == 'O') {
                        MLV_draw_filled_rectangle(x_corner_def+j*cel_dim, y_corner+i*cel_dim, cel_dim, cel_dim, MLV_COLOR_BLUE);
                        MLV_draw_image (splash, x_corner_def+j*cel_dim, y_corner+i*cel_dim);
                    }
                }
            }

            // Display shots in offensive map
            for (i = 0; i < NDIM; i++) {
                for (j = 0; j < NDIM; j++) {
                    if (map_att[i][j] == 'X') {
                        MLV_draw_image (flamme, x_corner_att+j*cel_dim, y_corner+i*cel_dim);
                    }
                    if (map_att[i][j] == 'O') {
                         MLV_draw_filled_rectangle(x_corner_att+j*cel_dim, y_corner+i*cel_dim, cel_dim, cel_dim, MLV_COLOR_BLUE);
                         MLV_draw_image (splash, x_corner_att+j*cel_dim, y_corner+i*cel_dim);
                    }
                }
            }

            // DISPLAY ALERTS
                // Player
                whatPlayer(player);
    
    MLV_actualise_window();

    MLV_free_image(flamme);
    MLV_free_image(splash);

    free(file_name);
    free(num);
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

/* START */

int division(int a, int b) {
    return a/b;
}

void rotationImg(MLV_Image *img, int orientation) {
    if (orientation == 0) {
        MLV_rotate_image(img, 90);
    } else {
         MLV_rotate_image(img, -90);
    }
}

// Give Orientation + selected slot
int putShip(Fleet *p_fleet, MLV_Image *ship[], int length, char **map, char **map_att, int *l, int *c, int *x, int *y, int *o, int player, int *alert_tab) {
    int i, j, k;
    int select = 0;
    //int m_x, m_y;
    int p_x = 0, p_y = 0;
    MLV_Music* rotate = MLV_load_music("sound/rotate.mp3");

    MLV_init_audio();

    *l = 1; // Reset line cursor
    *c = 1; // Reset column cursor

    // Instructions
        // MLV_draw_text_box(
        //     WIDTH/4, y_corner-100, tab_dim*2, 50,
        //     "Cliquer sur un bateau pour le sélectionner et le positionner sur la grille.\nAppuyer sur espace pour changer sa rotation.", 9,
        //     MLV_COLOR_BLACK, MLV_COLOR_GREY, MLV_COLOR_WHITE,
        //     MLV_TEXT_LEFT, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
        // );
        // MLV_actualise_window();
    
    // Keyboard & Mouse management // DO NOT PUT PRINTF INSIDE THIS LOOP
    do {
        MLV_get_mouse_position(x,y);

        // Display a ship
        displayMaps(p_fleet, map, map_att, player, alert_tab);
        //MLV_get_mouse_position(&m_x,&m_y);
        if ((*x>=(x_corner_def+cel_dim) && *x <= x_corner_def+tab_dim) && (*y>=(y_corner+cel_dim) && *y<=y_corner+tab_dim)) { // If mouse inside the grid
            for (i = 0; i < length; i++) {
                if( *o == 0 ) {
                    p_x = division(*x,cel_dim)*cel_dim+(cel_dim*i)-6; // calculate the nearest X position for a ship from mouse position
                    p_y = (division(*y,cel_dim)*cel_dim)-0.5*cel_dim; // calculate the nearest Y position for a ship from mouse position
                } else {
                    p_x = division(*x,cel_dim)*cel_dim-6; // calculate the nearest X position for a ship from mouse position
                    p_y = (division(*y,cel_dim)*cel_dim)-0.5*cel_dim+(cel_dim*i); // calculate the nearest Y position for a ship from mouse position
                }
                MLV_draw_image (ship[i], p_x, p_y); // draw the ship
            }
        }
        MLV_actualise_window();

        // If space bar pressed
        if (MLV_get_keyboard_state(MLV_KEYBOARD_SPACE)==MLV_PRESSED) {
            if (*o == 0)
                *o = 1;                                       // Change orientation
            else if (*o == 1)
                 *o = 0;

            for(k = 0; k < length; k++) {
               rotationImg(ship[k], *o);
            }

            MLV_play_music(rotate, 1.0, 1);

            MLV_actualise_window();
        }
        // If left click
        if (MLV_get_mouse_button_state(MLV_BUTTON_LEFT)==MLV_PRESSED) {                          
            if ((*x>=(x_corner_def+cel_dim) && *x <= x_corner_def+tab_dim) && (*y>=(y_corner+cel_dim) && *y<=y_corner+tab_dim)) {   // If mouse pressed inside the grid
                for (i=(y_corner+2*cel_dim); i<=(y_corner+tab_dim); i=i+cel_dim) {          // Lines
                    for (j=(x_corner_def+2*cel_dim); j<=(x_corner_def+tab_dim); j=j+cel_dim) {      // Columns
                        if (*y<=i && *x<=j) {     // If it's inside the last cel
                            *x=j-cel_dim;         // x = corner of that cel
                            *y=i-cel_dim;
                            select = 1;           // select worked
                        }
                        if (select == 1)
                            break;
                        *c = *c+1;
                    }
                    if (select == 1)
                        break;
                    *c=1;
                    *l = *l+1;
                }
            }
        }
        //MLV_wait_seconds(2);
    } while(select != 1);

    MLV_free_music(rotate);
    MLV_free_audio();

    return *o;
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
            else   {                // If it's out of the grid
                return 0;
            }                                     
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
void placeShip(Fleet *p_fleet, char **map, char **map_att, int *l, int *c, Ship *p_ship, int num_ship, int *x, int *y, int player, int *alert_tab) {
    int i, j;
    int o = 0;
    int checkposition;
    char *file_name = malloc(16 * sizeof(char));
    char *num = malloc(3 * sizeof(char));
    MLV_Image *ship_img[5];
    MLV_Music* error = MLV_load_music("sound/error.mp3");
    MLV_Music* place = MLV_load_music("sound/place.mp3");

    MLV_init_audio();

    // Load the image files
    strcpy(file_name, "fleet/");
    strcat(file_name, p_ship->name);         
    for (j = 0; j < p_ship->length; j++) {
        sprintf(num,"%d",j+1);
        ship_img[j] = image(file_name,num,"png");       // Load each img of the actual ship
    }

    // Select a valid Position
    do {
        o = putShip(p_fleet, ship_img, p_ship->length, map, map_att, l, c, x, y, &o, player, alert_tab);
        printf("l = %d c = %d\n",*l, *c);
        checkposition = checkPlacement(map, l, c, o, p_ship->length);
        if (checkposition == 0) {
            // MLV_draw_text_box(
            //     80, y_corner+(tab_dim/2), 250, 70,
            //     "You ship can not be placed here.\nPlease put it somewhere else", 9,
            //     MLV_COLOR_RED, MLV_COLOR_RED, MLV_COLOR_WHITE,
            //     MLV_TEXT_LEFT, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
            // );
            // MLV_actualise_window();
            printf("bateau mal placé \n o = %d\n", o);

            MLV_play_music(error, 1.0, 1); // (music, volume, nb of plays)
        }
        else {
            MLV_play_music(place, 1.0, 1);
            MLV_wait_seconds(1);
        }
    } while(checkposition==0);

    // Free music
    MLV_free_music(error);
    MLV_free_music(place);
    MLV_free_audio();

    // Load the image files
    strcpy(file_name, "fleet/");
    strcat(file_name, p_ship->name);         
    for (j = 0; j < p_ship->length; j++) {
        sprintf(num,"%d",j+1);
        ship_img[j] = image(file_name,num,"png");       // Load each img of the actual ship
        MLV_rotate_image(ship_img[j], o*(-90));
    }

    if (checkposition == 1) {

        if (o == 0) {
            for (i = 0; i < p_ship->length; i++) {
                MLV_draw_image (ship_img[i], (*x)+i*cel_dim, *y);
            }
        }
        else if (o == 1) {
            for (i = 0; i < p_ship->length; i++) {
                MLV_draw_image (ship_img[i], *x, (*y)+i*cel_dim);
            }
        }
    }

    MLV_actualise_window();

    free(file_name);
    free(num);
    for (i = 0; i < p_ship->length; i++) {
            MLV_free_image(ship_img[i]);
    }


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

    MLV_wait_seconds(1); // Wait 1 second
}

/* Place all ships */
void placeFleet(char **map, char **map_att, int *l, int *c, Fleet *p_fleet, int *x, int *y, int player, int *alert_tab) {
    int i;
    Ship *current_ship; // pointer to the ship that is placed
    current_ship = &(p_fleet->carrier); // pointer initialized to the first ship Carrier
    for (i = 0; i<NSHIPS; i++) {
        printf("Set %s (%d)\n", current_ship->name, current_ship->length);
        placeShip(p_fleet, map, map_att, l, c, current_ship, i, x, y, player, alert_tab);
        MLV_actualise_window();
        //displayMap(map);
        current_ship += 1; // the pointer changes to the next ship
    }
}

void flemme(char **map, char **map_att, int *l, int *c, Fleet *p_fleet, int *x, int *y, int player, int *alert_tab) { // pour placer qu'un bateau parce que sinon c'est relou
    Ship *current_ship; // pointer to the ship that is placed
    current_ship = &(p_fleet->carrier); // pointer initialized to the first ship Carrier
    printf("Set %s (%d)\n", current_ship->name, current_ship->length);
    placeShip(p_fleet, map, map_att, l, c, current_ship, 0, x, y, player, alert_tab);
    //displayMap(map);
}

// ----------- ENDING OF FLEET POSITIONNING FUNCTIONS -------------- //

// ----------- START OF FLEET ATTACKING FUNCTIONS -------------- //

/* Select a slot | put the line & the column values in variables l and c and coordinates in x and y */
// Return 1 if succeed, 0 if not
int selectSlot(char **map, int *l, int *c, int *x, int *y) {
    int i, j;
    int b = 0;

    *l = 1; // Reset line cursor
    *c = 1; // Reset column cursor
    MLV_wait_mouse(x, y);
    if ((*x>=(x_corner_att+cel_dim) && *x <= x_corner_att+tab_dim) && (*y>=(y_corner+cel_dim) && *y<=y_corner+tab_dim)) {   // If mouse pressed inside the grid
        for (i=(y_corner+2*cel_dim); i<=(y_corner+tab_dim); i=i+cel_dim) {
             for (j=(x_corner_att+2*cel_dim); j<=(x_corner_att+tab_dim); j=j+cel_dim) {
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

/* TURN */

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

    return -1;
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
void attackFleet(char **my_map_def, char **map_att, char **map_def, int *l, int *c, Fleet *my_fleet, Fleet *p_fleet, int *adversary_life, int *x, int *y, int player, int *alert_tab) {
    int check = 1;
    int select = 0;
    MLV_Music* miss = MLV_load_music("sound/splash.mp3");
    MLV_Music* hit  = MLV_load_music("sound/hit.wav");

    MLV_init_audio();

    displayMaps(my_fleet, my_map_def, map_att, player, alert_tab);
    
        MLV_draw_text_box(
                 80, 80, 250, 70,
                 "PLAYER 1 ATTACKS !", 9,
                 MLV_COLOR_RED, MLV_COLOR_RED, MLV_COLOR_WHITE,
                 MLV_TEXT_LEFT, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
             );

    printf("It's your time to attack !\n");
    do {
        do {
            do {
                select = selectSlot(map_att, l, c, x, y);
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
                displayMaps(p_fleet, map_def, map_att, player, alert_tab);
                break;
            }
        }

        else if (check == 0) {           // If miss
            map_att[*l][*c] = 'O';
            map_def[*l][*c] = 'O';
            MLV_play_music(miss, 1.0, 1);
        }

        displayMaps(my_fleet, my_map_def, map_att, player, alert_tab);
    } while (check == 1);            // Attack while success

    MLV_wait_seconds(2);
    MLV_free_music(hit);
    MLV_free_music(miss);
    MLV_free_audio();
    
}

/* Free memory */
void freeGame(char **p_att,char **p_def, Fleet *p_fleet) {
    free(p_att);
    free(p_def);
    free(p_fleet);
}

// ALERT FUNCTIONS

void whatPlayer(char no) {
    char alert = "PLAYER ";
    strcat(&alert, &no);
    strcat(&alert, " ATTACKS!");
     MLV_draw_text_box(
                 80, 80, 250, 70,
                 &alert, 9,
                 MLV_COLOR_RED, MLV_COLOR_RED, MLV_COLOR_WHITE,
                 MLV_TEXT_LEFT, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
             );
}