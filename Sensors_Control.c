/*!
 * @file   Sensors_Control.c
 * @date   2023-10-26
 * @author Estiven Delgado <yeisondelgado@unicauca.edu.co>
 * @author Alejandra Benavides <vbenavides@unicauca.edu.co>
 * @brief  Development of functions for sensors.
 *
 * @par Copyright
 * Information contained herein is proprietary to and constitutes valuable
 * confidential trade secrets of unicauca, and
 * is subject to restrictions on use and disclosure.
 *
 * @par
 * Copyright (c) unicauca 2023. All rights reserved.
 *
 * @par
 * The copyright notices above do not evidence any actual or
 * intended publication of this material.
 */

#include <xc.h>
#include "Sensors_Control.h"
#include "fuses.h"
#include <math.h>

#define beta 4090
#define resistance 10


/**
 * @brief ADC_Init.
 * 
 * Initialize the analog digital converter.
 * 
 * @param none
 * @return none
 */
void ADC_Init(void) {
    ADCON1bits.ADFM = 1;    // Right Justified (8-bit mode).
    ADCON1bits.VCFG0 = 0;   // Voltage Reference Selection (5V - 0V).
    ADCON1bits.VCFG1 = 0;   // Voltage Reference Selection (5V - 0V).
    ADCON0bits.ADCS = 0b01; // Conversion Time Fosc/8.
}


/**
 * @brief Use_Channel.
 * 
 * Select the channel to use an analog sensor.
 * 
 * @param select - The channel to select.
 * @return 8 bits digital temperature.
 */
int Use_Channel(int select){
    ADCON0bits.CHS = (0x0F & select);  // Select the Analog Channel AN4.
    ADCON0bits.ADON = 1;             // Enable the AD Module.
    __delay_us(30);
    ADCON0bits.GO = 1;               // Start AD Conversion.
    while (ADCON0bits.GO);           // Wait for AD Conversion to complete.
    ADCON0bits.ADON = 0;             // Disable the AD Module.

    // Calculate the digital conversion value.
    return ((ADRESH << 8) | ADRESL);
}


/**
 * @brief Calculus_Temperature.
 * 
 * Formula to convert the obtained temperature to degrees Celsius.
 * 
 * @param temperature - The temperature value to convert.
 * @return The temperature in degrees Celsius.
 */
int Calculus_Temperature(int temperature) {
    long a = 1023 - temperature;
    // Temperature conversion formula.
    float tempC = (float)(beta / (log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0);
    float tempF = (float)(tempC + 273.15);
    return (int)tempC;
}


/**
 * @brief Sensors_IHM.
 * 
 * Return 1 if any of the three sensors are activated.
 * 
 * @param none
 * @return 1 if any sensor is activated, 0 otherwise.
 */
unsigned char Sensors_IHM(void) {
    unsigned char alarm_activate = 0;

    if (SENSOR_IR == 0) {
        alarm_activate = 1;
    }

    if (SENSOR_HALL == 1) {
        alarm_activate = 1;
    }

    if (SENSOR_METAL == 1) {
        alarm_activate = 1;
    }

    return alarm_activate;
}