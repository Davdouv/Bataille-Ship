/* * * * * * * * * * * * * * */
/*      BATAILLE  NAVALE     */
/*       par David Nasr      */
/*       et Daphné Rose      */
/* * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "functions.h"

int main() {
    char **p1_att, **p1_def, **p2_att, **p2_def;
    p1_att = p1_def = p2_att = p2_def = initMap();
    createMap(p1_att);
    createMap(p1_def);
    createMap(p2_att);
    createMap(p1_def);
    /* Map display (this is temporary, I just want to see what I'm doing) */

    

    return 0;
}