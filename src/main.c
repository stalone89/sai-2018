#include "lab1.h"

int main(void){
	
	Coord waypoint_prev_coor, waypoint_next_coor;
	
	Waypoint waypointlist[BUFFER];
	
	double totaldist = 0;
	
	int i = 0;
	int listsize;
	
	listsize = read_file(waypointlist);
	getchar();
	
	printf("Started in city %s\n", waypointlist[i].location);
	printf("The waypoint count is %d\n", listsize);
	for(i = 0; i < listsize; i++){
		waypoint_prev_coor = waypoint_to_coord(waypointlist[i]);
		waypoint_next_coor = waypoint_to_coord(waypointlist[i+1]);
		
		totaldist += coord_dist(waypoint_prev_coor, waypoint_next_coor);
	}
	
	printf("Total distance run is %fm\n", totaldist);
	
	
	
	return 0;
}
