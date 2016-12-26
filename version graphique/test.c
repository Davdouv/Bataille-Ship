#include <MLV/MLV_all.h>
#include <stdio.h>
//
// Attention ! 
// Pour pouvoir compiler ce programme sous windows et sous macintosh,
// il faut, pour la déclaration du main, respecter strictement la syntaxe
// suivante :
//
int main(int argc, char *argv[]){
        int width = 640, height=480;
        //
        // On créé et affiche la fenêtre
        //
        MLV_create_window( "medium - 4 events", "4 events", width, height );
        while( 1 ){
                //
                // On commence par nettoyer l'écran
                //
                MLV_clear_window( MLV_COLOR_BLACK );
                //
                // On récupère puis affiche l'état de la touche a
                //
                if( MLV_get_keyboard_state( MLV_KEYBOARD_k ) == MLV_PRESSED ){
                        MLV_draw_text(  
                                0, 20, "L'utilisateur est en train d'appuyer sur la touche k",
                                MLV_COLOR_GREEN
                        );
                }else{
                        MLV_draw_text( 
                                0, 20,
                                "L'utilisateur n'est pas en train d'appuyer sur la touche k",
                                MLV_COLOR_GREEN
                        );
                }
                
                //
                // On récupère puis affiche la position de la souris
                //
                int x,y;
                MLV_get_mouse_position( &x, &y );
                MLV_draw_text(
                        0, 40,
                        "Position de la souris : (%d, %d)",
                        MLV_COLOR_GREEN,
                        x, y 
                );
                //
                // On récupère puis affiche l'état du bouton gauche de la souris.
                //
                if( MLV_get_mouse_button_state( MLV_BUTTON_LEFT ) == MLV_PRESSED ){
                        MLV_draw_text(
                                0, 60,
                                "L'utilisateur est en train de cliquer sur le bouton gauche de la souris.",
                                MLV_COLOR_GREEN
                        );
                }else{
                        MLV_draw_text(
                                0, 60,
                                "L'utilisateur n'est pas en train de cliquer sur le bouton gauche de la souris.",
                                MLV_COLOR_GREEN
                        );
                }
                //
                // On met à jour l'affichage.
                //
                MLV_actualise_window();
        }
        //
        // On libère la fenêtre
        //
        MLV_free_window();
        return 0;
}