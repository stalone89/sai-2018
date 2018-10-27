#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define M_PI 3.14159265358979323846
#define EARTH_RADIUS 6378000

/* Altitude rate modifier */
#define ALTRATE_MOD 0.001

/* Feet to meter ratio */
#define FT2METER 0.3048

/* Character buffer */
#define CHARBUFFER 100

/* Coordinate structure, with latitude and longitude in radians and altitude in meters. */
typedef struct {
	double latitude;
	double longitude;
	double altitude;
} Coord ;

/* Coordinate structure, with latitude and longitude in radians and altitude in meters. */
typedef struct {
	double latitude;
	double longitude;
	double altitude;
	double tas;
	char* location;
} Waypoint ;

/* int read_file(void); */
double coord_dist(Coord waypoint1, Coord waypoint2);
Coord coord_fromdist(Coord waypoint1, double dist, double heading, double climb);
double depheading(Coord waypoint1, Coord waypoint2);
double appheading(Coord waypoint1, Coord waypoint2);
int read_file(Waypoint* waypoint);
Waypoint csv_waypoint_parse(char line[]);