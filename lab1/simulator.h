#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "waypoints.h"

Coord iter(Coord pos1, double v, double deltat, double theta, double heading);
double theta_path(Coord coor1, Coord coor2, double v_hor);
Coord update_position(Coord pos1, double v, double deltat, double theta, double heading);


#endif
