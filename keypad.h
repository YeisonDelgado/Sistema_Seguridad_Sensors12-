/*!
 * @file   keypad.h
 * @date   2023-10-26
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

/* ROWS - OUTPUTS */
#define ROW1   PB0
#define ROW2   PB1
#define ROW3   PB2
#define ROW4   PB3
/* COLUMNS - INPUTS */
#define COL1   PB4
#define COL2   PB5
#define COL3   PB6
#define COL4   PB7

#define KEYPAD_PORT  TRISB
#define ROWS     PORTB
#define COLUMNS  PORTB

void keypad_init(void);
char keypad_getkey(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* KEYPAD_H */
