

#include <stdio.h>
#include <stdlib.h>
typedef struct
	{
	double latitude;
	double longitude;
	double altitude;
	}Waypoint;

int *readfile (filename)

{	
FILE *pFile;

struct Waypoint Waypoints[20];
int i;
int latg;
int latm;
int lats;
int longig;
int longim
int longis;
int alti;
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
		Waypoints[i].longitude = longig + (loglim/60) + (longis/3600);
		Waypoints[i].altitude = alti;  
		}
	return Waypoints;
	fclose(pFile);

	}	
}
