/* FUNCTIONS.C */
/* DISPLAY FUNCTIONS, GAME MANAGER, AND UTILITY FUNCTIONS */

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
            x_corner_map, y_corner-20, tab_dim, 20,
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
void displayShots(char **map, int x_corner) {
    int i, j;
    MLV_Image *flamme, *splash;

    flamme = image("flamme","","png");
    splash = image("splash","","png");

    // Display shots in defensive map
    for (i = 0; i < NDIM; i++) {
        for (j = 0; j < NDIM; j++) {
            if (map[i][j] == 'X') {
                MLV_draw_image (flamme, x_corner+j*cel_dim, y_corner+i*cel_dim);
            }
            if (map[i][j] == 'O') {
                MLV_draw_filled_rectangle(x_corner+j*cel_dim, y_corner+i*cel_dim, cel_dim, cel_dim, MLV_COLOR_BLUE);
                MLV_draw_image (splash, x_corner+j*cel_dim, y_corner+i*cel_dim);
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
    MLV_actualise_window();
}

// Display the attacking map for solo only
void displayAttackMap (char **map_att) {
    // Hide the previous display
	MLV_clear_window(MLV_COLOR_BLACK);
    displayOneMap(1, x_corner_center);
    displayShots(map_att, x_corner_center);
    MLV_actualise_window();
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
    
    displayShots(map_def, x_corner_def);
    displayShots(map_att, x_corner_att);

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

// Check if mouse is inside of grid YES = 1  NO = 0
int mouseInsideGrid (int *x, int *y, int x_corner) {
    if ((*x>=(x_corner+cel_dim) && *x <= x_corner+tab_dim) && (*y>=(y_corner+cel_dim) && *y<=y_corner+tab_dim))
        return 1;
    else
        return 0;
}

// Free memory
void freeGame(char **p_att, char **p_def, Fleet *p_fleet) {
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

int randomNumber(int a, int b){
    return rand()%(b-a)+a;
}