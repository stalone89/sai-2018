#include "file_operations.h"


int read_file(Waypoint* waypointlist){
	/* Accepts a Waypoint array to fill with waypoints from a file.
	 * Returns -1 in case of file opening failure, and the line count otherwise. */
	
	char* filename = "waypoints_dms.csv";
	char line[100];
	int i = 0;
	
	FILE* fid;
	
	if ((fid = fopen(filename,"r")) == NULL){
		printf("Error opening waypoints.csv file.\n");
		return -1;
	}
	
	fgets(line,100,fid); /* Skip first line */
	
	while(!feof(fid)){
		line[0] = '\0';
		fgets(line,100,fid);
		
		if(strlen(line) > 30){
			waypointlist[i] = csv_waypoint_parse_dms(line);
			printf("Latitude = %f, Longitude = %f, Altitude = %fm, TAS = %f, Location = %s\n", waypointlist[i].latitude, waypointlist[i].longitude, waypointlist[i].altitude * FT2METER, waypointlist[i].tas, waypointlist[i].location);
			i++;
		}
	}
	
	fclose(fid);
	
	return i;
}

Waypoint csv_waypoint_parse(char line[]){
	/* Accepts a string formatted according to a line from the provided CSV file */
	/* Returns the line's corresponding waypoint in a Waypoint struct */
	
	int i = 0;
	char* field;
	Waypoint waypoint;
	
	field = strtok(line, ",");
	
	while(field != NULL){
		switch(i){
			case 0:
				strcpy(waypoint.location, field);
			case 1:
				sscanf(field, "%lf", &waypoint.latitude);
			case 2:
				sscanf(field, "%lf", &waypoint.longitude);
			case 3:
				sscanf(field, "%lf", &waypoint.altitude);
			case 4:
				sscanf(field, "%lf", &waypoint.tas);
		}
		
		field = strtok(NULL, ",");
		i++;
	}
	
	return waypoint;
}

FILE* create_log(char filepath[]){
	
	FILE* fp;
	
	fp = fopen(filepath,"w+");
	if (fp==NULL){
		printf("Erro na escrita do ficheiro");}
	else {
		fprintf(fp,"----- Data Log for FMS -----\n");		
	}
	
	return fp;
}
	
void update_log(FILE* fp, int time, float theta_path, float theta_path_true, float VTAS, float Vm, float true_heading, Coord position, Coord position_true){
	
	fprintf(fp, "%u\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", time,position.latitude, position.longitude, position.altitude, position_true.latitude, position_true.longitude, position_true.altitude, VTAS, Vm, true_heading, theta_path, theta_path_true);
	
	return;
	
}

Waypoint csv_waypoint_parse_dms(char line[]){
	/* Accepts a string formatted according to a line from the provided CSV file */
	/* Returns the line's corresponding waypoint in a Waypoint struct */
	
	int i = 0;
	char* field;
	Waypoint waypoint;
	double degrees, minutes, seconds;
	
	field = strtok(line, ",");
	
	while(field != NULL){
		switch(i){
			case 0:
				strcpy(waypoint.location, field);
			case 1:
				sscanf(field, "%lf°%lf'%lf''", &degrees, &minutes, &seconds);
				waypoint.latitude = degrees + minutes/60 + seconds/3600;
			case 2:
				if(strcmp(field, "S") == 0){
					waypoint.latitude = -waypoint.latitude;
				}
			case 3:
				sscanf(field, "%lf°%lf'%lf''", &degrees, &minutes, &seconds);
				waypoint.longitude = degrees + minutes/60 + seconds/3600;
			case 4:
				if(strcmp(field, "W") == 0){
					waypoint.longitude = -waypoint.longitude;
				}
			case 5:
				sscanf(field, "%lf", &waypoint.altitude);
			case 6:
				sscanf(field, "%lf", &waypoint.tas);
		}
		
		field = strtok(NULL, ",");
		i++;
	}
	
	return waypoint;
}