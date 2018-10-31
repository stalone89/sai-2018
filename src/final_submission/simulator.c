#include "simulator.h"

Coord iter(Coord pos1, double v, double deltat, double theta, double heading){
	
	Coord pos2;
	
	double v_east, v_north, v_up;
	
	v_east = v * cos(theta) * sin(heading);
	v_north = v * cos(theta) * cos(heading);
	v_up = v * sin(theta);
	
	pos2.latitude = pos1.latitude + ((v_north * deltat) / (pos1.altitude + EARTH_RADIUS));
	pos2.longitude = pos1.longitude + ((v_east * deltat) / ((pos1.altitude + EARTH_RADIUS)*cos(pos1.latitude)));
	pos2.altitude = pos1.altitude + v_up * deltat;
	
	return pos2;
}

double theta_path(Coord coor1, Coord coor2, double v_tas){
	/* Accepts two coordinates and returns the climb angle from the first to
	 * the second based on the altitude rate equation. */
	double altituderate = -ALTRATE_MOD * coor1.altitude + ALTRATE_MOD * coor2.altitude;
	return asin(altituderate / v_tas);
}


Coord update_position(Coord pos1, double v, double deltat, double theta, double heading){
	
	Coord pos2;
	double v_east, v_north, v_up, dlat, dNorth, dEast, dUp, dlon, lat_new, lon_new, alt_new, R;
	
	v_east = v * cos(theta) * sin(heading);
	v_north = v * cos(theta) * cos(heading);
	v_up = v * sin(theta);
	
	dNorth=v_north*TIMESPAN;
	dEast=v_east*TIMESPAN;
	dUp=v_up*TIMESPAN;
	
	R = EARTH_RADIUS + pos1.altitude;
	
	dlat=dNorth/R;
	dlon=dEast/(R*cos(pos1.latitude));
	
	lat_new=pos1.latitude + dlat;
	lon_new=pos1.longitude + dlon;
	alt_new=pos1.altitude + dUp;
	
	pos2.latitude=lat_new;
	pos2.longitude=lon_new;
	pos2.altitude=alt_new;
	
	return pos2;
}
