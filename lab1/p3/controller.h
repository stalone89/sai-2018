/* controller.h */
#ifndef _controller_h
#define _controller_h

#include <math.h>

#define kp 0.5
#define ki 0.005
#define delay 1
#define alpha 1

double speed_controller(double TAS_current, double VRef, double *integral_acc);
double plant_dynamics(double csignal, double TAS_current, double *delay_memory);
double altitude_controller(double h_current, double hRef);

#endif
