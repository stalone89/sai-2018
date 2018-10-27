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
	int i = 0;
	
	char line[100];
	
	FILE* fid;
	
	if ((fid = fopen(filename,"r")) == NULL){
		printf("Error opening waypoints.csv file.\n");
		return 1;
	}
	
	fgets(line,100,fid); /* Skip first line */
	
	while (!feof(fid)){
		fgets(line,100,fid);
		waypointlist[i] = csv_waypoint_parse(line);
		printf("Latitude = %f, Longitude = %f, Altitude = %fm, TAS = %f, Location = %s\n", waypointlist[i].latitude, waypointlist[i].longitude, waypointlist[i].altitude, waypointlist[i].tas, waypointlist[i].location);
		i++;
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
	i++;
	
	while(field != NULL){
		field = strtok(NULL, ",");
		switch(i){
			case 0:
				waypoint.location = field;
			case 1:
				sscanf(field, "%lf", &waypoint.latitude);
			case 2:
				sscanf(field, "%lf", &waypoint.longitude);
			case 3:
				sscanf(field, "%lf", &waypoint.altitude);
			case 4:
				sscanf(field, "%lf", &waypoint.tas);
		}
		i++;
	}
	
	return waypoint;
}
