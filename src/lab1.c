#include <stdio.h>
#include <math.h>

#define EARTH_RADIUS 6378000
#define M_PI 3.14159265358979323846

double waypoint_dist(double waypoint1[], double waypoint2[]){
	/* Accepts waypoints with format {lat, lon, alt} in radians, radians and meters, respectively. */
	/* Returns the distance between waypoints in meters. */
	
	double ortho = acos(cos(waypoint2[0]) * cos(waypoint1[1] - waypoint2[1]) * cos(waypoint1[0]) + sin(waypoint2[0]) * sin(waypoint1[0]));
	
	/* I'll keep these lines here just in case. */
	/* bearing = atan2(waypoint2[1] - waypoint1[1], sigma(waypoint2[0]) - sigma(waypoint1[0])); */
	/* dist = EARTH_RADIUS * fabs(waypoint2[0] - waypoint1[0]) * (1/cos(bearing)); */
	
	return ortho * EARTH_RADIUS;
}

double waypoint_depheading(double waypoint1[], double waypoint2[]){
	/* Accepts waypoints with format {lat, lon, alt} in radians, radians and meters, respectively. */
	/* Returns departure heading (from north) in radians. */
	
	double depheading = atan2(-cos(waypoint2[0]) * sin(waypoint1[1] - waypoint2[1]), -cos(waypoint2[0]) * cos(waypoint1[1] - waypoint2[1]) * sin(waypoint1[0]) + sin(waypoint2[0]) * cos(waypoint1[0]));
	
	return depheading;
}

double waypoint_appheading(double waypoint1[], double waypoint2[]){
	/* Accepts waypoints with format {lat, lon, alt} in radians, radians and meters, respectively. */
	/* Returns approach heading (from north) in radians. */
	
	double appheading = atan2(-sin(waypoint1[1] - waypoint2[1]) * cos(waypoint1[0]), sin(waypoint2[0]) * cos(waypoint1[1] - waypoint2[1]) * cos(waypoint1[0]) - cos(waypoint2[0]) * sin(waypoint1[0]));
	
	return appheading;
}

int main(void){
	
	double waypoint1[3] = {0.71122167,	-1.29096245,	0};
	double waypoint2[3] = {0.899425759,	-0.00290946386,	0};
	
	double dist = waypoint_dist(waypoint1, waypoint2);
	double depheading = waypoint_depheading(waypoint1, waypoint2);
	double appheading = waypoint_appheading(waypoint1, waypoint2);
	
	printf("The distance is %f meters.\n", dist);
	printf("The departure heading is %f degrees and the arrival heading is %f degrees.\n", depheading * 180.0 / M_PI, appheading * 180.0/M_PI);
	
	return 0;
}
