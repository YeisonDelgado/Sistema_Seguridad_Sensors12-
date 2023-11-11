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
* NAME: function_Environment
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
void function_Environment(void){
    LCD_Clear();
    LCD_String_xy(0,0,"Temperatura Amb");
    int value_potenciometro = Use_Chanel(1);
    int value_temperature = Use_Chanel(0);
    int temperature_celsius = Calculus_Temperature(value_temperature);
    int value_fotocelda = Use_Chanel(4);
    __delay_ms(500);
    LCD_Clear();
    int n = sprintf(buffer, "TS %d",temperature_celsius);
    LCD_String_xy(0,0,buffer);
    __delay_ms(2000);
   
    if(temperature_celsius > 25){
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
* NAME: function_Environment_alarm
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
void function_Environment_alarm(void){
    unsigned char time2 = 0;
    LCD_Clear();
    LCD_String_xy(0,0,"Temperatura > 25 °C");
    while(time2 < 5){
        LED_BLUE = IN;
        __delay_ms(500);
        LED_BLUE = OFF;
        __delay_ms(500);
        time2++;
    } 
    Estado = monitoring;
}



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
    ADCON0bits.CHS = (0x0f & select);  //  Selecciona el Canal Analógico AN4.
    ADCON0bits.ADON = 1;               //  Habilita el Módulo AD.
    __delay_us(30);
    ADCON0bits.GO = 1;                 //  Inicia la Conversió AD.
    while (ADCON0bits.GO);             //  Espera a que termine la conversión AD.
    ADCON0bits.ADON = 0;               //  Habilita el Módulo AD.
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
    TRISA = 0b00010011;  // Configura puertos requeridos como Entrada Analógica.
    ANSEL = 0b00010011;  
    
    ADC_Init();    //Inicializa el conversor
    LCD_Init();    //Inicializa el LCD
    keypad_init(); //Inicializa el keypad
   
    Estado = security;
    
    while (1) {
        // Bucle principal para el cambio de estados
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




/*!
\file   fuses.h
\date   2023-10-26
\author Estiven Delgado <yeisondelgado@unicauca.edu.co>
\author Alejandra Benavides <vbenavides@unicauca.edu.co>
\brief  Configuracion interna microcontrolador.

\par Copyright
Information contained herein is proprietary to and constitutes valuable
confidential trade secrets of unicauca, and
is subject to restrictions on use and disclosure.

\par
Copyright (c) unicauca 2023. All rights reserved.

\par
The copyright notices above do not evidence any actual or
intended publication of this material.
******************************************************************************
*/

#ifndef FUSES_H
#define	FUSES_H

#ifdef	__cplusplus
extern "C" {
#endif 
    
// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
    
#define _XTAL_FREQ 8000000       //Frecuencia del oscilador

#ifdef	__cplusplus
}
#endif

#endif	/* FUSES_H */



/*!
\file   keypad.c
\date   2023-10-6
\author Estiven Delgado <yeisondelgado@unicauca.edu.co>
\author Alejandra Benavides <vbenavides@unicauca.edu.co>
\brief  Handler Keypad 4x4.

\par Copyright
Information contained herein is proprietary to and constitutes valuable
confidential trade secrets of unicauca, and
is subject to restrictions on use and disclosure.

\par
Copyright (c) unicauca 2023. All rights reserved.

\par
The copyright notices above do not evidence any actual or
intended publication of this material.
******************************************************************************
*/

#include <xc.h>
#include "keypad.h"
#include "fuses.h"

/*F**************************************************************************
* NAME: keypad_init
*----------------------------------------------------------------------------
* PARAMS:   none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Initialize KEYPAD 4x4
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/

void keypad_init(void){
   
    PTECLADO &= ~(1<<PB0) & ~(1<<PB1) & ~(1<<PB2) & ~(1<<PB3);   // FILAS    - SALIDA
    PTECLADO |= (1<<PB4) | (1<<PB5) | (1<<PB6) | (1<<PB7);       // COLUMNAS - ENTRADA
    OPTION_REGbits.nRBPU = 0;   //   PORTB pull-ups are enabled by individual port latch values 
    ANSELH = 0;
}

/*F**************************************************************************
* NAME: keypad_getkey
*----------------------------------------------------------------------------
* PARAMS:   none
* return:   KEY Pressed in KEYPAD
*----------------------------------------------------------------------------
* PURPOSE:
* Get KEY
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
char keypad_getkey(void){

char letras[4][4]={  {'1','2','3','A'},
                     {'4','5','6','B'},
                     {'7','8','9','C'},
                     {'*','0','#','D'}};
int  i=0;
char valor=0;

for(i=0;i<4;i++){

   if(i==0){

      FILAS = (1<<COL4)|(1<<COL3)|(1<<COL2)|(1<<COL1)|(1<<ROW4)|(1<<ROW3)|(1<<ROW2)|(0<<ROW1);
      __delay_us(100);
      while (!(COLUMNAS & (1<<COL1))){valor=letras[0][0];}   //1
      while (!(COLUMNAS & (1<<COL2))){valor=letras[0][1];}   //2
      while (!(COLUMNAS & (1<<COL3))){valor=letras[0][2];}   //3
      while (!(COLUMNAS & (1<<COL4))){valor=letras[0][3];}   //A
   }

   if(i==1){
      FILAS = (1<<COL4)|(1<<COL3)|(1<<COL2)|(1<<COL1)|(1<<ROW4)|(1<<ROW3)|(0<<ROW2)|(1<<ROW1);
      __delay_us(100);
      while (!(COLUMNAS & (1<<COL1))){valor=letras[1][0];}   //4
      while (!(COLUMNAS & (1<<COL2))){valor=letras[1][1];}   //5
      while (!(COLUMNAS & (1<<COL3))){valor=letras[1][2];}   //6
      while (!(COLUMNAS & (1<<COL4))){valor=letras[1][3];}   //B
   }

   if(i==2){
      FILAS = (1<<COL4)|(1<<COL3)|(1<<COL2)|(1<<COL1)|(1<<ROW4)|(0<<ROW3)|(1<<ROW2)|(1<<ROW1);
      __delay_us(100);
      while (!(COLUMNAS & (1<<COL1))){valor=letras[2][0];}   //7
      while (!(COLUMNAS & (1<<COL2))){valor=letras[2][1];}   //8
      while (!(COLUMNAS & (1<<COL3))){valor=letras[2][2];}   //9
      while (!(COLUMNAS & (1<<COL4))){valor=letras[2][3];}  //C
   }

   if(i==3){
      FILAS = (1<<COL4)|(1<<COL3)|(1<<COL2)|(1<<COL1)|(0<<ROW4)|(1<<ROW3)|(1<<ROW2)|(1<<ROW1);
      __delay_us(100);
      while (!(COLUMNAS & (1<<COL1))){valor=letras[3][0];}   //*
      while (!(COLUMNAS & (1<<COL2))){valor=letras[3][1];}   //0
      while (!(COLUMNAS & (1<<COL3))){valor=letras[3][2];}   //+
      while (!(COLUMNAS & (1<<COL4))){valor=letras[3][3];}   //D
      }
   }

   return valor;
}


/*!
\file   keypad.h
\date   2023-10-26
\author Estiven Delgado <yeisondelgado@unicauca.edu.co>
\author Alejandra Benavides <vbenavides@unicauca.edu.co>
\brief  Functions Handler Keypad 4x4.

\par Copyright
Information contained herein is proprietary to and constitutes valuable
confidential trade secrets of unicauca, and
is subject to restrictions on use and disclosure.

\par
Copyright (c) unicauca 2023. All rights reserved.

\par
The copyright notices above do not evidence any actual or
intended publication of this material.
******************************************************************************
*/

#ifndef KEYPAD_H
#define	KEYPAD_H

#ifdef	__cplusplus
extern "C" {
#endif

#define PB7     7 
#define PB6     6
#define PB5     5
#define PB4     4
#define PB3     3
#define PB2     2
#define PB1     1
#define PB0     0

/* FILAS-SALIDAS */
#define ROW1   PB0
#define ROW2   PB1
#define ROW3   PB2
#define ROW4   PB3
/* COLUMNAS-ENTRADAS */
#define COL1   PB4
#define COL2   PB5
#define COL3   PB6
#define COL4   PB7

#define PTECLADO  TRISB
#define FILAS     PORTB
#define COLUMNAS  PORTB

void keypad_init (void);
char keypad_getkey(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* KEYPAD_H */




/*!
\file   lcd.c
\date   2023-09-27
\author Yeison Delgado <yeisondelgado@unicauca.edu.co>
\author Alejandra Benavides <vbenavides@unicauca.edu.co>
\brief  LCD 16x2 mode 4 bits.

\par Copyright
Information contained herein is proprietary to and constitutes valuable
confidential trade secrets of unicauca, and
is subject to restrictions on use and disclosure.

\par
Copyright (c) unicauca 2023. All rights reserved.

\par
The copyright notices above do not evidence any actual or
intended publication of this material.
******************************************************************************
*/

#include <xc.h>
#include "lcd.h"
#include "fuses.h"

/*F**************************************************************************
* NAME: LCD_Init
*----------------------------------------------------------------------------
* PARAMS:   none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Initialize LCD 16x2
*----------------------------------------------------------------------------
* NOTE:
* 
******************************************************************************/
void LCD_Init(void){
    LCD_Port = 0;         /*PORT as Output Port*/
    __delay_ms(15);       /* 15 ms, Power-On delay*/
    LCD_Command(0x02);    /*send for initialization of LCD with nibble method */
    LCD_Command(0x28);    /*use 2 line and initialize 5*7 matrix in (4-bit mode)*/
    LCD_Command(0x01);    /*clear display screen*/
    LCD_Command(0x0c);    /*display on cursor off*/
    LCD_Command(0x06);    /*increment cursor (shift cursor to right)*/	
}

/*F**************************************************************************
* NAME: LCD_Command
*----------------------------------------------------------------------------
* PARAMS:   
* cmd: Command to execute in LCD (example: 0x01 clear display screen, 0x0C display On, Cursor Off)
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Send Command to LCD 16x2
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void LCD_Command(unsigned char cmd )
{
	ldata = (ldata & 0x0f) |(0xF0 & cmd);  /*Send higher nibble of command first to PORT*/ 
	RS = 0;  /*Command Register is selected i.e.RS=0*/ 
	EN = 1;  /*High-to-low pulse on Enable pin to latch data*/ 
	NOP();
	EN = 0;
	__delay_ms(1);
    ldata = (unsigned char)((ldata & 0x0f) | (cmd<<4));  /*Send lower nibble of command to PORT */
	EN = 1;
	NOP();
	EN = 0;
	__delay_ms(3);
}

/*F**************************************************************************
* NAME: LCD_Char
*----------------------------------------------------------------------------
* PARAMS:   
* dat: character to display in LCD
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Display Character to LCD 16x2
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void LCD_Char(unsigned char dat)
{
	ldata = (ldata & 0x0f) | (0xF0 & dat);  /*Send higher nibble of data first to PORT*/
	RS = 1;  /*Data Register is selected*/
	EN = 1;  /*High-to-low pulse on Enable pin to latch data*/
	NOP();
	EN = 0;
	__delay_ms(1);
    ldata = (unsigned char)((ldata & 0x0f) | (dat<<4));  /*Send lower nibble of data to PORT*/
	EN = 1;  /*High-to-low pulse on Enable pin to latch data*/
	NOP();
	EN = 0;
	__delay_ms(3);
}

/*F**************************************************************************
* NAME: LCD_String
*----------------------------------------------------------------------------
* PARAMS:   none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Clear the Display Screen
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/

void LCD_String(const char *msg)
{
	while((*msg)!=0)
	{		
	  LCD_Char(*msg);
	  msg++;	
    }
}

/*F**************************************************************************
* NAME: LCD_String_xy
*----------------------------------------------------------------------------
* PARAMS:   none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Clear the Display Screen
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void LCD_String_xy(char row,char pos,const char *msg)
{
    char location=0;
    if(row<=1)
    {
        location=(0x80) | ((pos) & 0x0f);  /*Print message on 1st row and desired location*/
        LCD_Command(location);
    }
    else
    {
        location=(0xC0) | ((pos) & 0x0f);  /*Print message on 2nd row and desired location*/
        LCD_Command(location);    
    }  
    LCD_String(msg);
}

/*F**************************************************************************
* NAME: LCD_Clear
*----------------------------------------------------------------------------
* PARAMS:   none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Clear the Display Screen
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void LCD_Clear(void)
{
   	LCD_Command(0x01);
    __delay_ms(3);
}
