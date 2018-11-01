#include "lab1.h"
#include "file_operations.h"
#include "waypoints.h"
#include "simulator.h"

int main (void){


	Waypoint waypoint_list[BUFFER];
	
	int i, j;
	int waypoint_count;
	double subpoint_count;
	double theodist = 0;
	/* double totaldist = 0; */
	double runtime = 0;
	
	Coord waypoint_prev_coor, waypoint_next_coor;
	Coord position_current;
	
	double v_tas, theta, heading;

	FILE *fid_log = create_log("lab1_1.log");

	waypoint_count = read_file(waypoint_list);



	/*Print list of waypoints to the file and console*/

	fprintf(fid_log, "=== Waypoint List ===\n");
	for(i = 0; i < waypoint_count - 1; i++){
		fprintf(fid_log, "Latitude = %f, Longitude = %f, Altitude = %fm, TAS = %f, Location = %s\n", waypoint_list[i].latitude, waypoint_list[i].longitude, waypoint_list[i].altitude * FT2METER, waypoint_list[i].tas, waypoint_list[i].location);
	}
	fprintf(fid_log, "\n");

	printf( "=== Waypoint List ===\n");
	for(i = 0; i < waypoint_count - 1; i++){
		printf("Latitude = %f, Longitude = %f, Altitude = %fm, TAS = %f, Location = %s\n", waypoint_list[i].latitude, waypoint_list[i].longitude, waypoint_list[i].altitude * FT2METER, waypoint_list[i].tas, waypoint_list[i].location);
	}
	printf("\n");


	/*Wait for user input to proceed*/
	printf("\n\nPress any key to continue\n");
	getchar();


	/* Compute the total theoretical distance for the flight plan*/
	printf("Started in city %s\n", waypoint_list[0].location);
	printf("The waypoint count is %d\n", waypoint_count);
	for(i = 0; i < waypoint_count; i++){
		waypoint_prev_coor = waypoint_to_coord(waypoint_list[i]);
		waypoint_next_coor = waypoint_to_coord(waypoint_list[i+1]);
		
		theodist += coord_dist(waypoint_prev_coor, waypoint_next_coor);
	}
	
	printf("Total theoretical distance is %fm\n", theodist);


	for(i = 0; i < waypoint_count - 1; i++){
		v_tas = waypoint_list[i].tas;
		
		waypoint_prev_coor = waypoint_to_coord(waypoint_list[i]);
		waypoint_next_coor = waypoint_to_coord(waypoint_list[i+1]);
		
		position_current = waypoint_prev_coor;
		
		subpoint_count = coord_dist(waypoint_prev_coor, waypoint_next_coor) / (v_tas * TIMESPAN);
		fprintf(fid_log, "=== From %s to %s ===\n", waypoint_list[i].location, waypoint_list[i+1].location);
		fprintf(fid_log, "%s has coordinates %f %f and altitude %fm, %s has coordinates %f %f and altitude %fm\n", waypoint_list[i].location, waypoint_list[i].latitude, waypoint_list[i].longitude, waypoint_list[i].altitude * FT2METER, waypoint_list[i+1].location, waypoint_list[i+1].latitude, waypoint_list[i+1].longitude, waypoint_list[i+1].altitude * FT2METER);
		fprintf(fid_log, "Distance between waypoints: %fm, Distance each segment: %fm, Subpoints: %f\n", coord_dist(waypoint_prev_coor, waypoint_next_coor), v_tas * TIMESPAN, subpoint_count);
		fprintf(fid_log,"Time	Latitude(DR)	Longitude(DR)	Altitude(DR)	Latitude(True)	Longitude(True)	Altitude(True)	TAS		TAS(noisy)	True Heading	Theta_path 	Theta_path_true\n");

		update_log(fid_log, runtime, theta, theta, v_tas, v_tas, heading, position_current, position_current);
		for(j = 0; j < floor(subpoint_count); j++){
			
			theta = theta_path(position_current, waypoint_next_coor, v_tas);
			heading = depheading(position_current, waypoint_next_coor);
			
			/* position_current = iter(position_current, v_tas, TIMESPAN, theta, heading); */
			position_current = coord_fromdist(position_current, v_tas * TIMESPAN, heading, v_tas * sin(theta));
			/*fprintf(f, "Longitude = %f, Latitude = %f, Altitude = %fm, Distance left = %fm, Subpoint %d\n", position_current.latitude * 180/M_PI, position_current.longitude * 180/M_PI, position_current.altitude, coord_dist(position_current, waypoint_next_coor), j);*/
			/* totaldist += v_tas * TIMESPAN; */
			runtime += TIMESPAN;
			update_log(fid_log, runtime, theta, theta, v_tas, v_tas, heading, position_current, position_current);

		}
		if(j == floor(subpoint_count)){
			theta = theta_path(position_current, waypoint_next_coor, v_tas);
			heading = depheading(position_current, waypoint_next_coor);
			
			/* position_current = iter(position_current, v_tas, (coord_dist(position_current, waypoint_next_coor) / v_tas), theta, heading); */
			position_current = coord_fromdist(position_current, coord_dist(position_current, waypoint_next_coor), heading, v_tas * sin(theta));
			/*fprintf(f, "Longitude = %f, Latitude = %f, Altitude = %fm, Distance left = %fm, Subpoint %d\n", position_current.latitude * 180/M_PI, position_current.longitude * 180/M_PI, position_current.altitude, coord_dist(position_current, waypoint_next_coor), j);*/
			update_log(fid_log, runtime, theta, theta, v_tas, v_tas, heading, position_current, position_current);
			/* totaldist += coord_dist(position_current, waypoint_next_coor); */
			runtime += coord_dist(position_current, waypoint_next_coor) / v_tas;
		}
		
		fprintf(fid_log, "\n");
	}

	printf("End runtime: %fs\n", runtime);
	
	fclose(fid_log);
	
	return 0;
}