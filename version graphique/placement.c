/* PLACEMENT.C */
#define _CRT_SECURE_NO_WARNINGS
#define WIDTH 1280
#define HEIGHT 720

#include <MLV/MLV_all.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "struct.h"
#include "functions.h"
#include "placement.h"

#define NDIM 11     // Grid dimensions
#define NSHIPS 5    // Number of ships MUST NOT EXCEED 5!

#define x_corner_center WIDTH/3 // x coordinate of the top/left corner of the grid
#define x_corner_def WIDTH/5
#define x_corner_att WIDTH/1.8
#define y_corner HEIGHT/4       // y coordinate of the top/left corner of the grid
#define tab_dim HEIGHT/2        // Grid size (it's a square)
//#define cel_dim tab_dim/NDIM    // Cel size (square too)

// START //

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

// Give a position to each img of the ship
void shipPosition (int *x, int *y, int *p_x, int *p_y, int o, int num, int gameSize) {
    float i, j;
    int b = 0;
    int cel_dim = tab_dim/gameSize;

    for (i=(y_corner+2*cel_dim); i<=(y_corner+tab_dim); i=i+cel_dim) {          // Lines
        for (j=(x_corner_center+2*cel_dim); j<=(x_corner_center+tab_dim); j=j+cel_dim) {      // Columns
            if (*y<=i && *x<=j) {     // If it's inside the last cel
                if (o==0)
                {
                    *p_x=j-cel_dim+(cel_dim*num);      // calculate the nearest X position from mouse position and adjust
                    *p_y=i-cel_dim;
                }
                else {
                    *p_x=j-cel_dim;
                    *p_y=i-cel_dim+(cel_dim*num);
                }
                b = 1;
            }
            if (b==1) break;
        }
        if (b==1) break;
    }
/*
    if( o == 0 )
    {
        *p_x = division(*x,cel_dim)*cel_dim+(cel_dim*num); // calculate the nearest X position for a ship from mouse position
        *p_y = (division(*y,cel_dim)*cel_dim)-0.5*cel_dim; // calculate the nearest Y position for a ship from mouse position
    }
    else
    {
       *p_x = division(*x,cel_dim)*cel_dim; // calculate the nearest X position for a ship from mouse position
       *p_y = (division(*y,cel_dim)*cel_dim)-0.5*cel_dim+(cel_dim*num); // calculate the nearest Y position for a ship from mouse position
    }
*/
}

