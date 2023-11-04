/*!
\file   Funciones_Estadoc.c
\date   2023-10-26
\author Estiven Delgado <yeisondelgado@unicauca.edu.co>
\author Alejandra Benavides <vbenavides@unicauca.edu.co>
\brief  Desarrollo de funciones para maquina de estados.

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
#include "Funciones_Estado.h"
#include "lcd.h"
#include "keypad.h"
#include <stdio.h>
#include <string.h>

/*F*************************
* NAME: function_Monitoring
*----------------------------------------------------------------------------
* PARAMS:   none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Monitoring of digital sensors and conditional decision making
*----------------------------------------------------------------------------
* NOTE:
*
**************************/
void function_Monitoring(void){
    var_sensor_ir = SENSOR_IR;
    var_sensor_hall = SENSOR_HALL;
    var_sensor_metal = SENSOR_METAL;
   
    if (Sensors_IHM() == 1){
        Estado = alarm;
        int n = sprintf(buffer, "IR %d, H %d, MT %d", var_sensor_ir, var_sensor_hall,var_sensor_metal);
        LCD_String_xy(0,0,buffer);
        __delay_ms(1000);
    }else{
        __delay_ms(2000);
        Estado = environment;  
    }
}

/*F*************************
* NAME: function_Security
*----------------------------------------------------------------------------
* PARAMS:   none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Main security system that receives an operating key
*----------------------------------------------------------------------------
* NOTE:
*
**************************/
void function_Security(void){
 
    char key = '0';
    LCD_Clear();
    LCD_String_xy(0,0,"Press a Key");
    LCD_Command(0xC0);
   
    do{
        key = keypad_getkey();
        if(key != 0){
            LCD_Char('*');// display pressed key on LCD16x2 */
            // LCD_Char(key);/* display pressed key on LCD16x2 */
            pass_user[idx++] = key;
        }
        
    }while(idx < 5);
    if(strncmp(pass_user,password,4) == 0){
        LCD_Clear();
        LCD_String_xy(0,0,"Clave Correcta");
        LED_GREEN = IN;
        __delay_ms(2000);
        LED_GREEN = OFF;
        Estado = monitoring;    
     
    }else{
        LCD_Clear();
        LCD_String_xy(0,0,"Clave Incorrecta");
        LED_BLUE = IN;
        __delay_ms(1000);
        LED_BLUE = OFF;
        intend++;
        idx = 0;
    }
    if(intend > 3){
        intend = 0;
        idx = 0;
        Estado = locked;
    }
}

/*F*************************
* NAME: function_Locked
*----------------------------------------------------------------------------
* PARAMS:   none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Locks the system for 10 seconds, maximum number of failed password attempts
*----------------------------------------------------------------------------
* NOTE:
*
**************************/          
void function_Locked(void){
    LCD_Clear();
    LCD_String_xy(0,0,"Block System");
    LED_RED = IN;
    __delay_ms(10000);
    LED_RED = OFF;
    LCD_Clear();
    Estado = security;    
}

/*F*************************
* NAME: function_Alarm
*----------------------------------------------------------------------------
* PARAMS:   none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Indicates the activation of at least one of the three digital sensors
*----------------------------------------------------------------------------
* NOTE:
*
**************************/
void function_Alarm(void){
    unsigned char time = 0;
    LCD_Clear();
    LCD_String_xy(0,0,"NO AUTORIZADO!!");
    while(time < 5){
        LED_RED = IN;
        __delay_ms(500);
        LED_RED = OFF;
        __delay_ms(500);
        time++;        
    }  
    Estado = monitoring;
}

/*F*************************
* NAME: function_Environment_alarm
*----------------------------------------------------------------------------
* PARAMS:   none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Conversion to display the temperature in degrees Celsius
*----------------------------------------------------------------------------
* NOTE:
*
**************************/
void function_Environment_alarm(void){
    LCD_Clear();
    LCD_String_xy(0,0,"Temperatura Amb");
    int value_potenciometro = Use_Chanel(0);
    int value_temperature = Use_Chanel(1);
    int temperature_celsius = Calculus_Temperature(value_temperature);
    int value_fotocelda = Use_Chanel(4);
    __delay_ms(500);
    int n = sprintf(buffer, "TS %d",temperature_celsius);
    LCD_String_xy(0,0,buffer);
    __delay_ms(2000);
   
    if(temperature_celsius > 30){
        Estado = alarm_environment;    
    }
    else{
        Estado = environment;
    }  
    LCD_Clear();
    __delay_ms(3000);
    Estado = monitoring;
}
 
/*F*************************
* NAME: function_Environment
*----------------------------------------------------------------------------
* PARAMS:   none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Displays a warning signal on temperature increase
*----------------------------------------------------------------------------
* NOTE:
*
**************************/
void function_Environment(void){
    unsigned char time2 = 0;
    LCD_Clear();
    LCD_String_xy(0,0,"Que Calor :(");
    while(time2 < 5){
        LED_BLUE = IN;
        __delay_ms(500);
        LED_BLUE = OFF;
        __delay_ms(500);
        time2++;
    }  
    Estado = monitoring;
}