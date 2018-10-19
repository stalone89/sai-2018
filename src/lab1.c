#include "lab1.h"

/* int read_file(void)
{
	int *readfile (filename);
	
	FILE *pFile;
	
	Coord Waypoints[20];
	int i, latg, latm, lats, longig, longim, longis, alti;
	
	pFile = fopen(filename,"r");
	if (pFile == NULL)
	{
		printf("File Error\n");
		exit(1);
	}
	else
	{
		for (i=0;fscanf(pFile, "%dº%d'%d'' %dº%d'%d'' %d", &latg, &latm, &lats, &longig, &longim, &longis!,&alti=EOF);i++)
		{
			Waypoints[i].latitude = latg + (latm/60) + (lats/(3600));
			Waypoints[i].longitude = longig + (longim/60) + (longis/3600);
			Waypoints[i].altitude = alti;
		}
		return Waypoints;
		fclose(pFile);
	}
} */

double coord_dist(Coord waypoint1, Coord waypoint2){
	/* Accepts a waypoint struct. */
	/* Returns the horizontal distance between waypoints in meters. */
	
	double ortho = acos(cos(waypoint2.latitude) * cos(waypoint1.longitude - waypoint2.longitude) * cos(waypoint1.latitude) + sin(waypoint2.latitude) * sin(waypoint1.latitude));
	double climb = waypoint2.altitude - waypoint1.altitude;
	
	return sqrt(pow(ortho * EARTH_RADIUS, 2) + pow(climb, 2));
}

Coord coord_fromdist(Coord waypoint1, double dist, double heading, double climb){
	/* Accepts a waypoint struct, a distance in meters, a heading in radians, and a climb distance in meters. */
	/* Returns a waypoint struct. */
	
	Coord waypoint2;
	
	waypoint2.latitude = asin(sin(waypoint1.latitude) * cos(dist / EARTH_RADIUS) + cos(waypoint1.latitude) * sin(dist / EARTH_RADIUS) * cos(heading));
	waypoint2.longitude = waypoint1.longitude + atan2(sin(heading) * sin(dist / EARTH_RADIUS) * cos(waypoint1.latitude), cos(dist / EARTH_RADIUS) - sin(waypoint1.latitude) * sin(waypoint2.latitude));
	waypoint2.altitude = waypoint1.altitude + climb;
	
	return waypoint2;
}

double depheading(Coord waypoint1, Coord waypoint2){
	/* Accepts two waypoint structs. */
	/* Returns departure heading (from north) in radians. */
	
	double depheading = atan2(-cos(waypoint2.latitude) * sin(waypoint1.longitude - waypoint2.longitude), -cos(waypoint2.latitude) * cos(waypoint1.longitude - waypoint2.longitude) * sin(waypoint1.latitude) + sin(waypoint2.latitude) * cos(waypoint1.latitude));
	
	return depheading;
}

double appheading(Coord waypoint1, Coord waypoint2){
	/* Accepts two waypoint structs. */
	/* Returns approach heading (from north) in radians. */
	
	double appheading = atan2(-sin(waypoint1.longitude - waypoint2.longitude) * cos(waypoint1.latitude), sin(waypoint2.latitude) * cos(waypoint1.longitude - waypoint2.longitude) * cos(waypoint1.latitude) - cos(waypoint2.latitude) * sin(waypoint1.latitude));
	
	return appheading;
}
