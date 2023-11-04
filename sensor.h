/* 
 * File:   sensor.h
 * Author: estiv
 *
 * Created on 6 de octubre de 2023, 02:45 PM
 */

#ifndef SENSOR_H
#define	SENSOR_H

#ifdef	__cplusplus
extern "C" {
#endif

#define SENSOR_IR RC1
#define SENSOR_HALL RC0
#define SENSOR_METAL RC2

unsigned char control_sensor(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SENSOR_H */

