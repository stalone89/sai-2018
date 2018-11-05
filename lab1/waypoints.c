#include "waypoints.h"

double coord_dist(Coord coord1, Coord coord2){
	/* Accepts a coord struct.
	 * Returns the great circle distance between coordinates in meters. */
	
	double ortho = acos(cos(coord2.latitude) * cos(coord1.longitude - coord2.longitude) * cos(coord1.latitude) + sin(coord2.latitude) * sin(coord1.latitude));
	
	return (ortho * (EARTH_RADIUS + coord1.altitude));
}

Coord coord_fromdist(Coord coord1, double dist, double heading, double climb){
	/* Accepts a coord struct, a distance in meters, a heading in radians, and a climb distance in meters.
	 * Returns a coord struct. */
	
	Coord coord2;
	
	coord2.latitude = asin(sin(coord1.latitude) * cos(dist / EARTH_RADIUS) + cos(coord1.latitude) * sin(dist / EARTH_RADIUS) * cos(heading));
	coord2.longitude = coord1.longitude + atan2(sin(heading) * sin(dist / EARTH_RADIUS) * cos(coord1.latitude), cos(dist / EARTH_RADIUS) - sin(coord1.latitude) * sin(coord2.latitude));
	coord2.altitude = coord1.altitude + climb;
	
	return coord2;
}

double depheading(Coord coord1, Coord coord2){
	/* Accepts two coord structs.
	 * Returns departure heading (from north) in radians using Great Circles (Orthodrome). */
	
	double depheading = atan2(-cos(coord2.latitude) * sin(coord1.longitude - coord2.longitude), -cos(coord2.latitude) * cos(coord1.longitude - coord2.longitude) * sin(coord1.latitude) + sin(coord2.latitude) * cos(coord1.latitude));
	
	return depheading;
}

double appheading(Coord coord1, Coord coord2){
	/* Accepts two coord structs.
	 * Returns approach heading (from north) in radians using Great Circles (Orthodrome). */
	
	double appheading = atan2(-sin(coord1.longitude - coord2.longitude) * cos(coord1.latitude), sin(coord2.latitude) * cos(coord1.longitude - coord2.longitude) * cos(coord1.latitude) - cos(coord2.latitude) * sin(coord1.latitude));
	
	return appheading;
}


Coord waypoint_to_coord (Waypoint waypoint){
	/* Accepts a waypoint and returns a coordinate struct with its position. */
	Coord coordinate;
	
	coordinate.latitude = waypoint.latitude * M_PI/180;
	coordinate.longitude = waypoint.longitude * M_PI/180;
	coordinate.altitude = waypoint.altitude * FT2METER;
	
	return coordinate;
}

double gen_subpoints(Coord* subpointlist, Coord waypoint_prev, Coord waypoint_next, double tas){
	
	Coord position_previous, position_current;
	double heading, altituderate;
	double dist, climb;
	
	int i = 0;
	
	double totaldist = 0;
	
	position_current = waypoint_prev;
	
	altituderate = 0; /* m/s */
	
	position_previous = position_current;
	heading = depheading(position_current, waypoint_next);
	
	while(coord_dist(position_current, waypoint_next) > tas * TIMESPAN){
		dist = TIMESPAN * tas;
		climb = TIMESPAN * altituderate;
		
		totaldist += dist;
		
		position_current = coord_fromdist(position_previous, dist, heading, climb);
		subpointlist[i] = position_current;
		i++;
		
		heading = depheading(position_current, waypoint_next);
		altituderate = -ALTRATE_MOD * position_current.altitude + ALTRATE_MOD * waypoint_next.altitude;
		
		printf("Latitude = %f, Longitude = %f, Altitude = %fm, Heading = %f, Distance left = %fm\n", position_current.latitude * 180/M_PI, position_current.longitude * 180/M_PI, position_current.altitude, heading, coord_dist(position_current, waypoint_next));
		
		position_previous = position_current;
	}
	
	dist = coord_dist(position_current, waypoint_next);
	climb = dist / tas * altituderate;
	
	totaldist += dist;
	
	position_current = coord_fromdist(position_previous, dist, heading, climb);
	subpointlist[i] = position_current;
	
	printf("Latitude = %f, Longitude = %f, Altitude = %fm, Heading = %f, Distance left = %fm\n", position_current.latitude * 180/M_PI, position_current.longitude * 180/M_PI, position_current.altitude, heading, coord_dist(position_current, waypoint_next));
	
	
	return totaldist;
}
