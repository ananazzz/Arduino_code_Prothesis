#include <avr/io.h>
#include <avr/interrupt.h>

int pin1 = 13;

void t1_init()
{
   
   TCCR1A = (1<<COM1A1);
   TCCR1B = (0<<CS12)|(0<<CS11)|(1<<CS10)|(1<<WGM12); //Дільник = 1 (65535 -> 0.0040959 c)

   OCR1A = 32000;

   TIMSK1 |= (1<<TOIE1)|(1<<OCIE1A);

   Serial.println("Timer1_init");
   sei();               
}


ISR(TIMER1_COMPA_vect)
{
   Serial.println(digitalRead(12));
}

void setup()
{
    Serial.begin(9600);
    pinMode(13, OUTPUT);
    pinMode(12, INPUT);

    Serial.println("Start");
    t1_init();
}

void loop()
{
  
  digitalWrite(13, true);
  delay(2);
  digitalWrite(13, false);
  delay(2);
  
}
