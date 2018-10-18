#include <stdio.h>
#include <math.h>

#define M_PI 3.14159265358979323846
#define EARTH_RADIUS 6378000
#define ALTRATE_MOD 0.001

#define FT2METER 0.3048

/* Coordinate structure, with latitude and longitude in radians and altitude in meters. */
struct coord {
	double latitude;
	double longitude;
	double altitude;
};

double coord_dist(struct coord waypoint1, struct coord waypoint2){
	/* Accepts a waypoint struct. */
	/* Returns the horizontal distance between waypoints in meters. */
	
	double ortho = acos(cos(waypoint2.latitude) * cos(waypoint1.longitude - waypoint2.longitude) * cos(waypoint1.latitude) + sin(waypoint2.latitude) * sin(waypoint1.latitude));
	double climb = waypoint2.altitude - waypoint1.altitude;
	
	return sqrt(pow(ortho * EARTH_RADIUS, 2) + pow(climb, 2));
}

struct coord coord_fromdist(struct coord waypoint1, double dist, double heading, double climb){
	/* Accepts a waypoint struct, a distance in meters, a heading in radians, and a climb distance in meters. */
	/* Returns a waypoint struct. */
	
	struct coord waypoint2;
	
	waypoint2.latitude = asin(sin(waypoint1.latitude) * cos(dist / EARTH_RADIUS) + cos(waypoint1.latitude) * sin(dist / EARTH_RADIUS) * cos(heading));
	waypoint2.longitude = waypoint1.longitude + atan2(sin(heading) * sin(dist / EARTH_RADIUS) * cos(waypoint1.latitude), cos(dist / EARTH_RADIUS) - sin(waypoint1.latitude) * sin(waypoint2.latitude));
	waypoint2.altitude = waypoint1.altitude + climb;
	
	return waypoint2;
}

double depheading(struct coord waypoint1, struct coord waypoint2){
	/* Accepts two waypoint structs. */
	/* Returns departure heading (from north) in radians. */
	
	double depheading = atan2(-cos(waypoint2.latitude) * sin(waypoint1.longitude - waypoint2.longitude), -cos(waypoint2.latitude) * cos(waypoint1.longitude - waypoint2.longitude) * sin(waypoint1.latitude) + sin(waypoint2.latitude) * cos(waypoint1.latitude));
	
	return depheading;
}

double appheading(struct coord waypoint1, struct coord waypoint2){
	/* Accepts two waypoint structs. */
	/* Returns approach heading (from north) in radians. */
	
	double appheading = atan2(-sin(waypoint1.longitude - waypoint2.longitude) * cos(waypoint1.latitude), sin(waypoint2.latitude) * cos(waypoint1.longitude - waypoint2.longitude) * cos(waypoint1.latitude) - cos(waypoint2.latitude) * sin(waypoint1.latitude));
	
	return appheading;
}

int main(void){
	
	/* struct coord waypoint1, waypoint2;
	waypoint1.latitude = 0.71122167; waypoint1.longitude = -1.29096245; waypoint1.altitude = 0;
	waypoint2.latitude = 0.899425759; waypoint2.longitude = -0.00290946386; waypoint2.altitude = 0;
	
	double dist = coord_dist(waypoint1, waypoint2);
	double depheading = depheading(waypoint1, waypoint2);
	double appheading = appheading(waypoint1, waypoint2);
	struct coord waypoint2_calc = coord_fromdist(waypoint1, dist, depheading);
	
	printf("The distance is %f meters.\n", dist);
	printf("The departure heading is %f degrees and the arrival heading is %f degrees.\n", depheading * 180.0 / M_PI, appheading * 180.0/M_PI);
	printf("Comparing to the original waypoint2, latitude %f and longitude %f,\nCalculating from distance and heading we get latitude %f and longitude %f.\n", waypoint2.latitude, waypoint2.longitude, waypoint2_calc.latitude, waypoint2_calc.longitude);
	*/
	
	
	/* Values for these come from file parsing */
	struct coord waypoint_previous, waypoint_next;
	struct coord position_previous, position_current;
	
	double tas, heading, altituderate, timespan;
	double dist, climb;
	
	double totaldistance = 0;
	
	waypoint_previous.latitude = 38.7812995911 * M_PI/180;
	waypoint_previous.longitude = -9.13591957092 * M_PI/180;
	waypoint_previous.altitude = 374 * FT2METER;
	waypoint_next.latitude = 49.0127983093 * M_PI/180;
	waypoint_next.longitude = 2.54999995232 * M_PI/180;
	waypoint_next.altitude = 392 * FT2METER;
	
	position_current = waypoint_previous;
	
	/* Here starts a loop run for every position check. This loop receives
	 * the coordinates of the previous position check, the previous heading,
	 * the velocity and altitude rate maintained, and the time that has
	 * passed since the last position check.
	 * In return, it defines the new heading and altitude rate to follow.
	 * This loop is meant to run between two waypoints, so the velocity will
	 * remain theoretically constant. */
	tas = 200; /* m/s */
	altituderate = 0; /* m/s */
	timespan = 10 * 60; /* seconds */
	/* Start of theoretical loop */
	while(coord_dist(position_current, waypoint_next) > tas * timespan){ /* End conditions need work */
		position_previous = position_current;
		dist = timespan * tas;
		climb = timespan * altituderate;
		
		position_current = coord_fromdist(position_previous, dist, heading, climb);
		
		heading = depheading(position_current, waypoint_next);
		altituderate = -ALTRATE_MOD * position_current.altitude + ALTRATE_MOD * waypoint_next.altitude;
		
		totaldistance = totaldistance + dist;
		printf("Latitude = %f, Longitude = %f, Altitude = %fm, Heading = %f, Distance left = %fm\n",
			position_current.latitude * 180/M_PI, position_current.longitude * 180/M_PI, position_current.altitude,heading, coord_dist(position_current, waypoint_next));
	}
	/* End of theoretical loop */
	
	dist = coord_dist(waypoint_previous, waypoint_next);
	
	printf("Distance between waypoints: %f\n", dist);
	printf("Number of subpoints at current TAS: %f\n", dist / (timespan * tas) - 1);
	printf("Total distance traveled: %f\n", totaldistance);
	printf("Number of subpoints traveled at current TAS: %f\n", totaldistance / (timespan * tas) - 1);
	
	return 0;
}
