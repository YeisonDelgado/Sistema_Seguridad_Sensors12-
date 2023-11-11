/*!
 * @file   keypad.c
 * @date   2023-10-6
 * @author Estiven Delgado <yeisondelgado@unicauca.edu.co>
 * @author Alejandra Benavides <vbenavides@unicauca.edu.co>
 * @brief  Handler for 4x4 Keypad.
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
 *
 ******************************************************************************
 */

#include <xc.h>
#include "keypad.h"
#include "fuses.h"


/**
 * @brief keypad_init.
 * 
 * Initialize KEYPAD 4x4.
 * 
 * @param none
 * @return none
 */
void keypad_init(void){
   
    KEYPAD_PORT &= ~(1<<ROW1) & ~(1<<ROW2) & ~(1<<ROW3) & ~(1<<ROW4);   // ROWS    - OUTPUT
    KEYPAD_PORT |= (1<<COL1) | (1<<COL2) | (1<<COL3) | (1<<COL4);       // COLUMNS - INPUT
    OPTION_REGbits.nRBPU = 0;   //   PORTB pull-ups are enabled by individual port latch values 
    ANSELH = 0;
}


/**
 * @brief keypad_getkey.
 * 
 * Get KEY.
 * 
 * @param none
 * @return KEY Pressed in KEYPAD
 */
char keypad_getkey(void){

char letters[4][4]={  {'1','2','3','A'},
                     {'4','5','6','B'},
                     {'7','8','9','C'},
                     {'*','0','#','D'}};
int  i=0;
char value=0;

for(i=0;i<4;i++){

   if(i==0){

      ROWS = (1<<COL4)|(1<<COL3)|(1<<COL2)|(1<<COL1)|(1<<ROW4)|(1<<ROW3)|(1<<ROW2)|(0<<ROW1);
      __delay_us(100);
      while (!(COLUMNS & (1<<COL1))){value=letters[0][0];}   //1
      while (!(COLUMNS & (1<<COL2))){value=letters[0][1];}   //2
      while (!(COLUMNS & (1<<COL3))){value=letters[0][2];}   //3
      while (!(COLUMNS & (1<<COL4))){value=letters[0][3];}   //A
   }

   if(i==1){
      ROWS = (1<<COL4)|(1<<COL3)|(1<<COL2)|(1<<COL1)|(1<<ROW4)|(1<<ROW3)|(0<<ROW2)|(1<<ROW1);
      __delay_us(100);
      while (!(COLUMNS & (1<<COL1))){value=letters[1][0];}   //4
      while (!(COLUMNS & (1<<COL2))){value=letters[1][1];}   //5
      while (!(COLUMNS & (1<<COL3))){value=letters[1][2];}   //6
      while (!(COLUMNS & (1<<COL4))){value=letters[1][3];}   //B
   }

   if(i==2){
      ROWS = (1<<COL4)|(1<<COL3)|(1<<COL2)|(1<<COL1)|(1<<ROW4)|(0<<ROW3)|(1<<ROW2)|(1<<ROW1);
      __delay_us(100);
      while (!(COLUMNS & (1<<COL1))){value=letters[2][0];}   //7
      while (!(COLUMNS & (1<<COL2))){value=letters[2][1];}   //8
      while (!(COLUMNS & (1<<COL3))){value=letters[2][2];}   //9
      while (!(COLUMNS & (1<<COL4))){value=letters[2][3];}  //C
   }

   if(i==3){
      ROWS = (1<<COL4)|(1<<COL3)|(1<<COL2)|(1<<COL1)|(0<<ROW4)|(1<<ROW3)|(1<<ROW2)|(1<<ROW1);
      __delay_us(100);
      while (!(COLUMNS & (1<<COL1))){value=letters[3][0];}   //*
      while (!(COLUMNS & (1<<COL2))){value=letters[3][1];}   //0
      while (!(COLUMNS & (1<<COL3))){value=letters[3][2];}   //+
      while (!(COLUMNS & (1<<COL4))){value=letters[3][3];}   //D
      }
   }

   return value;
}
