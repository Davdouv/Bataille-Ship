/* FUNCTIONS.C */
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

#define NDIM 11     // Grid dimensions
#define NSHIPS 5    // Number of ships MUST NOT EXCEED 5!

#define x_corner_center WIDTH/3        // x coordinate of the top/left corner of the grid
#define x_corner_def WIDTH/5
#define x_corner_att WIDTH/1.8
#define y_corner HEIGHT/4       // y coordinate of the top/left corner of the grid
#define tab_dim HEIGHT/2        // Grid size (it's a square)
#define cel_dim tab_dim/NDIM    // Cel size (square too)

/* DISPLAY MAP FUNCTIONS */

// Make declaration of image of the size of a cell easier 
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

// Display a single map 
void displayOneMap(int map, int x_corner_map) {
    int i, j;
    char *num = malloc(3 * sizeof(char));
    char *letter = malloc(3 * sizeof(char));
    strncpy(num, "1 \0", 3);
    strncpy(letter, "A \0", 3);
    MLV_Image *water;

    if (map == 0) {
        water = image("water","","jpg");
        MLV_draw_text_box(
            x_corner_map, y_corner-20, tab_dim, 20,
            "Defensive Map", 9,
            MLV_COLOR_BLACK, MLV_COLOR_BLUE, MLV_COLOR_WHITE,
            MLV_TEXT_LEFT, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
        );
    } else {
        water = image("water_att","","jpg");
        MLV_draw_text_box(
            x_corner_att, y_corner-20, tab_dim, 20,
            "Offensive Map", 9,
            MLV_COLOR_BLACK, MLV_COLOR_BLUE, MLV_COLOR_WHITE,
            MLV_TEXT_LEFT, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
        );
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


// Display fleet on defense map
void displayFleet(Fleet *p_fleet, char **map_def, int x_corner) {
    int i, j, k, l;
    Ship *current_ship; // pointer to the ship that is placed
    current_ship = &(p_fleet->carrier); // pointer initialized to the first ship Carrier
    MLV_Image *ship_img[5];
    char *file_name = malloc(16 * sizeof(char));
    char *num = malloc(3 * sizeof(char));

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
                    MLV_draw_image (ship_img[k], x_corner+(current_ship->slot.column)*cel_dim+k*cel_dim, y_corner+(current_ship->slot.line)*cel_dim);
                }
            }
            else if (current_ship->orientation == 1) {
                for (k = 0; k < current_ship->length; k++) {
                    MLV_draw_image (ship_img[k], x_corner+(current_ship->slot.column)*cel_dim, y_corner+(current_ship->slot.line)*cel_dim+k*cel_dim);
                }
            }

            // Free the image files
            for (l = 0; l < current_ship->length; l++) {
                MLV_free_image(ship_img[l]);
            }

        }
        
        current_ship += 1; // the pointer changes to the next ship
    }

    free(file_name);
    free(num);
}

// Display shots
void displayShots(char **map_def, char **map_att) {
    int i, j;
    MLV_Image *flamme, *splash;

    flamme = image("flamme","","png");
    splash = image("splash","","png");

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

    MLV_free_image(flamme);
    MLV_free_image(splash);
}

// Display the map where you can place your fleet
void displaySettableMap (char **map, Fleet *p_fleet) {
    MLV_clear_window(MLV_COLOR_BLACK);
    displayOneMap(0, x_corner_center);
    displayFleet(p_fleet, map, x_corner_center);
}

// Display the 2 maps
void displayMaps(Fleet *p_fleet, char **map_def, char **map_att, int *alert_tab) {
    int i;

    // Hide the previous display
	MLV_clear_window(MLV_COLOR_BLACK);

    // Defensive Map
	displayOneMap(0, x_corner_def);

    // Offensive Map
	displayOneMap(1, x_corner_att);

    displayFleet(p_fleet, map_def, x_corner_def);
    
    displayShots(map_def, map_att);

    // DISPLAY ALERTS
    for (i = 0; i < 10; i++) {
        switch(i) {
            case 0: // Player 1
            if(alert_tab[i] == 1) {
                whatPlayer("1");
            }
            break;

            case 1: // Player 2
            if(alert_tab[i] == 1) {
                whatPlayer("2");
            }
            break;
        }
    }
    
    MLV_actualise_window();
}


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
void shipPosition (int *x, int *y, int *p_x, int *p_y, int o, int num) {
    if( o == 0 )
    {
        *p_x = division(*x,cel_dim)*cel_dim+(cel_dim*num)-6; // calculate the nearest X position for a ship from mouse position
        *p_y = (division(*y,cel_dim)*cel_dim)-0.5*cel_dim; // calculate the nearest Y position for a ship from mouse position
    }
    else
    {
       *p_x = division(*x,cel_dim)*cel_dim-6; // calculate the nearest X position for a ship from mouse position
       *p_y = (division(*y,cel_dim)*cel_dim)-0.5*cel_dim+(cel_dim*num); // calculate the nearest Y position for a ship from mouse position
    }
}

int mouseInsideGrid (int *x, int *y, int x_corner) {
    if ((*x>=(x_corner+cel_dim) && *x <= x_corner+tab_dim) && (*y>=(y_corner+cel_dim) && *y<=y_corner+tab_dim))
        return 1;
    else
        return 0;
}

