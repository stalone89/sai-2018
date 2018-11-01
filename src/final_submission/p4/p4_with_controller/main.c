#include "lab1.h"
#include "file_operations.h"
#include "waypoints.h"
#include "simulator.h"
#include "controller.h"

int main (void){


	Waypoint waypoint_list[BUFFER];
	
	int i;
	int waypoint_count;
	double theodist = 0;
	/* double totaldist = 0; */
	double runtime = 0;
	double threshold;
	
	Coord waypoint_prev_coor, waypoint_next_coor;
	Coord position_current_sensor, position_current_true;
	
	double v_tas, v_m, v_tas_ref, theta_true, theta_sensor, heading, distance, csignal;

	double integral_acc = 0;
	double delay_memory = 0;

	FILE *fid_log = create_log("lab1_4_with_controller.log");
	FILE *fid_errors = create_log("lab1_4_with_controller_errors.log");


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

	waypoint_prev_coor = waypoint_to_coord(waypoint_list[0]);
	v_m = waypoint_list[0].tas;
	v_tas = waypoint_list[0].tas;

	for(i = 0; i < waypoint_count -1; i++){
		
		
		position_current_sensor = position_current_true;

		v_tas_ref = waypoint_list[i].tas;
		
		
		waypoint_next_coor = waypoint_to_coord(waypoint_list[i+1]);

		fprintf(fid_log, "=== From %s to %s ===\n", waypoint_list[i].location, waypoint_list[i+1].location);
		fprintf(fid_log, "%s has coordinates %f %f and altitude %fm, %s has coordinates %f %f and altitude %fm\n", waypoint_list[i].location, waypoint_list[i].latitude*(M_PI/180), waypoint_list[i].longitude*(M_PI/180), waypoint_list[i].altitude * FT2METER, waypoint_list[i+1].location, waypoint_list[i+1].latitude*(M_PI/180), waypoint_list[i+1].longitude*(M_PI/180), waypoint_list[i+1].altitude * FT2METER);
		fprintf(fid_log,"Time	Latitude(DR)	Longitude(DR)	Altitude(DR)	Latitude(True)	Longitude(True)	Altitude(True)	TAS		TAS(noisy)	True Heading	Theta_path\n");

		fprintf(fid_errors, "=== From %s to %s ===\n", waypoint_list[i].location, waypoint_list[i+1].location);
		fprintf(fid_errors, "%s has coordinates %f %f and altitude %fm, %s has coordinates %f %f and altitude %fm\n", waypoint_list[i].location, waypoint_list[i].latitude*(M_PI/180), waypoint_list[i].longitude*(M_PI/180), waypoint_list[i].altitude * FT2METER, waypoint_list[i+1].location, waypoint_list[i+1].latitude*(M_PI/180), waypoint_list[i+1].longitude*(M_PI/180), waypoint_list[i+1].altitude * FT2METER);

		update_log(fid_log, runtime, theta_sensor, v_tas, v_m, heading, position_current_sensor, position_current_true);	
		fprintf(fid_errors,"Position eror: %f\n", coord_dist(position_current_sensor, position_current_true));

		/*Define the distance threshold as the minimum spatial resolution for the simulation at reference VTAS*/
			threshold = TIMESPAN * v_tas;

		do{
			runtime += TIMESPAN;
			/*runtime_delta += TIMESPAN;*/

			distance = coord_dist(position_current_sensor, waypoint_next_coor);
			heading = depheading(position_current_sensor, waypoint_next_coor);

			csignal = speed_controller(v_m, v_tas_ref, &integral_acc);

			v_tas = plant_dynamics(csignal, v_m , &delay_memory);

			/*v_m = v_tas * (1+0.01*sin(2*M_PI*(runtime_delta/(20*60))));*/
			v_m = v_tas * (1+0.01*sin(2*M_PI*(runtime/(20*60))));

			theta_true = theta_path(position_current_true, waypoint_next_coor, v_tas);
			position_current_true = update_position(position_current_true, v_tas, TIMESPAN, theta_true, heading);

			theta_sensor = theta_path(position_current_sensor, waypoint_next_coor, v_m);
			position_current_sensor = update_position(position_current_sensor, v_m, TIMESPAN, theta_sensor, heading);

			

			update_log(fid_log, runtime, theta_sensor, v_tas, v_m, heading, position_current_sensor, position_current_true);
			fprintf(fid_errors,"Position eror: %f\n", coord_dist(position_current_sensor, position_current_true));

		} while (distance >= threshold);
		fprintf(fid_log, "\n");
		fprintf(fid_errors, "\nError at waypoint: %f\n\n\n", coord_dist(position_current_sensor, position_current_true));
	}

	printf("End runtime: %fs\n", runtime);
	
	fclose(fid_log);
	
	return 0;
}