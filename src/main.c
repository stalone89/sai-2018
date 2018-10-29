#include "lab1.h"

int main(void){
	
	Coord waypoint_previouspos, waypoint_nextpos;
	
	Waypoint waypointlist[12];
	Coord subpointlist[20] = { 0 };
	
	double tas;
	
	int i;
	
	read_file(waypointlist);
	getchar();
	
	for(i = 0; i < sizeof(waypointlist) / sizeof(Waypoint); i++){
		waypoint_previouspos.latitude = waypointlist[i].latitude * M_PI/180;
		waypoint_previouspos.longitude = waypointlist[i].longitude * M_PI/180;
		waypoint_previouspos.altitude = waypointlist[i].altitude;
		waypoint_nextpos.latitude = waypointlist[i+1].latitude * M_PI/180;
		waypoint_nextpos.longitude = waypointlist[i+1].longitude * M_PI/180;
		waypoint_nextpos.altitude = waypointlist[i+1].altitude;
		tas = waypointlist[i].tas;
		
		gen_subpoints(subpointlist, waypoint_previouspos, waypoint_nextpos, tas);
	}
	
	return 0;
}
