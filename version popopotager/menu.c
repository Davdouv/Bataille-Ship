/* MENU.C */
#define _CRT_SECURE_NO_WARNINGS
#define WIDTH 1280
#define HEIGHT 720

#include <MLV/MLV_all.h>
#include <stdio.h>
#include <stdlib.h>
#include "menu.h"

void splashScreen() {
    MLV_Image *splash_screen;
    MLV_Font *font;
    font = MLV_load_font("font/OpenSans-Bold.ttf", 40);

    splash_screen = MLV_load_image("img/splash_screen_potager.jpg");
    MLV_draw_image (splash_screen, 0, 0);

    MLV_draw_text_with_font(
           WIDTH/3-20, HEIGHT-300,
           "PRESS ANY KEY TO START", font,
           MLV_COLOR_WHITE
    );

    MLV_actualise_window();

    MLV_wait_keyboard (NULL, NULL, NULL);

    MLV_clear_window(MLV_COLOR_BLACK);
    MLV_free_image(splash_screen);
    MLV_free_font(font);
}

void displayMenu(int fleetSize, int gameSize) {
    int i;
    char* num = malloc(3 * sizeof(char));

    MLV_clear_window(MLV_COLOR_BLACK);

    // MENU DISPLAY
    MLV_draw_text_box(
         WIDTH/4, 100, 
         WIDTH/2, 100, 
         "SOLO", 10, 
         MLV_COLOR_BLACK, MLV_COLOR_BLACK, 
         MLV_COLOR_YELLOW, MLV_TEXT_CENTER, 
         MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
    );

    MLV_draw_text_box(
         WIDTH/4, 300, 
         WIDTH/2, 100, 
         "PLAYER VS PLAYER", 10, 
         MLV_COLOR_BLACK, MLV_COLOR_BLACK, 
         MLV_COLOR_YELLOW, MLV_TEXT_CENTER, 
         MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
    );

    MLV_draw_text_box(
         WIDTH/4, 500, 
         WIDTH/2, 100, 
         "PLAYER VS AI", 10, 
         MLV_COLOR_BLACK, MLV_COLOR_BLACK, 
         MLV_COLOR_YELLOW, MLV_TEXT_CENTER, 
         MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
    );

    // FLEET SIZE
    MLV_draw_text_box(
         1100, 100, 
         100, 80, 
         "FLEET SIZE", 10, 
         MLV_COLOR_BLACK, MLV_COLOR_BLACK, 
         MLV_COLOR_YELLOW, MLV_TEXT_CENTER, 
         MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
    );

    for (i = 1; i <= 5; i++)
    {
        sprintf(num,"%d",i); 
        if(i==fleetSize) {
            MLV_draw_text_box(
                1080+i*20, 180, 
                20, 20, 
                num, 10, 
                MLV_COLOR_BLACK, MLV_COLOR_BLACK, 
                MLV_COLOR_RED, MLV_TEXT_CENTER, 
                MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
            );
        }
        else {
            MLV_draw_text_box(
                1080+i*20, 180, 
                20, 20, 
                num, 10, 
                MLV_COLOR_BLACK, MLV_COLOR_BLACK, 
                MLV_COLOR_YELLOW, MLV_TEXT_CENTER, 
                MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
            );
        }
    }

    // GAME SIZE
    MLV_draw_text_box(
         1100, 500, 
         100, 80, 
         "GAME SIZE", 10, 
         MLV_COLOR_BLACK, MLV_COLOR_BLACK, 
         MLV_COLOR_YELLOW, MLV_TEXT_CENTER, 
         MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
    );

    for (i = 1; i <= 5; i++)
    {
        sprintf(num,"%d",(i+5)); 
        if((i+5)==gameSize-1) {
            MLV_draw_text_box(
                1080+i*20, 580, 
                20, 20, 
                num, 10, 
                MLV_COLOR_BLACK, MLV_COLOR_BLACK, 
                MLV_COLOR_RED, MLV_TEXT_CENTER, 
                MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
            );
        }
        else {
            MLV_draw_text_box(
                1080+i*20, 580, 
                20, 20, 
                num, 10, 
                MLV_COLOR_BLACK, MLV_COLOR_BLACK, 
                MLV_COLOR_YELLOW, MLV_TEXT_CENTER, 
                MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
            );
        }
    }

    free(num);

    MLV_actualise_window();
}

int menuManager(int *x, int *y, int *fleetSize, int *gameSize){
    int game = 0;
    int i;
    MLV_Sound* select;

    MLV_init_audio();
    select = MLV_load_sound("sound/select.ogg");

    splashScreen();

    displayMenu(*fleetSize, *gameSize);
    // MENU SELECTION
    do {
            displayMenu(*fleetSize, *gameSize);
            MLV_wait_mouse(x, y);
            printf("X = %d Y = %d\n", *x, *y);
            MLV_play_sound(select, 1.0);
            if (*x >= WIDTH/4 && *x <= WIDTH/4+WIDTH/2 && *y >= 100 && *y <=200)
            {
                game = 1;           // SOLO
            }
            else if (*x >= WIDTH/4 && *x <= WIDTH/4+WIDTH/2 && *y >= 300 && *y <=400)
            {
                game = 2;           // PLAYER VS PLAYER
            }
            else if (*x >= WIDTH/4 && *x <= WIDTH/4+WIDTH/2 && *y >= 500 && *y <=600)
            {
                game = 3;           // PLAYER VS AI
            }
            else
            {
                game = 0;
            }
            for (i = 1; i<=5; i++){
                if(*x >= 1080+i*20 && *x<=1100+i*20 && *y >= 180 && *y <= 200)
                {
                    *fleetSize = i;
                    printf("FleetSize = %d\n", *fleetSize);
                }
                if(*x >= 1080+i*20 && *x<=1100+i*20 && *y >= 580 && *y <= 600)
                {
                    *gameSize= (i+6);
                    printf("GameSize = %d\n", *gameSize);
                }
            }
    } while(game == 0);
    printf("Game = %d\n", game);
    printf("FleetSize = %d\n", *fleetSize);
    MLV_wait_seconds(1);
    //MLV_stop_all_sounds();

    MLV_clear_window(MLV_COLOR_BLACK);

    MLV_free_sound(select);
    MLV_free_audio();

    return game;
}

// Display how to play message
void howTo() {
    MLV_clear_window(MLV_COLOR_BLACK);

    MLV_draw_text_box(
            WIDTH/4, 250, 
            WIDTH/2, 100, 
            "Press left click to select a slot\nPress space to change ship orientation", 10, 
            MLV_COLOR_BLACK, MLV_COLOR_BLACK, 
            MLV_COLOR_YELLOW, MLV_TEXT_CENTER, 
            MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
        );

    MLV_draw_text_box(
            WIDTH/3, 550, 
            WIDTH/3, 50, 
            "Press any key to continue", 10, 
            MLV_COLOR_BLACK, MLV_COLOR_BLACK, 
            MLV_COLOR_YELLOW, MLV_TEXT_CENTER, 
            MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
        );
    MLV_actualise_window();
    MLV_wait_seconds(1);
    //MLV_wait_mouse(NULL, NULL);
    MLV_wait_keyboard(NULL, NULL, NULL);    
}