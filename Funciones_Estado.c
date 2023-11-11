/*!
 * @file   Funciones_Estado.c
 * @date   2023-10-26
 * @author Estiven Delgado <yeisondelgado@unicauca.edu.co>
 * @author Alejandra Benavides <vbenavides@unicauca.edu.co>
 * @brief  Development of functions for a state machine.
 *
 * Copyright (c) unicauca 2023. All rights reserved.
 * Information contained herein is proprietary to and constitutes valuable
 * confidential trade secrets of unicauca, and
 * is subject to restrictions on use and disclosure.
 */

#include <xc.h>
#include "Sensors_Control.h"
#include "fuses.h"
#include "Funciones_Estado.h"
#include "lcd.h"
#include "keypad.h"
#include <stdio.h>
#include <string.h>


/**
 * @brief function_Monitoring. 
 * 
 * Monitoring of digital sensors and conditional decision making.
 * 
 * @param none
 * @return none
 */
void function_Monitoring(void) {
    var_sensor_ir = SENSOR_IR;
    var_sensor_hall = SENSOR_HALL;
    var_sensor_metal = SENSOR_METAL;
   
    if (Sensors_IHM() == 1) {
        Estado = alarm;
        int n = sprintf(buffer, "IR %d, H %d, MT %d", var_sensor_ir, var_sensor_hall, var_sensor_metal);
        LCD_String_xy(0, 0, buffer);
        __delay_ms(1000);
    } else {
        __delay_ms(2000);
        Estado = environment;  
    }
}


/**
 * @brief function_Security. 
 * 
 * Main security system that receives an operating key.
 * 
 * @param none
 * @return none
 */
void function_Security(void) {
    char key = '0';
    LCD_Clear();
    LCD_String_xy(0, 0, "Press a Key");
    LCD_Command(0xC0);
   
    do {
        key = keypad_getkey();
        if (key != 0) {
            LCD_Char('*'); // display pressed key on LCD16x2 */
            // LCD_Char(key); /* display pressed key on LCD16x2 */
            pass_user[idx++] = key;
        }
    } while (idx < 5);

    if (strncmp(pass_user, password, 4) == 0) {
        LCD_Clear();
        LCD_String_xy(0, 0, "Clave Correcta");
        LED_GREEN = IN;
        __delay_ms(2000);
        LED_GREEN = OFF;
        Estado = monitoring;
    } else {
        LCD_Clear();
        LCD_String_xy(0, 0, "Clave Incorrecta");
        LED_BLUE = IN;
        __delay_ms(1000);
        LED_BLUE = OFF;
        intend++;
        idx = 0;
    }

    if (intend > 3) {
        intend = 0;
        idx = 0;
        Estado = locked;
    }
}


/**
 * @brief function_Lockedy. 
 * 
 * Locks the system for 10 seconds, maximum number of failed password attempts.
 * 
 * @param none
 * @return none
 */
void function_Locked(void) {
    LCD_Clear();
    LCD_String_xy(0, 0, "Block System");
    LED_RED = IN;
    __delay_ms(10000);
    LED_RED = OFF;
    LCD_Clear();
    Estado = security;
}


/**
 * @brief function_Alarm. 
 * 
 * Indicates the activation of at least one of the three digital sensors.
 * 
 * @param none
 * @return none
 */
void function_Alarm(void) {
    unsigned char time = 0;
    LCD_Clear();
    LCD_String_xy(0, 0, "NO AUTORIZADO!!");
    while (time < 5) {
        LED_RED = IN;
        __delay_ms(500);
        LED_RED = OFF;
        __delay_ms(500);
        time++;
    }

    Estado = monitoring;
}


/**
 * @brief function_Environment. 
 * 
 * Conversion to display the temperature in degrees Celsius.
 * 
 * @param none
 * @return none
 */
void function_Environment(void) {
    LCD_Clear();
    LCD_String_xy(0, 0, "Temperatura Amb");
    int value_potenciometro = Use_Channel(1);
    int value_temperature = Use_Channel(0);
    int temperature_celsius = Calculus_Temperature(value_temperature);
    int value_fotocelda = Use_Channel(4);
    __delay_ms(500);
    LCD_Clear();
    int n = sprintf(buffer, "TS %d", temperature_celsius);
    LCD_String_xy(0, 0, buffer);
    __delay_ms(2000);

    if (temperature_celsius > 25) {
        Estado = alarm_environment;
    } else {
        Estado = environment;
    }

    LCD_Clear();
    __delay_ms(3000);
    Estado = monitoring;
}


/**
 * @brief function_Environment_alarm.
 * 
 * 
 * Displays a warning signal on temperature increase.
 * 
 * @param none
 * @return none
 */
void function_Environment_alarm(void) {
    unsigned char time2 = 0;
    LCD_Clear();
    LCD_String_xy(0, 0, "Temperature > 25 C");
    while (time2 < 5) {
        LED_BLUE = IN;
        __delay_ms(500);
        LED_BLUE = OFF;
        __delay_ms(500);
        time2++;
    }

    Estado = monitoring;
}