/*!
\file   Sensors_Control.c
\date   2023-10-26
\author Estiven Delgado <yeisondelgado@unicauca.edu.co>
\author Alejandra Benavides <vbenavides@unicauca.edu.co>
\brief  Desarrollo de funciones para los sensores.

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

#include <xc.h>
#include "Sensors_Control.h"
#include "fuses.h"
#include <math.h>

#define beta 4090
#define resistence 10

/*F*************************
* NAME: ADC_Init
*----------------------------------------------------------------------------
* PARAMS:   void
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Initialize the analog digital converter
*----------------------------------------------------------------------------
* NOTE:
*
**************************/
void ADC_Init(void){
    ADCON1bits.ADFM = 1;    //  Justificación Derecha (modo-8bits).
    ADCON1bits.VCFG0 = 0;   //  Selecciona Voltajes de Referencia (5v-0v).
    ADCON1bits.VCFG1 = 0;   //  Selecciona Voltajes de Referencia (5v-0v).
    ADCON0bits.ADCS = 0b01; //  Tiempo de Conversión Fosc/8.
}

/*F*************************
* NAME: Use_Chanel
*----------------------------------------------------------------------------
* PARAMS:   int select
* return:   8 bits digital temperature
*----------------------------------------------------------------------------
* PURPOSE:
* Select the channel to use an analog sensor
*----------------------------------------------------------------------------
* NOTE:
*
**************************/
int Use_Chanel(int select){
    ADCON0bits.CHS = (0x0f & select); //  Selecciona el Canal Analógico AN4.
    ADCON0bits.ADON = 1; //  Habilita el Módulo AD.
    __delay_us(30);
    ADCON0bits.GO = 1; //  Inicia la Conversió AD.
    while (ADCON0bits.GO); //  Espera a que termine la conversión AD.
    ADCON0bits.ADON = 0; //  Habilita el Módulo AD.
    // Operacion para retornar el valor de la conversion en Digital
    return ((ADRESH << 8) | ADRESL);
}

/*F*************************
* NAME: Calculus_Temperature
*----------------------------------------------------------------------------
* PARAMS:   int temperature
* return:   tempC
*----------------------------------------------------------------------------
* PURPOSE:
* Formula to convert the temperature obtained to degrees Celsius
*----------------------------------------------------------------------------
* NOTE:
*
**************************/
int Calculus_Temperature(int temperature){
    long a = 1023 - temperature;
    //Formula of temeperature for calculate value
    float tempC = (float)(beta/(log((1025.0 * 10 / a - 10) / 10) + beta/298.0) - 273.0);
    float tempF = (float)(tempC + 273.15);
    return (int)tempC;
}

/*F*************************
* NAME: Sensors_IHM
*----------------------------------------------------------------------------
* PARAMS:   void
* return:   alarm_activate
*----------------------------------------------------------------------------
* PURPOSE:
* Return 1 if any of the three sensors are activated
*----------------------------------------------------------------------------
* NOTE:
*
**************************/
unsigned char Sensors_IHM(void){
    unsigned char alarm_activate = 0;
    if(SENSOR_IR == 0){
        alarm_activate = 1;
    }
    if(SENSOR_HALL == 1){
        alarm_activate = 1;
    }
    if(SENSOR_METAL == 1){
        alarm_activate = 1;
    }
    return alarm_activate;  
 }