// Give Orientation + selected slot
int putShip(Fleet *p_fleet, MLV_Image *ship[], int length, char **map, char **map_att, int *l, int *c, int *x, int *y, int *o, int *alert_tab, int gameSize, int fleetSize) {
    int i, j, k;
    int select = 0;
    int p_x = 0, p_y = 0;
    int temp_x = p_x;
    int temp_y = p_y;
    int temp_o = 2;
    int draw = 0;
    MLV_Music* rotate = MLV_load_music("sound/rotate.mp3");
    int cel_dim = tab_dim/gameSize;

    MLV_init_audio();

    *l = 1; // Reset line cursor
    *c = 1; // Reset column cursor
    
    // Keyboard & Mouse management // DO NOT PUT PRINTF INSIDE THIS LOOP
    do {
        MLV_get_mouse_position(x,y);

        // If space bar pressed
        if (MLV_get_keyboard_state(MLV_KEYBOARD_SPACE)==MLV_PRESSED) {
            if (*o == 0)
                *o = 1;                                       // Change orientation
            else if (*o == 1)
                 *o = 0;
            
            temp_o = *o;
            displaySettableMap (map, p_fleet, gameSize, fleetSize);

            for(k = 0; k < length; k++) {
               rotationImg(ship[k], *o);
            }

            MLV_play_music(rotate, 1.0, 1);

            MLV_actualise_window();
        }

        // Display a ship on the map
        if (mouseInsideGrid(x, y, x_corner_center, gameSize)) {
            for (i = 0; i < length; i++) {
                shipPosition(x, y, &p_x, &p_y, *o, i, gameSize);

                if(!(temp_x == p_x && temp_y == p_y) || temp_o == *o) {     // If ship position have changed (p_x or p_y or orientation)
                    draw ++;
                    if (draw == 1 && i == 0) {
                        temp_x = p_x;
                        temp_y = p_y;
                        displaySettableMap (map, p_fleet, gameSize, fleetSize);
                    }
                    MLV_draw_image (ship[i], p_x, p_y); // draw the ship
                }
            }
            if (draw != 0) {
                MLV_actualise_window();
            }
            draw = 0;
            if (temp_o == *o) {
                temp_o = 2;
            }
        }

        // If left click
        if (MLV_get_mouse_button_state(MLV_BUTTON_LEFT)==MLV_PRESSED) {                          
             if (mouseInsideGrid(x, y, x_corner_center, gameSize)) {   // If mouse pressed inside the grid
                for (i=(y_corner+2*cel_dim); i<=(y_corner+tab_dim); i=i+cel_dim) {          // Lines
                    for (j=(x_corner_center+2*cel_dim); j<=(x_corner_center+tab_dim); j=j+cel_dim) {      // Columns
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

// Check if slots are free. Return 1 if it is, 0 if it's not
int checkPlacement(char **map, int *l, int *c, int o, int ship_length, int gameSize) {
    int i;
    if (o == 0) {                                   // If it's horizontal
        for (i = 0; i<ship_length; i++) {
            if (*c + i < gameSize) {
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
            if (*l + i < gameSize) {
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

// Players place their ships
void placeShip(Fleet *p_fleet, char **map, char **map_att, int *l, int *c, Ship *p_ship, int num_ship, int *x, int *y, int *alert_tab, int gameSize, int fleetSize) {
    int i, j;
    int o = 0;
    int checkposition;
    char *file_name = malloc(16 * sizeof(char));
    char *num = malloc(3 * sizeof(char));
    MLV_Image *ship_img[5];
    MLV_Music* error = MLV_load_music("sound/error.mp3");
    MLV_Music* place = MLV_load_music("sound/place.mp3");
    int cel_dim = tab_dim/gameSize;

    MLV_init_audio();

    // Load the image files
    strcpy(file_name, "fleet/");
    strcat(file_name, p_ship->name);         
    for (j = 0; j < p_ship->length; j++) {
        sprintf(num,"%d",j+1);
        ship_img[j] = image(file_name,num,"png", gameSize);       // Load each img of the actual ship
    }

    // Select a valid Position
    do {
        o = putShip(p_fleet, ship_img, p_ship->length, map, map_att, l, c, x, y, &o, alert_tab, gameSize, fleetSize);
        printf("l = %d c = %d\n",*l, *c);
        checkposition = checkPlacement(map, l, c, o, p_ship->length, gameSize);
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
        ship_img[j] = image(file_name,num,"png", gameSize);       // Load each img of the actual ship
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

// Place all ships
void placeFleet(char **map, char **map_att, int *l, int *c, Fleet *p_fleet, int *x, int *y, int *alert_tab, int fleetSize, int gameSize) {
    int i;
    Ship *current_ship; // pointer to the ship that is placed
    current_ship = &(p_fleet->carrier); // pointer initialized to the first ship Carrier
    for (i = 0; i<fleetSize; i++) {
        printf("Set %s (%d)\n", current_ship->name, current_ship->length);
        placeShip(p_fleet, map, map_att, l, c, current_ship, i, x, y, alert_tab, gameSize, fleetSize);
        MLV_actualise_window();
        current_ship += 1; // the pointer changes to the next ship
    }
}

void placeRandomFleet(char **map, int *l, int *c, Fleet *ai_fleet, int fleetSize, int gameSize) {
    int i, j;
    int orientation;
    int check = 0;
    Ship *current_ship; // pointer to the ship that is placed

    srand(time(NULL));
    current_ship = &(ai_fleet->carrier); // pointer initialized to the first ship Carrier
    for (i = 0; i<fleetSize; i++) {
        do {
            *l = randomNumber(1, gameSize);
            *c = randomNumber(1, gameSize);
            orientation = rand()%1;
            check = checkPlacement(map, l, c, orientation, current_ship->length, gameSize);
        } while(check == 0);

        current_ship->orientation = orientation;
        current_ship->slot.line = *l;
        current_ship->slot.column = *c;

        if (orientation == 0) {
            for (j = 0; j< current_ship->length; j++) {
                map[*l][*c + j] = '1'+i;
            }
        }
        else {
            for (j = 0; j< current_ship->length; j++) {
                map[*l + j][*c] = '1'+i;
            }
        }

        current_ship += 1; // the pointer changes to the next ship
    }
}

// ----------- ENDING OF FLEET POSITIONNING FUNCTIONS -------------- //