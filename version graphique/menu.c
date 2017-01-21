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

    splash_screen = MLV_load_image("img/splash_screen.jpg");
    //MLV_resize_image_with_proportions(splash_screen, WIDTH, HEIGHT);
    MLV_draw_image (splash_screen, 0, 0);

    MLV_draw_text_box(
         WIDTH/4, 580, 
         WIDTH/2, 100, 
         "PRESS ANY KEY TO START", 10, 
         MLV_COLOR_BLACK, MLV_COLOR_BLACK, 
         MLV_COLOR_YELLOW, MLV_TEXT_CENTER, 
         MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
    );

    MLV_actualise_window();

    MLV_wait_keyboard (NULL, NULL, NULL);

    MLV_clear_window(MLV_COLOR_BLACK);
    MLV_free_image(splash_screen);
}

int displayMenu(int *x, int *y) {
    int game = 0;
    MLV_Sound* select;

    MLV_init_audio();
    select = MLV_load_sound("sound/select.ogg");

    splashScreen();

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

    MLV_actualise_window();

    // MENU SELECTION
    do {
            MLV_wait_mouse(x, y);
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
    } while(game == 0);
    printf("Game = %d\n", game);
    MLV_wait_seconds(1);
    //MLV_stop_all_sounds();

    MLV_clear_window(MLV_COLOR_BLACK);

    MLV_free_sound(select);
    MLV_free_audio();

    return game;
}

