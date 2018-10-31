/* waypoints.h */
#ifndef WAYPOINTS_H
#define WAYPOINTS_H

#include "lab1.h"
#define BUFFER 100			/* Array initializing buffer */

/* Coordinate structure, with latitude and longitude in radians and altitude in meters. */
typedef struct Coord{
	double latitude;
	double longitude;
	double altitude;
} Coord ;

/* Coordinate structure, with latitude and longitude in radians and altitude in meters. */
typedef struct Waypoint{
	double latitude;
	double longitude;
	double altitude;
	double tas;
	char location[BUFFER];
} Waypoint ;

double coord_dist(Coord coord1, Coord coord2);
Coord coord_fromdist(Coord coord1, double dist, double heading, double climb);
double depheading(Coord coord1, Coord coord2);
double appheading(Coord coord1, Coord coord2);
Coord waypoint_to_coord (Waypoint waypoint);
double gen_subpoints(Coord* subpointlist, Coord waypoint_prev, Coord waypoint_next, double tas);



#endif
