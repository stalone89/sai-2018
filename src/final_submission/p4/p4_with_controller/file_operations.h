#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "lab1.h"
#include "waypoints.h"

int read_file(Waypoint* waypoint);
Waypoint csv_waypoint_parse(char line[]);
FILE* create_log(char filepath[]);
void update_log(FILE* fp, int time, float theta_path, float theta_path_true, float VTAS, float Vm, float true_heading, Coord position, Coord position_true);

#endif
