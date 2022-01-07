/*****************************************************************************
 *	P89V51 Microprocessors
 *	file name : lcdText.c	
 *	purpose : functions for lcdText
 *
 *	Copyright(C) BKIT - tdlong506(5/2009), nova(10/2014)
 *
*****************************************************************************/
#include "lcd.h"
#define LCD_SCREEN_0		0
#define LCD_SCREEN_1		1
#define LCD_SCREEN_2            2
#define LCD_SCREEN_3		3
const unsigned char UserFont[8][8] = {
  { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
  { 0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10 },
  { 0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18 },
  { 0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C },
  { 0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E },
  { 0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F },
  { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
  { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 }
};

int current_row, current_col;
char statusLCD = LCD_SCREEN_0;
unsigned char LcdScreen[2][16] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
unsigned long lcd_power_of(int A, int x);

void lcd_delay(unsigned long time) {
//	time = 6*time;
//	while(--time);
    int i,j;
    for(i=0;i<time;i++)
            for(j=0;j<5;j++);
}
void v_timer0(unsigned long time)
{
    int i,j;
    for(i=0;i<time;i++)
            for(j=0;j<238;j++);
}
void lcd_sendCommand(unsigned char cmd)	//Sends Command to LCD
{
	LCD_PORT&=0x0f;
	LCD_PORT|=(cmd&0xf0);
	LCD_PORT|=(1 << LCD_E);
	//lcd_delay(1);
	v_timer0(1);

	LCD_PORT&=~(1 <<LCD_E);
	v_timer0(1);

	LCD_PORT&=0x0f;
	LCD_PORT|=(cmd<<4);
	LCD_PORT|=(1 <<LCD_E);
	v_timer0(1);

	LCD_PORT&=~(1 <<LCD_E);
	v_timer0(1);
}

void init_lcd() {
//	int i;
	unsigned char cmd;
        v_timer0(25);//Wait more than 15ms after Vcc = 4.5V
        LCD_PORT&=0x0f;
	LCD_PORT&=0xf8;

        //---------one------
        if (LCD_DATA_4BIT_HIGH)
        {
            LCD_DATA_OUT(0x03<<4)
        }
        else
        {
            LCD_DATA_OUT(0x03)
        }
	EN(SET);
        v_timer0(1);
	EN(CLR);
        v_timer0(1);

        //---------two------
	if (LCD_DATA_4BIT_HIGH)
        {
            LCD_DATA_OUT(0x03<<4)
        }
        else
        {
            LCD_DATA_OUT(0x03)
        }
	EN(SET);
        v_timer0(1);
	EN(CLR);
        v_timer0(1);

        //---------three------
	if (LCD_DATA_4BIT_HIGH)
        {
            LCD_DATA_OUT(0x03<<4)
        }
        else
        {
            LCD_DATA_OUT(0x03)
        }
	EN(SET);
        v_timer0(1);
	EN(CLR);
        v_timer0(1);

        //---------four------
	if (LCD_DATA_4BIT_HIGH)
        {
            LCD_DATA_OUT(0x02<<4)
        }
        else
        {
            LCD_DATA_OUT(0x02)
        }
	EN(SET);
        v_timer0(1);
	EN(CLR);
        v_timer0(1);

        //--------4 bit--dual line---------------
	lcd_sendCommand(0x28);
        //-----increment address, invisible cursor shift------
	lcd_sendCommand(0x0c);	
}
/*=================================================*/
/* ham kiem tra trang thai thuc thi lenh cua LCD */
unsigned char lcd_read_status() {
	unsigned char status = 0x00;
	unsigned char status_bitHigh = 0x00;
	
	RS(CMD);
	//RW(READ);
	lcd_delay(15);
	LCD_DIR_IN;
	EN(SET);
		lcd_delay(15);		
		status_bitHigh = LCD_DATA_IN;
	EN(CLR);
	lcd_delay(15);
	LCD_DIR_IN;
	EN(SET);		
		lcd_delay(15);		
		status = LCD_DATA_IN;
		//status >>= 4;
		status |= status_bitHigh;
	EN(CLR);
	lcd_delay(1);
	return status;
}

/*=================================================*/
unsigned char lcd_wait_busy() {
	unsigned char retValue = 0x00;
	do {
		retValue = lcd_read_status();
	} while(retValue & 0x80);
	return retValue;
}

/*=================================================*/
/*=================================================*/
/* send 4bits data from MCU to LCD */
void lcd_write_4bits(unsigned char dat) {	
	//RW(WRITE);
        LCD_DATA_OUT(dat);
	EN(SET);
        lcd_delay(5);
	EN(CLR);
        lcd_delay(5);
}

/*=================================================*/
/* ham goi lenh thuc thi den LCD */
void lcd_write_cmd(unsigned char cmd){
	
	//lcd_wait_busy();
	RS(CMD);
        lcd_delay(5);
	if (LCD_DATA_4BIT_HIGH)
	{
		lcd_write_4bits(cmd);
		lcd_write_4bits(cmd<<4);
	}
	else
	{
		lcd_write_4bits(cmd>>4);
		lcd_write_4bits(cmd);
	}
}

/* ham goi data den LCD */
void lcd_write_data(unsigned char data) {
        //lcd_wait_busy();
	RS(DAT);
        lcd_delay(5);
	if (LCD_DATA_4BIT_HIGH)
	{
		lcd_write_4bits(data);
		lcd_write_4bits(data<<4);
	}
	else
	{
		lcd_write_4bits(data>>4);
		lcd_write_4bits(data);
	}
        RS(CMD);
}

void lcd_putchar (char c)
{ 
  lcd_write_data (c);
}

void lcd_print_char (char c)
{ 
  lcd_write_data (c);
}
/*******************************************************************************
* Print sting to LCD display                                                   *
*   Parameter:    string: pointer to output string                             *
*   Return:                                                                    *
*******************************************************************************/

void lcd_print_str (const rom unsigned char *string)
{
  while (*string)  {
    lcd_print_char (*string++);
  }
}

void lcd_set_cursor (unsigned char row, unsigned char column)
{
  unsigned char address;

  address = (row%2 * 0x40) + column%16;
  address = 0x80 + (address & 0x7F);
  lcd_write_cmd(address);               /* Set DDRAM address counter to 0     */											  
}

void lcd_clear (void)
{
  lcd_write_cmd(0x01);                  /* Display clear                      */
  lcd_set_cursor (0, 0);
}
/*=================================================*/
unsigned long lcd_power_of(int A, int x) {
    char i;
    unsigned long temp = 1;
    for(i = 0; i < x; i++)
            temp *= A;
    return temp;
}
void lcd_print_num(long num) {
    char num_flag = 0;
    char i;

    if(num == 0) {
            lcd_print_char('0');
            return;
    }
    if(num < 0) {
            lcd_print_char('-');
            num *= -1;
    }
    //else
    //	lcd_print_char(' ');

    for(i = 7; i > 0; i--) {
            if((num / lcd_power_of(10, i-1)) != 0) {
                    num_flag = 1;
                    lcd_print_char(num/lcd_power_of(10, i-1) + '0');
            }
            else {
                    if(num_flag != 0)
                            lcd_print_char('0');
            }
            num %= lcd_power_of(10, i-1);
    }
}
void LcdPrintNum(unsigned char x, unsigned char y, long num)
{
	lcd_set_cursor(x,y);
	lcd_print_num(num);
}
void LcdPrintString(unsigned char x, unsigned char y, unsigned char * string)
{
	lcd_set_cursor(x,y);
	lcd_print_str(string);
}

void lcd_print_charS(unsigned char c)
{
    LcdScreen[current_row][ current_col] = c;
    current_col ++;
}

void LcdClearS()
{
    char i;
    for (i = 0; i<16; i++)
    {
        LcdScreen[0][i] = ' ';
        LcdScreen[1][i] = ' ';
    }
}
void DisplayLcdScreen()
{
    unsigned char i;
    switch (statusLCD)
    {
        case LCD_SCREEN_0:
            lcd_set_cursor (0,0);
            for (i = 0; i<8; i++)
                lcd_print_char(LcdScreen[0][i]);
            statusLCD = LCD_SCREEN_1;
            break;
         case LCD_SCREEN_1:
            lcd_set_cursor (0,8);
            for (i = 0; i<8; i++)
                lcd_print_char(LcdScreen[0][8 + i]);
            statusLCD = LCD_SCREEN_2;
            break;
         case LCD_SCREEN_2:
            lcd_set_cursor (1,0);
            for (i = 0; i<8; i++)
                lcd_print_char(LcdScreen[1][i]);
            statusLCD = LCD_SCREEN_3;
            break;
         case LCD_SCREEN_3:
            lcd_set_cursor (1,8);
            for (i = 0; i<8; i++)
                lcd_print_char(LcdScreen[1][8 + i]);
            statusLCD = LCD_SCREEN_0;
            break;
        default:
            statusLCD = LCD_SCREEN_0;
            break;
    }
}
void DisplayLcdScreen2()
{
    unsigned char i;
    switch (statusLCD)
    {
        case LCD_SCREEN_0:
            lcd_set_cursor (0,0);
            for (i = 0; i<16; i++)
                lcd_print_char(LcdScreen[0][i]);
            statusLCD = LCD_SCREEN_1;
            break;
        case LCD_SCREEN_1:
            lcd_set_cursor (1,0);
            for (i = 0; i<16; i++)
                lcd_print_char(LcdScreen[1][i]);
            statusLCD = LCD_SCREEN_0;
            break;
        default:
            statusLCD = LCD_SCREEN_0;
            break;
    }
}
void DisplayLcdScreenOld()
{
    unsigned char i;
    lcd_set_cursor (0,0);
    for (i = 0; i<16; i++)
        lcd_print_char(LcdScreen[0][i]);
        //lcd_print_char(~'B');
    lcd_set_cursor (1,0);
    for (i = 0; i<16; i++)
        lcd_print_char(LcdScreen[1][i]);
}
void LcdPrintCharS(unsigned char x, unsigned char y,unsigned char c)
{
    current_row = x%2;
    current_col = y%16;
    LcdScreen[current_row][ current_col] = c;
}

void LcdPrintNumS(unsigned char x, unsigned char y, long num)
{
    char num_flag = 0;
    char i;
    current_row = x%2;
    current_col = y%16;

    if(num == 0) {
            lcd_print_charS('0');
            return;
    }
    if(num < 0) {
            lcd_print_charS('-');
            num *= -1;
    }
    //else
    //	lcd_print_charS(' ');

    for(i = 7; i > 0; i--) {
            if((num / lcd_power_of(10, i-1)) != 0) {
                    num_flag = 1;
                    lcd_print_charS(num/lcd_power_of(10, i-1) + '0');
            }
            else {
                    if(num_flag != 0)
                            lcd_print_charS('0');
            }
            num %= lcd_power_of(10, i-1);
    }
}
void LcdPrintStringS(unsigned char x, unsigned char y, const rom unsigned char *string)
{
    current_row = x%2;
    current_col = y%16;
    while (*string)  {
    lcd_print_charS (*string++);}
}
// Dinh dang so ab.cd
void LcdPrintNumPercentS(unsigned char row, unsigned char column, long num)
{
//		long temp_num = num;
//        char i;
//		LcdPrintNumS(row,column,temp_num/1000);
//		temp_num = temp_num%1000;
//		LcdPrintNumS(row,column+1,temp_num/100);
//		temp_num = temp_num%100;
//		LcdPrintStringS(row,column + 2,".");
//		LcdPrintNumS(row,column+3,temp_num/10);
//		temp_num = temp_num%1000;
//		LcdPrintNumS(row,column+4,temp_num%10);
        long temp_num = num;
        char i;
        if(temp_num >= 10)
		LcdPrintNumS(row,column,temp_num);
        else {
        LcdPrintNumS(row,column,0);
        LcdPrintNumS(row,column+1,temp_num);
        }
		//LcdPrintStringS(row,column + 2,".");
		//LcdPrintNumS(row,column+3,temp_num/10);
		//temp_num = temp_num%1000;
		//LcdPrintNumS(row,column+4,temp_num%10);
}

