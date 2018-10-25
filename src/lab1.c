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
	 * File syntax isn't ideal, could use work.
	 * Returns 0 in case of success, 1 in case of file opening failure. */
	
	/* char filename[13] = "waypoints.csv"; */
	
	char city[20];
	double latitude, longitude, altitude, tas;
	
	int i = 0;
	
	FILE* fid;
	
	if ((fid = fopen("waypoints.csv","r")) == NULL){
		printf("Error opening waypoint file.\n");
		return(1);
	}
	
	while (!feof(fid)){
		if (fscanf(fid,"%s %lf %lf %lf %lf\n", city, &latitude, &longitude, &altitude, &tas)){
			printf("Lat/Lon = %f %f / Altitude = %f / TAS = %f / City is %s\n", latitude, longitude, altitude, tas, city);
		}
		
		waypointlist[i].latitude = latitude;
		waypointlist[i].longitude = longitude;
		waypointlist[i].altitude = altitude;
		waypointlist[i].tas = tas;
		
		i++;
	}
	
	return 0;
}