// Give Orientation + selected slot
int putShip(Fleet *p_fleet, MLV_Image *ship[], int length, char **map, char **map_att, int *l, int *c, int *x, int *y, int *o, int *alert_tab) {
    int i, j, k;
    int select = 0;
    int p_x = 0, p_y = 0;
    int temp_x = p_x;
    int temp_y = p_y;
    int temp_o = 2;
    int draw = 0;
    MLV_Music* rotate = MLV_load_music("sound/rotate.mp3");

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
            displaySettableMap (map, p_fleet);

            for(k = 0; k < length; k++) {
               rotationImg(ship[k], *o);
            }

            MLV_play_music(rotate, 1.0, 1);

            MLV_actualise_window();
        }


        // Display a ship on the map
        if (mouseInsideGrid(x, y, x_corner_center)) {
            for (i = 0; i < length; i++) {
                shipPosition(x, y, &p_x, &p_y, *o, i);

                if(!(temp_x == p_x && temp_y == p_y) || temp_o == *o) {     // If ship position have changed
                    draw ++;
                    if (draw == 1 && i == 0) {
                        temp_x = p_x;
                        temp_y = p_y;
                        displaySettableMap (map, p_fleet);
                    }
                    p_x+=6;
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
             if (mouseInsideGrid(x, y, x_corner_center)) {   // If mouse pressed inside the grid
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

// Players place their ships
void placeShip(Fleet *p_fleet, char **map, char **map_att, int *l, int *c, Ship *p_ship, int num_ship, int *x, int *y, int *alert_tab) {
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
        o = putShip(p_fleet, ship_img, p_ship->length, map, map_att, l, c, x, y, &o, alert_tab);
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

// Place all ships
void placeFleet(char **map, char **map_att, int *l, int *c, Fleet *p_fleet, int *x, int *y, int *alert_tab) {
    int i;
    Ship *current_ship; // pointer to the ship that is placed
    current_ship = &(p_fleet->carrier); // pointer initialized to the first ship Carrier
    for (i = 0; i<NSHIPS; i++) {
        printf("Set %s (%d)\n", current_ship->name, current_ship->length);
        placeShip(p_fleet, map, map_att, l, c, current_ship, i, x, y, alert_tab);
        MLV_actualise_window();
        //displayMap(map);
        current_ship += 1; // the pointer changes to the next ship
    }
}

void flemme(char **map, char **map_att, int *l, int *c, Fleet *p_fleet, int *x, int *y, int *alert_tab) { // pour placer qu'un bateau parce que sinon c'est relou
    Ship *current_ship; // pointer to the ship that is placed
    current_ship = &(p_fleet->carrier); // pointer initialized to the first ship Carrier
    printf("Set %s (%d)\n", current_ship->name, current_ship->length);
    placeShip(p_fleet, map, map_att, l, c, current_ship, 0, x, y, alert_tab);
    //displayMap(map);
}

// ----------- ENDING OF FLEET POSITIONNING FUNCTIONS -------------- //

// ----------- START OF FLEET ATTACKING FUNCTIONS -------------- //

// Select a slot | put the line & the column values in variables l and c and coordinates in x and y
// Return 1 if succeed, 0 if not
int selectSlot(char **map, int *l, int *c, int *x, int *y) {
    int i, j;
    int b = 0;

    *l = 1; // Reset line cursor
    *c = 1; // Reset column cursor
    MLV_wait_mouse(x, y);
    if (mouseInsideGrid(x, y, x_corner_att)) {   // If mouse pressed inside the grid
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

// TURN

// Free memory
void freeGame(char **p_att,char **p_def, Fleet *p_fleet) {
    free(p_att);
    free(p_def);
    free(p_fleet);
}

// CHANGE PLAYER
void changePlayer( int *alert_tab) {
    if (alert_tab[0] == 1) {
        alert_tab[0] = 0;
        alert_tab[1] = 1;
    } else {
        alert_tab[0] = 1;
        alert_tab[1] = 0;
    }
}

// Transition screen
void transitionScreen(int *alert_tab) {
    MLV_clear_window(MLV_COLOR_BLACK);
    if (alert_tab[0] == 1) {
        MLV_draw_text_box(
            WIDTH/4, 250, 
            WIDTH/2, 100, 
            "IT'S PLAYER 2 TURN!\n PRESS ANY KEY TO CONTINUE", 10, 
            MLV_COLOR_BLACK, MLV_COLOR_BLACK, 
            MLV_COLOR_YELLOW, MLV_TEXT_CENTER, 
            MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
        );
    } else {
        MLV_draw_text_box(
            WIDTH/4, 250, 
            WIDTH/2, 100, 
            "IT'S PLAYER 1 TURN!\n PRESS ANY KEY TO CONTINUE", 10, 
            MLV_COLOR_BLACK, MLV_COLOR_BLACK, 
            MLV_COLOR_YELLOW, MLV_TEXT_CENTER, 
            MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
        );
    }
    

        MLV_actualise_window();

        MLV_wait_keyboard (NULL, NULL, NULL);

        MLV_clear_window(MLV_COLOR_BLACK);
}

// ALERT FUNCTIONS
void whatPlayer(char *no) {
    char alert;
    strcat(&alert, "PLAYER ");
    strcat(&alert, no);
    strcat(&alert, "\0");
    MLV_draw_text_box(
                 537, 20, 250, 70,
                 &alert, 9,
                 MLV_COLOR_RED, MLV_COLOR_RED, MLV_COLOR_WHITE,
                 MLV_TEXT_LEFT, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
             );
}
