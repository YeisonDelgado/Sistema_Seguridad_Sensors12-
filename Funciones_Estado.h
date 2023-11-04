/*!
\file   Funciones_Estado.h
\date   2023-10-26
\author Estiven Delgado <yeisondelgado@unicauca.edu.co>
\author Alejandra Benavides <vbenavides@unicauca.edu.co>
\brief  Funciones generales de control.

\par Copyright
Information contained herein is proprietary to and constitutes valuable
confidential trade secrets of unicauca, and
is subject to restrictions on use and disclosure.

\par
Copyright (c) unicauca 2023. All rights reserved.

\par
The copyright notices above do not evidence any actual or
intended publication of this material.
**************************
*/

#ifndef FUNCIONES_ESTADO_H
#define FUNCIONES_ESTADO_H

#ifdef __cplusplus
extern "C" {
#endif

#define IN  1
#define OFF 0
#define LED_RED    RE0
#define LED_GREEN  RE1
#define LED_BLUE   RE2

enum State{
    security,
    monitoring,
    locked,
    alarm,
    environment,
    alarm_environment
};
enum State Estado;
unsigned char var_sensor_ir = 0;
unsigned char var_sensor_hall = 0;
unsigned char var_sensor_metal = 0;
unsigned char intend = 0;

const char password[5] ={'2','0','2','3','0'};
char pass_user[5];
unsigned char idx = 0;
unsigned char system_lock = 0;
char buffer [17];

void function_Security(void);
void function_Locked(void);
void function_Monitoring(void);
void function_Alarm(void);
void function_Environment(void);
void function_Environment_alarm(void);

#ifdef __cplusplus
}
#endif

#endif /* FUNCIONES_ESTADO_H */