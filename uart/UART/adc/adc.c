#include "adc.h"
//unsigned int adc_value[ADC_CHANNEL];
//void init_adc(void)
//{
//	TRISA = 0x01;
//	ADCON1 = 0x0d;
//	ADCON0 = 0x00;
//	ADCON2 = 0x04;
//	ADCON0bits.ADON = 1;
//}

//int get_adc_value()
//{
//	int result = 0;
//	ADCON0bits.GO_DONE = 1;
//	while(ADCON0bits.GO_DONE ==1){}
//	result = ADRESH;
//	result = (result<<2) + (ADRESL>>6);
//        adc_value[0] = result;
//	return result;
//}

unsigned int adc_value[ADC_CHANNEL];
unsigned int state[3];
void init_adc(void)
{
	TRISA = 0x2F; // khai bao ngo vao la port a
    TRISEbits.TRISE0 = 1;
  TRISEbits.TRISE1 = 1;
	ADCON1 = 0x08; // 00000000
	ADCON0 = 0x18; // 00011000  //chua cac bien dieu khien adc
	ADCON2 = 0x04; // 00000100
	ADCON0bits.ADON = 1; // mo nguon adc
}

int get_adc_value()
{
	int result = 0;
    switch (ADCON0bits.CHS)
    {
        case 6:
            ADCON0bits.CHS = 0;
            break;
        case 0:
            ADCON0bits.CHS = 1;
            break;
        case 1:
            ADCON0bits.CHS = 2;
            break;
        case 2:
            ADCON0bits.CHS = 3;
            break;
        case 3:
            ADCON0bits.CHS = 4;
            break;
        case 4:
            ADCON0bits.CHS = 5;
            break;
        case 5:
            ADCON0bits.CHS = 6;
            break;
        default:
            ADCON0bits.CHS = 0;
            break;
    }
	ADCON0bits.GO_DONE = 1;   // bat dau qua trinh chuyen doi
	while(ADCON0bits.GO_DONE ==1){} //cho den khi chuyen doi xong thi ADCON0bits.GO_DONE tu dong chuyen ve 1
	result = ADRESH;
	result = (result<<2) + (ADRESL>>6);
    switch (ADCON0bits.CHS)
    {
        case 0:
            adc_value[0] = result;
            break;
        case 1:
            adc_value[1] = result;
            break;
        case 2:
            adc_value[2] = result;
            break;
        case 3:
            adc_value[3] = result;
            break;
        case 4:
            adc_value[4] = result;
            break;
        case 5:
            adc_value[5] = result;
            break;
        case 6:
            adc_value[6] = result;
            break;
        default:
            adc_value[0] = result;
            break;
    }
	return result;
}
