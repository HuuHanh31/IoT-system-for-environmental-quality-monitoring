#ifndef	_MAIN_H_
#define _MAIN_H_

#include <p18f4620.h>
#include "timer\timer.h"
#include "interrupt\interrupt.h"
#include "button_matrix\button.h"
#include "lcd\lcd.h"
#include "uart\uart.h"
#include "adc\adc.h"
#include "i2c/i2c.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#pragma config OSC		= HS
#pragma config FCMEN            = OFF
#pragma config IESO		= OFF
#pragma config PWRT		= OFF
#pragma config BOREN            = ON
#pragma config BORV		= 0
#pragma config WDT		= OFF
#pragma config MCLRE            = ON
#pragma config LPT1OSC          = OFF
#pragma config PBADEN           = OFF
#pragma config CCP2MX           = PORTC
#pragma config STVREN           = ON
#pragma config LVP		= OFF
#pragma config XINST            = OFF
#pragma config DEBUG            = OFF
#pragma config CP0 		= OFF
#pragma config CP1 		= OFF
#pragma config CP2 		= OFF
#pragma config CP3 		= OFF
#pragma config CPB 		= OFF
#pragma config CPD 		= OFF
#pragma config WRT0		= OFF
#pragma config WRT1		= OFF
#pragma config WRT2		= OFF
#pragma config WRT3		= OFF
#pragma config WRTB		= OFF
#pragma config WRTC		= OFF
#pragma config WRTD		= OFF
#pragma config EBTR0            = OFF
#pragma config EBTR1            = OFF
#pragma config EBTR2            = OFF
#pragma config EBTR3            = OFF
#pragma config EBTRB            = OFF

#endif