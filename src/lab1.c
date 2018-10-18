#include <stdio.h>
#include <math.h>

#define EARTH_RADIUS 6378000
#define M_PI 3.14159265358979323846

/* Waypoint structure, with latitude and longitude in radians and altitude in meters. */
struct waypoint {
	double latitude;
	double longitude;
	int altitude;
};

double waypoint_dist(struct waypoint waypoint1, struct waypoint waypoint2){
	/* Accepts a waypoint struct. */
	/* Returns the distance between waypoints in meters. */
	
	double ortho = acos(cos(waypoint2.latitude) * cos(waypoint1.longitude - waypoint2.longitude) * cos(waypoint1.latitude) + sin(waypoint2.latitude) * sin(waypoint1.latitude));
	
	/* I'll keep these lines here just in case. */
	/* bearing = atan2(waypoint2.longitude - waypoint1.longitude, sigma(waypoint2.latitude) - sigma(waypoint1.latitude)); */
	/* dist = EARTH_RADIUS * fabs(waypoint2.latitude - waypoint1.latitude) * (1/cos(bearing)); */
	
	return ortho * EARTH_RADIUS;
}

struct waypoint waypoint_fromdist(struct waypoint waypoint1, double dist, double heading){
	/* Accepts a waypoint struct, a distance in meters and a heading in radians. */
	/* Returns a waypoint struct. */
	
	struct waypoint waypoint2;
	
	waypoint2.latitude = asin(sin(waypoint1.latitude) * cos(dist / EARTH_RADIUS) + cos(waypoint1.latitude) * sin(dist / EARTH_RADIUS) * cos(heading));
	waypoint2.longitude = waypoint1.longitude + atan2(sin(heading) * sin(dist / EARTH_RADIUS) * cos(waypoint1.latitude), cos(dist / EARTH_RADIUS) - sin(waypoint1.latitude) * sin(waypoint2.latitude));
	
	return waypoint2;
}

double waypoint_depheading(struct waypoint waypoint1, struct waypoint waypoint2){
	/* Accepts two waypoint structs. */
	/* Returns departure heading (from north) in radians. */
	
	double depheading = atan2(-cos(waypoint2.latitude) * sin(waypoint1.longitude - waypoint2.longitude), -cos(waypoint2.latitude) * cos(waypoint1.longitude - waypoint2.longitude) * sin(waypoint1.latitude) + sin(waypoint2.latitude) * cos(waypoint1.latitude));
	
	return depheading;
}

double waypoint_appheading(struct waypoint waypoint1, struct waypoint waypoint2){
	/* Accepts two waypoint structs. */
	/* Returns approach heading (from north) in radians. */
	
	double appheading = atan2(-sin(waypoint1.longitude - waypoint2.longitude) * cos(waypoint1.latitude), sin(waypoint2.latitude) * cos(waypoint1.longitude - waypoint2.longitude) * cos(waypoint1.latitude) - cos(waypoint2.latitude) * sin(waypoint1.latitude));
	
	return appheading;
}

int main(void){
	
	struct waypoint waypoint1, waypoint2;
	waypoint1.latitude = 0.71122167; waypoint1.longitude = -1.29096245; waypoint1.altitude = 0;
	waypoint2.latitude = 0.899425759; waypoint2.longitude = -0.00290946386; waypoint2.altitude = 0;
	
	double dist = waypoint_dist(waypoint1, waypoint2);
	double time, tas, theading;
	double depheading = waypoint_depheading(waypoint1, waypoint2);
	double appheading = waypoint_appheading(waypoint1, waypoint2);
	struct waypoint waypoint2_calc = waypoint_fromdist(waypoint1, dist, depheading);
	
	printf("The distance is %f meters.\n", dist);
	printf("The departure heading is %f degrees and the arrival heading is %f degrees.\n", depheading * 180.0 / M_PI, appheading * 180.0/M_PI);
	printf("Comparing to the original waypoint2, latitude %f and longitude %f,\nCalculating from distance and heading we get latitude %f and longitude %f.\n", waypoint2.latitude, waypoint2.longitude, waypoint2_calc.latitude, waypoint2_calc.longitude);
	
	return 0;
}
