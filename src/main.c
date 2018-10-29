#include "lab1.h"

int main(void){
	
	Coord waypoint_previouspos, waypoint_nextpos;
	
	Waypoint waypointlist[12];
	Coord subpointlist[20] = { 0 };
	
	double tas;
	double totaldist = 0;
	
	int i = 0;
	int j;
	
	Coord position_current_sensor, position_current_true;
	double heading, v_sensor, theta_true, theta_sensor, altituderate;
	int deltat = 10;
	
	read_file(waypointlist);
	getchar();
	
	printf("Started in city %s\n", waypointlist[i].location);
	for(i = 0; i < (sizeof(waypointlist) / sizeof(Waypoint) - 1); i++){
		waypoint_previouspos.latitude = waypointlist[i].latitude * M_PI/180;
		waypoint_previouspos.longitude = waypointlist[i].longitude * M_PI/180;
		waypoint_previouspos.altitude = waypointlist[i].altitude * FT2METER;
		waypoint_nextpos.latitude = waypointlist[i+1].latitude * M_PI/180;
		waypoint_nextpos.longitude = waypointlist[i+1].longitude * M_PI/180;
		waypoint_nextpos.altitude = waypointlist[i+1].altitude * FT2METER;
		tas = waypointlist[i].tas;
		
		totaldist += gen_subpoints(subpointlist, waypoint_previouspos, waypoint_nextpos, tas);
		printf("Calculated subpoints between waypoint %d in %s and waypoint %d in %s\n", i+1, waypointlist[i].location, i+2, waypointlist[i+1].location);
		
		position_current_true = waypoint_previouspos;
		position_current_sensor = waypoint_previouspos;
		
		for(j = 0; subpointlist[j].altitude != 0; j++){
			heading = depheading(position_current_true, subpointlist[j]);
			printf("Heading: %f Lat1: %f Lat2: %f\n", heading, position_current_true.longitude * 180/M_PI, subpointlist[j].longitude * 180/M_PI);
			
			v_sensor = 201;
			while(coord_dist(position_current_true, subpointlist[j]) > tas * deltat){
				altituderate = -ALTRATE_MOD * position_current_true.altitude + ALTRATE_MOD * subpointlist[j].altitude;
				theta_true = asin(altituderate / tas);
				position_current_true = iter(position_current_true, tas, deltat, theta_true, heading);
				position_current_true = coord_fromdist(position_current_true, tas * deltat, heading, 0);
				
				altituderate = -ALTRATE_MOD * position_current_sensor.altitude + ALTRATE_MOD * subpointlist[j].altitude;
				theta_sensor = asin(altituderate / v_sensor);
				position_current_sensor = iter(position_current_sensor, v_sensor, deltat, theta_sensor, heading);
				
				printf("Real position: %f %f %fm, Dead-reckoning position: %f %f %fm, Distance left: %f, Treshold: %f\n", position_current_true.latitude * 180/M_PI, position_current_true.longitude * 180/M_PI, position_current_true.altitude, position_current_sensor.latitude * 180/M_PI, position_current_sensor.longitude * 180/M_PI, position_current_sensor.altitude, coord_dist(position_current_true, subpointlist[j]), tas * deltat);
				getchar();
			}
			
			printf("Next subpoint\n");
		}
	}
	
	printf("Total distance run is %fm\n", totaldist);
	
	return 0;
}
