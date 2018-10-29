#include "lab1.h"

double coord_dist(Coord coord1, Coord coord2){
	/* Accepts a coord struct.
	 * Returns the horizontal distance between coordinates in meters. */
	
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
	 * Returns departure heading (from north) in radians. */
	
	double depheading = atan2(-cos(coord2.latitude) * sin(coord1.longitude - coord2.longitude), -cos(coord2.latitude) * cos(coord1.longitude - coord2.longitude) * sin(coord1.latitude) + sin(coord2.latitude) * cos(coord1.latitude));
	
	return depheading;
}

double appheading(Coord coord1, Coord coord2){
	/* Accepts two coord structs.
	 * Returns approach heading (from north) in radians. */
	
	double appheading = atan2(-sin(coord1.longitude - coord2.longitude) * cos(coord1.latitude), sin(coord2.latitude) * cos(coord1.longitude - coord2.longitude) * cos(coord1.latitude) - cos(coord2.latitude) * sin(coord1.latitude));
	
	return appheading;
}

int read_file(Waypoint* waypointlist){
	/* Accepts a Waypoint array to fill with waypoints from a file.
	 * Returns 0 in case of success, 1 in case of file opening failure. */
	
	char* filename = "waypoints.csv";
	char line[100];
	int i = 0;
	
	FILE* fid;
	
	if ((fid = fopen(filename,"r")) == NULL){
		printf("Error opening waypoints.csv file.\n");
		return 1;
	}
	
	fgets(line,100,fid); /* Skip first line */
	
	while(!feof(fid)){
		line[0] = '\0';
		fgets(line,100,fid);
		
		if(strlen(line) > 30){
			waypointlist[i] = csv_waypoint_parse(line);
			printf("Latitude = %f, Longitude = %f, Altitude = %fm, TAS = %f, Location = %s\n", waypointlist[i].latitude, waypointlist[i].longitude, waypointlist[i].altitude, waypointlist[i].tas, waypointlist[i].location);
			i++;
		}
	}
	
	fclose(fid);
	
	return 0;
}

Waypoint csv_waypoint_parse(char line[]){
	/* Accepts a string formatted according to a line from the provided CSV file */
	/* Returns the line's corresponding waypoint in a Waypoint struct */
	
	int i = 0;
	char* field;
	Waypoint waypoint;
	
	field = strtok(line, ",");
	
	while(field != NULL){
		switch(i){
			case 0:
				strcpy(waypoint.location, field);
			case 1:
				sscanf(field, "%lf", &waypoint.latitude);
			case 2:
				sscanf(field, "%lf", &waypoint.longitude);
			case 3:
				sscanf(field, "%lf", &waypoint.altitude);
			case 4:
				sscanf(field, "%lf", &waypoint.tas);
		}
		
		field = strtok(NULL, ",");
		i++;
	}
	
	return waypoint;
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

Coord iter(Coord pos1, double v, double deltat, double theta, double heading){
	
	Coord pos2;
	
	double v_east, v_north, v_up;
	
	v_east = v * cos(theta) * sin(heading);
	v_north = v * cos(theta) * cos(heading);
	v_up = v * sin(theta);
	
	pos2.latitude = pos1.latitude + ((v_north * deltat) / (pos1.altitude + EARTH_RADIUS));
	pos2.longitude = pos1.longitude + ((v_east * deltat) / (pos1.altitude + EARTH_RADIUS));
	pos2.altitude = pos1.altitude + v_up * deltat;
	
	return pos2;
}
