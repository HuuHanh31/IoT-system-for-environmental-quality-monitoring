
#include "uart/uart.h"

#include "main.h"
// Noi khai bao hang so
#define     LED     PORTD
#define     ON      1
#define     OFF     0

// Noi khai bao bien toan cuc
unsigned char arrayMapOfOutput [8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char statusOutput[8] = {0,0,0,0,0,0,0,0};
// Khai bao cac ham co ban IO
void init_system(void);
void delay_ms(int value);
void OpenOutput(int index);
void CloseOutput(int index);
void TestOutput(void);
void ReverseOutput(int index);
void SetupForFirstProgram(void);

void BaiTap_UART();
void SimulateFull_COCA(void);
void senddataUart(void);
unsigned int simulateCnt;
unsigned int simulateValue;
unsigned int pH_value;
unsigned int SS_value;
unsigned int COD_value;
unsigned int NH4_value;
unsigned int NO3_value;
unsigned int TMP_value;
unsigned int FLOW_value;
unsigned int check;
#define ADDRESS_SECOND  0x00
#define ADDRESS_MINUTE  0x01
#define ADDRESS_HOUR    0x02
#define ADDRESS_DATE    0x04
#define ADDRESS_MONTH   0x05
#define ADDRESS_YEAR    0x06
#define ADDRESS_FIRST_PROGRAM 0x20


void SetupForFirstProgram(void);
char second = 0, minute = 0, hour = 0;
char date = 0, month = 0, year = 0;
void SetupTimeForRealTime();
void SendTime(void);
unsigned int adcValue = 0;
unsigned char a ;

void main(void)
{
	unsigned int k = 0;
    int i = 0;
	init_system();
    lcd_clear();
    LcdClearS();
    delay_ms(1);
    SetupForFirstProgram();

	while (1)
	{
        
        get_adc_value();
        while (!flag_timer3);
        flag_timer3 = 0;
        i++;
        
//        LcdPrintNumS(0,0,PORTBbits.RB4);
//        uart_isr();
 //       UartSendNum(PORTBbits.RB4);
        
        
//        adcValue = ((adc_value[0]/10.24));
//       //adcValue = adc_value[0];
//        UartSendNum(adcValue);
//        if(adcValue > 50)
//            PORTBbits.RB0 = 0 ;
//        if(adcValue < 40)
           // PORTBbits.RB0 = 0 ;
//        UartSendString("assd");
//        UartSendString("uart receive: ");
//        UartSendString(RCREG);
//        LcdPrintString(0,0,RCREG);
//     a = USART_RxChar();
//    LcdPrintString(0,0,a);
//        UartSendString("a");
        
        uart_isr();
        adcValue = ((adc_value[0]/10.24));
        LcdPrintStringS(0,0,"CO");
        LcdPrintNumPercentS(1,0,adcValue);
     
//        
        adcValue = (adc_value[1]/10.24);
        LcdPrintStringS(0,3,"SS");
        LcdPrintNumPercentS(1,3,adcValue);
//        
        adcValue = (adc_value[2]/10.24);
        LcdPrintStringS(0,6,"NH");
        LcdPrintNumPercentS(1,6,adcValue);
//        
        adcValue = ((adc_value[3]/10.24));
        LcdPrintStringS(0,9,"NO");
        LcdPrintNumPercentS(1,9,adcValue);
//        
        adcValue = (adc_value[4]/10.24);
        LcdPrintStringS(0,12,"pH");
        LcdPrintNumPercentS(1,12,adcValue);
        
//        if((adc_value[0]/10.24) > 80)
//                PORTBbits.RB0 = 0 ;
//                
//        if((adc_value[0]/10.24) <70 )
//                PORTBbits.RB0 = 1 ;
//        if((adc_value[1]/10.24) > 80)
//                PORTBbits.RB1 = 0 ;
//        if((adc_value[1]/10.24) <70 )
//                PORTBbits.RB1 = 1 ;
        
        //PORTBbits.RB0 = 0 ;
        //__delay_ms(500);
        // PORTBbits.RB0 = 0;
        // __delay_ms(500);
        if(i == 800){
            senddataUart();
//            SimulateFull_COCA();
            i = 0;
        }
         DisplayLcdScreen();
    }
}

void delay_ms(int value)
{
	int i,j;
	for(i=0;i<value;i++)
		for(j=0;j<238;j++);
}

void init_system(void)
{
    TRISB = 0x00;		//setup PORTB is output
    TRISA = 0x01;
    init_lcd();
    LED = 0x00;
    init_interrupt();
    delay_ms(1000);
    init_timer0(4695);//dinh thoi 1ms sai so 1%
    init_timer1(9390);//dinh thoi 2ms
    init_timer3(46950);//dinh thoi 10ms
    SetTimer0_ms(2);
    SetTimer1_ms(10);
    SetTimer3_ms(50); //Chu ky thuc hien viec xu ly input,proccess,output
    init_key_matrix_with_uart();
    init_uart();
    init_adc();
    init_i2c();
    
//    SetupTimeForRealTime();
}

void OpenOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		LED = LED | arrayMapOfOutput[index];
	}

}

void CloseOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		LED = LED & ~arrayMapOfOutput[index];
	}
}

void ReverseOutput(int index)
{
    if (statusOutput[index]  == ON)
    {
        CloseOutput(index);
        statusOutput[index] = OFF;
    }
    else
    {
        OpenOutput(index);
        statusOutput[index] = ON;
    }
}

void TestOutput(void)
{
	int k;
	for (k=0;k<=7 ;k++ )
	{
		OpenOutput(k);
		delay_ms(500);
		CloseOutput(k);
		delay_ms(500);
	}
}

void BaiTap_UART()
{
    unsigned long pH_value,adc_value;
    adc_value = get_adc_value();
    pH_value = (adc_value * 140)/1023;
    uart_putchar(0x0a);
    uart_putchar(0x0d);
    LcdPrintStringS(0,0,"                ");
    if (pH_value<100)
    {
        LcdPrintNumS(0,10,pH_value/10);
        uart_putchar(pH_value/10 + 0x30);
        LcdPrintStringS(0,11,".");
        uart_putchar('.');
        LcdPrintNumS(0,12,pH_value%10);
        uart_putchar(pH_value%10 + 0x30);
        
    }
    else
    {
        LcdPrintNumS(0,10,pH_value/10);
        uart_putchar(pH_value/100 + '0');
        uart_putchar(pH_value/10%10 + '0');
        LcdPrintStringS(0,12,".");
        uart_putchar('.');
        LcdPrintNumS(0,13,pH_value%10);
        uart_putchar(pH_value%10 + '0');
    }
}
void senddataUart(void){
    UartSendString("!!");
    UartSendString("A0:");
    if((adc_value[0]/10.24)>= 10)
    UartSendNum((adc_value[0]/10.24));
    else{
    UartSendString("0");
    UartSendNum((adc_value[0]/10.24));
    }
    UartSendString(";");
    UartSendString("A1:");
    if((adc_value[1]/10.24)>= 10)
    UartSendNum((adc_value[1]/10.24));
    else{
    UartSendString("0");
    UartSendNum((adc_value[1]/10.24));
    }
    UartSendString(";");
    UartSendString("A2:");
     if((adc_value[2]/10.24)>= 10)
    UartSendNum((adc_value[2]/10.24));
    else{
    UartSendString("0");
    UartSendNum((adc_value[2]/10.24));
    }
    UartSendString(";");
    UartSendString("A3:");
    if((adc_value[3]/10.24)>= 10)
    UartSendNum((adc_value[3]/10.24));
    else{
    UartSendString("0");
    UartSendNum((adc_value[3]/10.24));
    }
    UartSendString(";");
    UartSendString("A4:");
     if((adc_value[4]/10.24)>= 10)
    UartSendNum((adc_value[4]/10.24));
    else{
    UartSendString("0");
    UartSendNum((adc_value[4]/10.24));
    }
     UartSendString(";");
    UartSendString("RL1:");
    UartSendNum( !PORTBbits.RB0);
     UartSendString(";");
    UartSendString("RL2:");
    UartSendNum( !PORTBbits.RB1);
    UartSendString("##");
}
void SimulateFull_COCA(void)
{
    int adcValue;
    simulateCnt = (simulateCnt + 1)%40;
    if (simulateCnt == 0)
    {
        adcValue = (get_adc_value()/10.23)*100;
//        adcValue = 5;
        simulateValue = (simulateValue + 1) % 100;
        //temp = yMin + (long)(adcValue - xMin) * (yMax - yMin) / (xMax - xMin);
        pH_value = 0 + (long)(adcValue - 0) * (1400 - 0) / (1023 - 0);
        SS_value = 0 + (long)(adcValue - 0) * (5000 - 0) / (1023 - 0);
        COD_value = 0 + (long)(adcValue - 0) * (500 - 0) / (1023 - 0);
        NH4_value = 0 + (long)(adcValue - 0) * (1000 - 0) / (1023 - 0);
        NO3_value = 0 + (long)(adcValue - 0) * (2000 - 0) / (1023 - 0);
        TMP_value = 2000 + (long)(adcValue - 0) * (4000 - 2000) / (1023 - 0);
        FLOW_value = 0 + (long)(adcValue - 0) * (10000 - 0) / (1023 - 0);
//        LcdPrintStringS(0,0,"ADC 1 =  ");
//        LcdPrintNumPercentS(0,8, adcValue);
//        LcdPrintStringS(1,0,"ADC 2 = ");
//        LcdPrintNumPercentS(1,8, 400 + simulateValue);

//        UartSendString("20.04.16 09:12:07  pH=  ");
//        SendTime();
        UartSendString("!!");
        UartSendNumPercent(adcValue);
        UartSendString("##");
//        UartSendNumPercent(SS_value);
//        UartSendString(" mg/l CSB= ");
//        UartSendNumPercent(COD_value);
//        UartSendString(" mg/l TMP= ");	
//        UartSendNumPercent(TMP_value);
//        UartSendString(" øC NH4= ");	
//        UartSendNumPercent(NH4_value);
//        UartSendString(" mg/l NO3= ");	
//        UartSendNumPercent(NO3_value);
//        UartSendString(" mg/l FLOW= ");	
//        UartSendNumPercent(FLOW_value);
//        UartSendString(" m3/h \r\n");	
    }
}

