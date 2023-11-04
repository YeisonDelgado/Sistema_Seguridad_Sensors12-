#include "sensor.h"
#include <xc.h>

unsigned char control_sensor (void){
     unsigned char alarm_act=0;
     if(SENSOR_IR == 1){
        alarm_act=1;
    }
    
    if(SENSOR_HALL == 1){
        alarm_act=1;
    }
    
    if(SENSOR_METAL == 1){
        alarm_act=1;
    }
    return alarm_act; 
     
 }
