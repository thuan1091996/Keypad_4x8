/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 2/27/2019
Author  : 
Company : 
Comments: 


Chip type               : ATmega8A
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*******************************************************/

#include <mega8.h>
#include <stdio.h>
#include <delay.h>
// Declare your global variables here
#define NUMB_ROWS 4
#define NUMB_COLS 8
const int KEYPAD[NUMB_ROWS][NUMB_COLS]=     { {1 ,2 ,3 ,4 ,5 ,6 ,7 ,8 },
                                              {11,12,13,14,15,16,17,18},
                                              {21,22,23,24,25,26,27,28},
                                              {31,32,33,34,35,36,37,38}  };
unsigned long get_numb=0;
void Keypad_Init();
int Check_KPad();
int Get_Key();
void main(void)
{

Keypad_Init();
// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART Mode: Asynchronous
// USART Baud Rate: 9600
//UCSRA=(0<<RXC) | (0<<TXC) | (0<<UDRE) | (0<<FE) | (0<<DOR) | (0<<UPE) | (0<<U2X) | (0<<MPCM);
//UCSRB=(0<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (1<<RXEN) | (1<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);
//UCSRC=(1<<URSEL) | (0<<UMSEL) | (0<<UPM1) | (0<<UPM0) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0) | (0<<UCPOL);
//UBRRH=0x00;
//UBRRL=0x33;

while (1)
      {
            if(Check_KPad()==1)
            {   
                get_numb=Get_Key();
                if(get_numb>15)  
                PORTC|=0x10;
                else             
                PORTC&=~0x10;
            }
      }
}

/*-----Check Keypad if any key is pressed----//
 * Use input (rows) to check        
 * If any row don't have value "1" --> at least one key pressed
 * retun 1
*/
int Check_KPad()
{
    int ui8_invalue=0;
    PORTB=0;
    ui8_invalue=PINC&0X0F;
    if(ui8_invalue!=0x0F)           return 1;
    else                            return 0;
}

/*Get the key are pressed in the  keyboard if detect at least one key is pressed//
 *Because all inputs pulls-up so they have value "0" if button is pressed
 *First set all outputs pin to 1, then clear col 0 (other have "1") to 0 to check
 *   if pressed button in col 0 by collects all inputs from rows ([1,2,3,4][1])
 *   if any inputs have "0" -> button pressed may have  1 4 7 * (in col 0)
 * -If no inputs have "0" so the button is another cols so check the next column and so on
*/
int Get_Key()
{
    unsigned char ui8_input=0;
    unsigned char key_pressed=0;
    unsigned char cur_col=0, cur_row=0;
    unsigned char bit_count_col=1, bit_count_row=1;                      //start with bit 0 (PIN B0)
    for(cur_col=0;cur_col<NUMB_COLS;cur_col++)
    {
        PORTB=0xFF;                     //Set all outputs to 1
        PORTB&=~bit_count_col;          //Clear 1 bit at a time
        bit_count_col<<=1;              //Shift to next bit
        bit_count_row=1;
        for(cur_row=0;cur_row<NUMB_ROWS;cur_row++)                //Check to see if the rows have "0"
        {                                                         //Specify the button in that column\
            ui8_input=PINC&0x0F;                                  //Collect input value
            if((ui8_input&bit_count_row)==0)
            key_pressed=KEYPAD[cur_row][cur_col];                 //else check another cols
            bit_count_row<<=1;
        }
    }
    return key_pressed;
}

void Keypad_Init()
{
    DDRB|=0xFF;         //PortB output (col)
    PORTC|=0x0F;       //C0->C4 input with pull-up (row)
    DDRC|=0x10;
}