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
           WIDTH/3-50, HEIGHT-300,
           "CLICK ANYWHERE TO START", font,
           MLV_COLOR_WHITE
    );

    MLV_actualise_window();

    //MLV_wait_keyboard (NULL, NULL, NULL);
    MLV_wait_mouse(NULL, NULL);

    MLV_clear_window(MLV_COLOR_BLACK);
    MLV_free_image(splash_screen);
    MLV_free_font(font);
}

void displayMenu(int fleetSize, int gameSize) {
    int i;
    char* num = malloc(3 * sizeof(char));
    MLV_Font *font_50;
    MLV_Image *bg;
    MLV_Image *pan;

    font_50 = MLV_load_font("font/OpenSans-Bold.ttf", 50);

    MLV_clear_window(MLV_COLOR_BLACK);

    bg = MLV_load_image("img/jardin.jpg");
    pan = MLV_load_image("img/3panneaux.png");
    MLV_draw_image(bg, 0, 0);
    MLV_draw_image(pan, WIDTH/4+50, 0);

    // MENU DISPLAY


    MLV_draw_text_with_font(
           WIDTH/2-60, 180,
           "SOLO", font_50,
           MLV_COLOR_WHITE
    );

    MLV_draw_text_with_font(
           WIDTH/2-220, 320,
           "PLAYER VS PLAYER", font_50,
           MLV_COLOR_WHITE
    );

    MLV_draw_text_with_font(
           WIDTH/2-160, 465,
           "PLAYER VS AI", font_50,
           MLV_COLOR_WHITE
    );

    // FLEET SIZE
    MLV_draw_text_box(
         1100, 100, 
         100, 80, 
         "VEGETABLES", 10, 
         MLV_COLOR_BLACK, MLV_COLOR_BLACK, 
         MLV_COLOR_WHITE, MLV_TEXT_CENTER, 
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
                MLV_COLOR_WHITE, MLV_TEXT_CENTER, 
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
         MLV_COLOR_WHITE, MLV_TEXT_CENTER, 
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
                MLV_COLOR_WHITE, MLV_TEXT_CENTER, 
                MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
            );
        }
    }

    free(num);
    MLV_free_image(bg);
    MLV_free_image(pan);

    MLV_actualise_window();
}

int menuManager(int *x, int *y, int *fleetSize, int *gameSize){
    int game = 0;
    int i;
    MLV_Sound* select;

    select = MLV_load_sound("sound/select.ogg");

    splashScreen();

    displayMenu(*fleetSize, *gameSize);
    // MENU SELECTION
    do {    
            displayMenu(*fleetSize, *gameSize);
            MLV_wait_mouse(x, y);
            printf("X = %d Y = %d\n", *x, *y);
            MLV_play_sound(select, 1.0);
            if (*x >= WIDTH/4+50 && *x <= WIDTH/4+WIDTH/2-50 && *y >= 150 && *y <=275)
            {
                game = 1;           // SOLO
            }
            else if (*x >= WIDTH/4+50 && *x <= WIDTH/4+WIDTH/2-50 && *y >= 290 && *y <=415)
            {
                game = 2;           // PLAYER VS PLAYER
            }
            else if (*x >= WIDTH/4+50 && *x <= WIDTH/4+WIDTH/2-50 && *y >= 430 && *y <=555)
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

    MLV_clear_window(MLV_COLOR_BLACK);

    MLV_free_sound(select);

    return game;
}

// Display how to play message
void howTo() {
    MLV_Font* font_20;
    MLV_Font* font_30;
    MLV_Font* font_50;
    MLV_Image* bg;
    MLV_Image* pan;

    font_20 = MLV_load_font("font/OpenSans-Bold.ttf", 20);
    font_30 = MLV_load_font("font/OpenSans-Bold.ttf", 30);
    font_50 = MLV_load_font("font/OpenSans-Bold.ttf", 50);
    bg = MLV_load_image("img/jardin.jpg");
    pan = MLV_load_image("img/1panneau.png");

    MLV_clear_window(MLV_COLOR_BLACK);
    MLV_draw_image(bg, 0, 0);
    //MLV_draw_image(pan, WIDTH/4+50, HEIGHT/3);
    MLV_draw_image(pan, WIDTH/4+50, 0);

    MLV_draw_text_with_font(
           WIDTH/4+150, 180,
           "HOW TO PLAY", font_50,
           MLV_COLOR_WHITE
    );

    MLV_draw_text_with_font(
           WIDTH/3-10, HEIGHT/3+80,
           "Press LEFT CLICK to select a slot", font_30,
           MLV_COLOR_WHITE
    );
    MLV_draw_text_with_font(
           WIDTH/3-120, HEIGHT/3+130,
           "Press RIGHT CLICK to change ship orientation", font_30,
           MLV_COLOR_WHITE
    );
    MLV_draw_text_with_font(
           WIDTH/3-15, HEIGHT/3+180,
           "If you hit, you can shoot again !", font_30,
           MLV_COLOR_WHITE
    );
/*
    MLV_draw_text_box(
            WIDTH/4, 250, 
            WIDTH/2, 100, 
            "Press left click to select a slot\nPress right click to change ship orientation\nIf you hit, you can shoot again !", 10, 
            MLV_COLOR_BLACK, MLV_COLOR_BLACK, 
            MLV_COLOR_WHITE, MLV_TEXT_CENTER, 
            MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
        );
*/
    MLV_draw_text_with_font(
           WIDTH/2-150, HEIGHT-220,
           "CLICK ANYWHERE TO CONTINUE", font_20,
           MLV_COLOR_WHITE
    );
    
    MLV_actualise_window();
    MLV_wait_seconds(1);
    MLV_wait_mouse(NULL, NULL);
    //MLV_wait_keyboard(NULL, NULL, NULL);

    MLV_free_image(bg);
    MLV_free_image(pan);
}