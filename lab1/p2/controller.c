/* controller.c */
#include "controller.h"


double speed_controller(double TAS_current, double VRef, double *integral_acc){
	double TAS_error;	
	double integral, proportional, csignal;
	
	integral=*integral_acc;
	TAS_error = VRef - TAS_current;
	
	proportional=TAS_error;
	integral=integral+TAS_error;

	csignal=kp*proportional + ki*integral;
	*integral_acc = integral;
	return csignal;
}


double plant_dynamics(double csignal, double TAS_current, double *delay_memory)
{
	
	double csignal_delayed;
	
	csignal_delayed=*delay_memory;
	*delay_memory=csignal;
	
	TAS_current=TAS_current+csignal_delayed;
	
	return TAS_current;
}