#include "lab1.h"

int main(void){
	
	/* Values for these come from file parsing */
	Coord waypoint_previouspos, waypoint_nextpos;
	Coord position_previous, position_current;
	
	double tas, heading, altituderate, timespan;
	double dist, climb, dist_error;
	
	double totaldistance = 0;
	
	/* waypoint_previous.latitude = 38.7812995911 * M_PI/180;
	 * waypoint_previous.longitude = -9.13591957092 * M_PI/180;
	 * waypoint_previous.altitude = 374 * FT2METER;
	 * waypoint_next.latitude = 49.0127983093 * M_PI/180;
	 * waypoint_next.longitude = 2.54999995232 * M_PI/180;
	 * waypoint_next.altitude = 392 * FT2METER; */
	
	read_file(waypointlist);
	
	/* Here starts a loop run for every position check. This loop receives
	 * the coordinates of the previous position check, the previous heading,
	 * the velocity and altitude rate maintained, and the time that has
	 * passed since the last position check.
	 * In return, it defines the new heading and altitude rate to follow.
	 * This loop is meant to run between two waypoints, so the velocity will
	 * remain theoretically constant. */
	
	/* First two waypoints */
	waypoint_previouspos.latitude = waypointlist[0].latitude; waypoint_previouspos.longitude = waypointlist[0].longtiude; waypoint_previouspos.altitude = waypointlist[0].altitude;
	waypoint_nextpos.latitude = waypointlist[1].latitude; waypoint_nextpos.longitude = waypointlist[1].longtiude; waypoint_nextpos.altitude = waypointlist[1].altitude;
	
	position_current = waypoint_previouspos;
	
	tas = waypointlist[0].tas; /* m/s */
	altituderate = 0; /* m/s */
	
	/* Start of theoretical loop */
	for(;;){
		position_previous = position_current;
		
		if(coord_dist(position_current, waypoint_next) > tas * TIMESPAN){
			dist = TIMESPAN * tas;
			climb = TIMESPAN * altituderate;
			
			position_current = coord_fromdist(position_previous, dist, heading, climb);
			
			heading = depheading(position_current, waypoint_next);
			altituderate = -ALTRATE_MOD * position_current.altitude + ALTRATE_MOD * waypoint_next.altitude;
			
			totaldistance = totaldistance + dist;
			printf("Latitude = %f, Longitude = %f, Altitude = %fm, Heading = %f, Distance left = %fm\n",
				position_current.latitude * 180/M_PI, position_current.longitude * 180/M_PI, position_current.altitude,heading, coord_dist(position_current, waypoint_next));
		}
		else{
			dist = coord_dist(position_current, waypoint_next);
			climb = dist / tas * altituderate;
			
			position_current = coord_fromdist(position_previous, dist, heading, climb);
			
			totaldistance = totaldistance + dist;
			
			dist_error = coord_dist(position_current, waypoint_nextpos);
			position_current = waypoint_nextpos;
			
			break;
		}
	}
	/* End of theoretical loop */
	/* End of first two waypoints */
	
	dist = coord_dist(waypoint_previouspos, waypoint_nextpos);
	
	printf("Distance between waypoints: %f\n", dist);
	printf("Total distance traveled: %f\n", totaldistance);
	printf("Number of theoretical subpoints at current TAS: %f\n", dist / (TIMESPAN * tas) - 1);
	printf("Number of subpoints traveled at current TAS: %f\n", totaldistance / (TIMESPAN * tas) - 1);
	
	return 0;
}
