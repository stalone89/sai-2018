#include "lab1.h"

int main(void){
	
	/* Values for these come from file parsing */
	Coord waypoint_previous, waypoint_next;
	Coord position_previous, position_current;
	
	Coord waypointlist[12];
	
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
	
	read_file(waypointlist);
	
	return 0;
}
