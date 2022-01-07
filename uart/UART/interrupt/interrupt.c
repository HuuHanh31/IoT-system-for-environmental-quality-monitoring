#include "interrupt.h"
#include "..\timer\timer.h"
#include "..\uart\uart.h"


unsigned int cnt0 = 0,cnt1 = 0;

void init_interrupt(void)
{
	INTCONbits.GIE = 1;  //enable global interrupt
	RCONbits.IPEN = 1;   //enable interrupt priority
	INTCONbits.PEIE = 1; //Peripheral External Interupt Enable
}

void init_ext_int0(void)
{
	INTCON2bits.INTEDG0 = 0; //Interrupt0 on falling edge
	INTCONbits.INT0IF = 0; //Clear Interrupt Flag bit
	INTCONbits.INT0IE = 1;	// Enable INT0
}



#pragma code InterruptVectorLow		= 0x18
void InterruptVectorLow(void)
{
	_asm
		GOTO	low_isr
	_endasm
}

#pragma code InterruptVectorHigh 	= 0x08
void InterruptVectorHigh(void)
{
	_asm
		GOTO	high_isr
	_endasm
}

#pragma code
#pragma interruptlow	low_isr
void low_isr(void)
{
	if (INTCONbits.T0IF) // timer0
	{
		stop_timer0();
		INTCONbits.T0IF = 0;
		start_timer0();
		timer0_isr();
	}

	if (PIR1bits.TMR1IF) // timer1
	{
		stop_timer1();
		PIR1bits.TMR1IF = 0;
		start_timer1();	
		timer1_isr();
	}

	

	if (PIR2bits.TMR3IF) // timer3
	{
		stop_timer3();
		PIR2bits.TMR3IF = 0;
		start_timer3();
		timer3_isr();
	}

	if (INTCONbits.INT0IF) // ext_interupt0
	{
	
        INTCONbits.INT0IF = 0;
		//add code here
//           if(RCREG == 'a'){
//           PORTBbits.RB0 = 0;
//        }
//        if(RCREG == 'b' ){
//            PORTBbits.RB0 = 1;
//        }
        
	}

        if(PIR1bits.RCIF == 1) // UART interupt Receive
	{
		PIR1bits.RCIF = 0;
                uart_isr();

       
	}
}

#pragma code
#pragma interrupt		high_isr
void high_isr(void)
{
	if (INTCONbits.T0IF) // timer0
	{
		stop_timer0();
		INTCONbits.T0IF = 0;
		start_timer0();
		timer0_isr();
	}

	if (PIR1bits.TMR1IF) // timer1
	{
		stop_timer1();
		PIR1bits.TMR1IF = 0;
		start_timer1();	
		timer1_isr();
	}

	

	if (PIR2bits.TMR3IF) // timer3
	{
		stop_timer3();
		PIR2bits.TMR3IF = 0;
		start_timer3();
		timer3_isr();
	}

	if (INTCONbits.INT0IF) // ext_interupt0
	{
		INTCONbits.INT0IF = 0;
		//add code here
	}

        if(PIR1bits.RCIF == 1) // UART interupt Receive
	{
		PIR1bits.RCIF = 0;
                uart_isr();
	}

        //if(PIR1bits.TXIF == 1) // UART interupt Receive
}

