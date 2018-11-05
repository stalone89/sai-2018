/* datalogger.c */
#include "datalogger.h"

FILE* create_log(){
	
	FILE* fp;
	
	fp = fopen(filepath,"w+");
	if (fp==NULL){
		printf("Erro na escrita do ficheiro");}
	else {
		fprintf(fp,"----- Data Log for FMS -----\n");
		fprintf(fp,"Time	Latitude	Longitude	Altitude	Latitude(True)	Longitude(True)	Altitude(True)	TAS		TAS(noisy)	True Heading	Theta_path\n");
	}
	
	return fp;
}
	
void update_log(FILE* fp, int time, float theta_path, float VTAS, float Vm, float true_heading, Coord position, Coord position_true){
	
	fprintf(fp, "%u\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", time,position.latitude, position.longitude, position.altitude, position_true.latitude, position_true.longitude, position_true.altitude, VTAS, Vm, true_heading, theta_path);
	
	return;
	
}
