/*!
\file   Main_Seguridad.c
\date   2023-10-26
\author Estiven Delgado <yeisondelgado@unicauca.edu.co>
\author Alejandra Benavides <vbenavides@unicauca.edu.co>
\brief  Security House.

\par Copyright
Information contained herein is proprietary to and constitutes valuable
confidential trade secrets of unicauca, and
is subject to restrictions on use and disclosure.

\par
Copyright (c) unicauca 2023. All rights reserved.

\par
The copyright notices above do not evidence any actual or
intended publication of this material.
****
*/

#include <xc.h>
#include <pic16f887.h>
#include "fuses.h"
#include "Sensors_Control.h"
#include "Funciones_Estado.h"
#include "lcd.h"
#include "keypad.h"

void main(){
    //Configura Fosc = 8Mhz interno, Fuente de Fosc del sistema = interno
    OSCCON = 0x71;       //Configura oscilador interno (FOSC = 8Mhz)
    TRISE = 0;
    PORTE = 0;
    TRISD = 0x00;        // salida puerto D
    TRISC = 0x07;        //Al PORTC no le asignamos valor, es entrada
    TRISA = 0b00010011;
    ANSEL = 0b00010011;  // Configura puertos requeridos como Entrada Analógica.
    
    ADC_Init();    //Inicializa el conversor
    LCD_Init();    //Inicializa el LCD
    keypad_init(); //Inicializa el keypad
   
    Estado = security;
    
    while (1) {
        if(Estado == security){
            function_Security();
        }
        else if(Estado == monitoring){
            function_Monitoring();                  
        }
        else if(Estado == locked){
            function_Locked();                  
        }
        else if(Estado == alarm){
            function_Alarm();
        }
        else if(Estado == alarm_environment){
            function_Environment_alarm();
        }
        else{
            function_Environment();
        }
    }
}