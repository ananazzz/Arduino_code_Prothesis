#include <avr/io.h>
#include <avr/interrupt.h>

int pin1 = 13;
int pinADC = 0;

void t1_init()
{
   TCCR1A = (1<<COM1A1);
   TCCR1B = (0<<CS12)|(1<<CS11)|(0<<CS10)|(1<<WGM12); //Дільник = 8 (65535)

   OCR1A = 2000; // 1 ms

   TIMSK1 |= (1<<TOIE1)|(1<<OCIE1A);

   Serial.println("Timer1_init");
   sei();               
}


ISR(TIMER1_COMPA_vect)
{
   Serial.println(adc_read(pinADC));
}

void setup()
{
    Serial.begin(9600);
    pinMode(13, OUTPUT);
    pinMode(12, INPUT);

    Serial.println("Start");
    t1_init();
    adc_init();
}

void loop()
{
  /*
  digitalWrite(13, true);
  delay(2);
  digitalWrite(13, false);
  delay(2);
  */
}

void adc_init(){
     
    ADMUX = (1<<REFS0); //AVCC with external capacitor at AREF pin = 5V
    ADCSRA = (1<<ADEN)|(1<<ADPS0);// ADC enable and system clock div 2
    Serial.println("ADC_init");
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
