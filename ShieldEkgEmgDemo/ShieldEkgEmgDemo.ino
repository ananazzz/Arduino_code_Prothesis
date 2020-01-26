/**********************************************************/
/* Demo program for:                                      */
/*    Board: SHIELD-EKG/EMG + Olimexino328                */
/*  Manufacture: OLIMEX                                   */
/*  COPYRIGHT (C) 2012                                    */
/*  Designed by:  Penko Todorov Bozhkov                   */
/*   Module Name:   Sketch                                */
/*   File   Name:   ShieldEkgEmgDemo.ino                  */
/*   Revision:  Rev.A                                     */
/*    -> Added is suppport for all Arduino boards.        */
/*       This code could be recompiled for all of them!   */
/*   Date: 19.12.2012                                     */
/*   Built with Arduino C/C++ Compiler, version: 1.0.3    */
/**********************************************************/
/**********************************************************
Purpose of this programme is to give you an easy way to 
connect Olimexino328 to ElectricGuru(TM), see:
https://www.olimex.com/Products/EEG/OpenEEG/EEG-SMT/resources/ElecGuru40.zip
where you'll be able to observe yours own EKG or EMG signal.
It is based on:
***********************************************************
* ModularEEG firmware for one-way transmission, v0.5.4-p2
* Copyright (c) 2002-2003, Joerg Hansmann, Jim Peters, Andreas Robinson
* License: GNU General Public License (GPL) v2
***********************************************************
For proper communication packet format given below have to be supported:
///////////////////////////////////////////////
////////// Packet Format Version 2 ////////////
///////////////////////////////////////////////
// 17-byte packets are transmitted from Olimexino328 at 256Hz,
// using 1 start bit, 8 data bits, 1 stop bit, no parity, 57600 bits per second.

// Minimial transmission speed is 256Hz * sizeof(Olimexino328_packet) * 10 = 43520 bps.

struct Olimexino328_packet
{
  uint8_t	sync0;		// = 0xa5
  uint8_t	sync1;		// = 0x5a
  uint8_t	version;	// = 2 (packet version)
  uint8_t	count;		// packet counter. Increases by 1 each packet.
  uint16_t	data[6];	// 10-bit sample (= 0 - 1023) in big endian (Motorola) format.
  uint8_t	switches;	// State of PD5 to PD2, in bits 3 to 0.
};
*/
/**********************************************************/
#include <compat/deprecated.h>
#include <FlexiTimer2.h>
#include <EmgLibrary.h>
//http://www.arduino.cc/playground/Main/FlexiTimer2
// All definitions
#define NUMCHANNELS 6
#define HEADERLEN 4
#define PACKETLEN (NUMCHANNELS * 2 + HEADERLEN + 1)
#define SAMPFREQ 256                      // ADC sampling rate 256
#define TIMER2VAL (1024/(SAMPFREQ))       // Set 256Hz sampling frequency                    
#define LED1  13
#define CAL_SIG 9

int it = 0;
int pinADC=0;
// Global constants and variables
volatile unsigned char TXBuf[PACKETLEN];  //The transmission packet
volatile unsigned char TXIndex;           //Next byte to write in the transmission packet.
volatile unsigned char CurrentCh;         //Current channel being sampled.
volatile unsigned char counter = 0;	  //Additional divider used to generate CAL_SIG
volatile unsigned int ADC_Value = 0;	  //ADC current value

//~~~~~~~~~~
// Functions
//~~~~~~~~~~

/****************************************************/
/*  Function name: Toggle_LED1                      */
/*  Parameters                                      */
/*    Input   :  No	                            */
/*    Output  :  No                                 */
/*    Action: Switches-over LED1.                   */
/****************************************************/
void Toggle_LED1(void){

 if((digitalRead(LED1))==HIGH){ digitalWrite(LED1,LOW); }
 else{ digitalWrite(LED1,HIGH); }
 
}


/****************************************************/
/*  Function name: toggle_GAL_SIG                   */
/*  Parameters                                      */
/*    Input   :  No	                            */
/*    Output  :  No                                 */
/*    Action: Switches-over GAL_SIG.                */
/****************************************************/
void toggle_GAL_SIG(void){
  
 if(digitalRead(CAL_SIG) == HIGH){ digitalWrite(CAL_SIG, LOW); }
 else{ digitalWrite(CAL_SIG, HIGH); }
 
}

void setup() 
{
 noInterrupts();  // Disable all interrupts before initialization
 pinMode(LED1, OUTPUT);  //Setup LED1 direction
 digitalWrite(LED1,LOW); //Setup LED1 state
 pinMode(CAL_SIG, OUTPUT);
 Serial.begin(115200);
 adc_init();
 timer1_init();
 interrupts();  // Enable all interrupts after initialization has been completed
}



void loop() {
  
 __asm__ __volatile__ ("sleep");
 
}

ISR(TIMER1_COMPA_vect)
{
   it++;
   if( it > 65000) it = 0;
   Serial.print(it);
   Serial.print(',');
   Serial.print(adc_read(pinADC));
   Serial.print('\n');
}

void adc_init(){
     
    ADMUX = (1<<REFS0); //AVCC with external capacitor at AREF pin = 5V
    ADCSRA = (1<<ADEN)|(1<<ADPS0);// ADC enable and system clock div 2
}

static uint16_t adc_read(int pin)
{
  if (pin < 0 || pin > 5){ //ADC0-ADC5 is available
    return 1; // pin number is out of range
  }
  pin &= 0x5;
  ADMUX |= (ADMUX & 0xF8) |pin;
  ADCSRA |= (1<<ADSC);
  while (ADCSRA & (1<<ADSC)); //as long as ADSC pin is 1
  return (ADC);
}

void timer1_init()
{
   TCCR1A = (1<<COM1A1);
   TCCR1B = (0<<CS12)|(1<<CS11)|(0<<CS10)|(1<<WGM12); //Дільник = 8 (65535)

   OCR1A = 2000; // 1 ms

   TIMSK1 |= (1<<TOIE1)|(1<<OCIE1A);

   sei();               
}
