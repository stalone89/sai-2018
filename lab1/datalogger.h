/* datalogger.h */
#ifndef _datalogger_h
#define _datalogger_h
#include <stdio.h>
#include "lab1.h"

#define filepath "log.txt"

FILE* create_log();
void update_log(FILE* fp, int time, float tetha_path, float VTAS, float Vm, float true_heading, Coord position, Coord position_true);


#endif
