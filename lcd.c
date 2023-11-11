/*!
 * @file   lcd.c
 * @date   2023-09-27
 * @brief  LCD 16x2 mode 4 bits.
 *
 * @details
 * This file contains the functions to interact with an LCD 16x2 in 4-bit mode.
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
 ******************************************************************************
 */

#include <xc.h>
#include "lcd.h"
#include "fuses.h"


/**
 * @brief Initialize the LCD.
 * 
 * This function initializes the LCD, configuring the necessary settings.
 * 
 * @param none
 * @return none
 */
void LCD_Init(void){
    LCD_Port = 0;         /*!< PORT as Output Port */
    __delay_ms(15);       /*!< 15 ms, Power-On delay */
    LCD_Command(0x02);    /*!< Send for initialization of LCD with nibble method */
    LCD_Command(0x28);    /*!< Use 2 lines and initialize 5x7 matrix in 4-bit mode */
    LCD_Command(0x01);    /*!< Clear display screen */
    LCD_Command(0x0c);    /*!< Display on, cursor off */
    LCD_Command(0x06);    /*!< Increment cursor (shift cursor to right) */
}


/**
 * @brief LCD_Command.
 * 
 * This function sends a command to the LCD, such as clearing the display screen
 * or turning the display on and off.
 * 
 * @param cmd Command to execute in LCD
 * @return none
 */
void LCD_Command(unsigned char cmd )
{
	ldata = (ldata & 0x0f) | (0xF0 & cmd);  /*!< Send higher nibble of command first to PORT */ 
	RS = 0;  /*!< Command Register is selected, i.e., RS=0 */ 
	EN = 1;  /*!< High-to-low pulse on Enable pin to latch data */ 
	NOP();
	EN = 0;
	__delay_ms(1);
    ldata = (unsigned char)((ldata & 0x0f) | (cmd<<4));  /*!< Send lower nibble of command to PORT */
	EN = 1;
	NOP();
	EN = 0;
	__delay_ms(3);
}


/**
 * @brief LCD_Char.
 * 
 * This function displays a character on the LCD.
 * 
 * @param dat Character to display on the LCD.
 * @return none
 */
void LCD_Char(unsigned char dat)
{
	ldata = (ldata & 0x0f) | (0xF0 & dat);  /*!< Send higher nibble of data first to PORT */
	RS = 1;  /*!< Data Register is selected */
	EN = 1;  /*!< High-to-low pulse on Enable pin to latch data */
	NOP();
	EN = 0;
	__delay_ms(1);
    ldata = (unsigned char)((ldata & 0x0f) | (dat<<4));  /*!< Send lower nibble of data to PORT */
	EN = 1;  /*!< High-to-low pulse on Enable pin to latch data */
	NOP();
	EN = 0;
	__delay_ms(3);
}


/**
 * @brief LCD_String.
 * 
 * This function displays a string on the LCD.
 * 
 * @param msg The string to display on the LCD.
 * @return none
 */
void LCD_String(const char *msg)
{
	while((*msg) != 0)
	{		
	  LCD_Char(*msg);
	  msg++;	
    }
}


/**
 * @brief LCD_String_xy.
 * 
 * Display a string on the LCD at a specific position.
 * 
 * @param row Row (1 or 2) where the message will be displayed.
 * @param pos Position on the row (0 to 15).
 * @param msg The string to display on the LCD.
 * @return none
 */
void LCD_String_xy(char row, char pos, const char *msg)
{
    char location = 0;
    if (row <= 1)
    {
        location = (0x80) | ((pos) & 0x0f);  /*!< Print message on the 1st row at the desired location */
        LCD_Command(location);
    }
    else
    {
        location = (0xC0) | ((pos) & 0x0f);  /*!< Print message on the 2nd row at the desired location */
        LCD_Command(location);    
    }  
    LCD_String(msg);
}


/**
 * @brief LCD_Clear.
 * 
 * This function clears the display screen of the LCD.
 * 
 * @param none
 * @return none
 */
void LCD_Clear(void)
{
   	LCD_Command(0x01);
    __delay_ms(3);
}
