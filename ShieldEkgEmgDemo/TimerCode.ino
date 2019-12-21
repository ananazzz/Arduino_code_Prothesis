#include <avr/io.h>
#include <avr/interrupt.h>

int pin1 = 13;

void t1_init()
{

   TCCR1B = (1<<CS12)|(0<<CS11)|(0<<CS10);

   TCNT1 = 65535-31250;

   TIMSK1 |= (1<<TOIE1);

   sei();                
}


ISR(TIMER1_OVF_vect)
{
   Serial.println(digitalRead(pin1));
}
