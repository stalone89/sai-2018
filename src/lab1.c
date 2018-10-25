#include "lab1.h"

double coord_dist(Coord waypoint1, Coord waypoint2){
	/* Accepts a waypoint struct.
	 * Returns the horizontal distance between waypoints in meters. */
	
	double ortho = acos(cos(waypoint2.latitude) * cos(waypoint1.longitude - waypoint2.longitude) * cos(waypoint1.latitude) + sin(waypoint2.latitude) * sin(waypoint1.latitude));
	double climb = waypoint2.altitude - waypoint1.altitude;
	
	return sqrt(pow(ortho * EARTH_RADIUS, 2) + pow(climb, 2));
}

Coord coord_fromdist(Coord waypoint1, double dist, double heading, double climb){
	/* Accepts a waypoint struct, a distance in meters, a heading in radians, and a climb distance in meters.
	 * Returns a waypoint struct. */
	
	Coord waypoint2;
	
	waypoint2.latitude = asin(sin(waypoint1.latitude) * cos(dist / EARTH_RADIUS) + cos(waypoint1.latitude) * sin(dist / EARTH_RADIUS) * cos(heading));
	waypoint2.longitude = waypoint1.longitude + atan2(sin(heading) * sin(dist / EARTH_RADIUS) * cos(waypoint1.latitude), cos(dist / EARTH_RADIUS) - sin(waypoint1.latitude) * sin(waypoint2.latitude));
	waypoint2.altitude = waypoint1.altitude + climb;
	
	return waypoint2;
}

double depheading(Coord waypoint1, Coord waypoint2){
	/* Accepts two waypoint structs.
	 * Returns departure heading (from north) in radians. */
	
	double depheading = atan2(-cos(waypoint2.latitude) * sin(waypoint1.longitude - waypoint2.longitude), -cos(waypoint2.latitude) * cos(waypoint1.longitude - waypoint2.longitude) * sin(waypoint1.latitude) + sin(waypoint2.latitude) * cos(waypoint1.latitude));
	
	return depheading;
}

double appheading(Coord waypoint1, Coord waypoint2){
	/* Accepts two waypoint structs.
	 * Returns approach heading (from north) in radians. */
	
	double appheading = atan2(-sin(waypoint1.longitude - waypoint2.longitude) * cos(waypoint1.latitude), sin(waypoint2.latitude) * cos(waypoint1.longitude - waypoint2.longitude) * cos(waypoint1.latitude) - cos(waypoint2.latitude) * sin(waypoint1.latitude));
	
	return appheading;
}

int read_file(Coord* waypointlist){
	/* Accepts a Coord array to fill with waypoints from a file.
	 * Returns 0 in case of success, 1 in case of file opening failure. */
	
	char* filename = "waypoints.csv";
	int i = 0;
	
	char line[100];
	
	FILE* fid;
	
	if ((fid = fopen(filename,"r")) == NULL){
		printf("Error opening waypoints.csv file.\n");
		return 1;
	}
	
	fscanf(fid,"%s\n", line); /* Skip first line */
	
	while (!feof(fid)){
		fscanf(fid,"%s\n", line);
		waypointlist[i] = csv_waypoint_parse(line);
		printf("Latitude = %f, Longitude = %f, Altitude = %fm, TAS = %f\n", waypointlist[i].latitude, waypointlist[i].longitude, waypointlist[i].altitude, waypointlist[i].tas);
		i++;
	}
	
	return 0;
}

Coord csv_waypoint_parse(char line[]){
	/* Accepts a string formatted according to a line from the provided CSV file */
	/* Returns the line's corresponding waypoint */
	
	int i = 0;
	char* field;
	Coord waypoint;
	
	field = strtok(line, ",");
	i++;
	
	while(field != NULL){
		field = strtok(NULL, ",");
		switch(i){
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
