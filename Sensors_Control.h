/*!
 * @file   Sensor_Control.h
 * @date   2023-10-26
 * @author Estiven Delgado <yeisondelgado@unicauca.edu.co>
 * @author Alejandra Benavides <vbenavides@unicauca.edu.co>
 * @brief  Sensors analogic and digital.
 *
 * Copyright (c) unicauca 2023. All rights reserved.
 * Information contained herein is proprietary to and constitutes valuable
 * confidential trade secrets of unicauca, and
 * is subject to restrictions on use and disclosure.
 */

#ifndef SENSORS_CONTROL_H
#define SENSORS_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

#define SENSOR_IR    RC1
#define SENSOR_HALL  RC0
#define SENSOR_METAL RC2

void ADC_Init(void);
int Use_Channel(int select);
int Calculus_Temperature(int temperature);
unsigned char Sensors_IHM(void);    

#ifdef __cplusplus
}
#endif

#endif /* SENSORS__CONTROL */