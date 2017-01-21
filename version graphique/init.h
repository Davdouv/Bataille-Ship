#ifndef _INIT_H_
#define _INIT_H_

/* INIT.H */

char** initMap();
void createMap(char **map);
Fleet* initFleet();
void createFleet(Fleet* pt_fleet);
void createPlayer(int* life, char** def_map, char** att_map, Fleet* fleet, int fleet_size);

#endif