void SendTime(void)
{
    second = read_ds1307(ADDRESS_SECOND);
    minute = read_ds1307(ADDRESS_MINUTE);
    hour = read_ds1307(ADDRESS_HOUR);
    date = read_ds1307(ADDRESS_DATE);
    month = read_ds1307(ADDRESS_MONTH);
    year = read_ds1307(ADDRESS_YEAR);
    
//    UartSendString("20.04.16 09:12:07  pH=  ");
    UartSendNum(date/10);
    UartSendNum(date%10);
    UartSendString(".");
    UartSendNum(month/10);
    UartSendNum(month%10);
    UartSendString(".");
    UartSendNum(year/10);
    UartSendNum(year%10);
    UartSendString(" ");
    UartSendNum(hour/10);
    UartSendNum(hour%10);
    UartSendString(":");
    UartSendNum(minute/10);
    UartSendNum(minute%10);
    UartSendString(":");
    UartSendNum(second/10);
    UartSendNum(second%10);
}
void SetupForFirstProgram(void)
{
    if(Read_DS1307(ADDRESS_FIRST_PROGRAM) != 0x22)
    {
        SetupTimeForRealTime();
        Write_DS1307(ADDRESS_FIRST_PROGRAM, 0x22);
    }
}

void SetupTimeForRealTime()
{
    second = 50;
    minute = 59;
    hour = 23;
    date = 31;
    month = 12;
    year = 14;
    
    write_ds1307(ADDRESS_SECOND, second);
    write_ds1307(ADDRESS_MINUTE, minute);
    write_ds1307(ADDRESS_HOUR, hour);
    write_ds1307(ADDRESS_DATE, date);
    write_ds1307(ADDRESS_MONTH, month);
    write_ds1307(ADDRESS_YEAR, year);
}
