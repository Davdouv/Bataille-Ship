/* MENU.C */
#define _CRT_SECURE_NO_WARNINGS
#define WIDTH 1280
#define HEIGHT 720

#include <MLV/MLV_all.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "menu.h"

void displayMenu() {
    MLV_draw_text(
                    0, 60,
                    "PLAY",
                    MLV_COLOR_GREEN
    );

    MLV_draw_text_box(
         WIDTH/4, 100, 
         WIDTH/2, 200, 
         "PLAYER VS PLAYER", 10, 
         MLV_COLOR_BLACK, MLV_COLOR_BLACK, 
         MLV_COLOR_YELLOW, MLV_TEXT_CENTER, 
         MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
    );

    MLV_actualise_window();
    MLV_wait_seconds(2);
    MLV_clear_window(MLV_COLOR_BLACK);
}

