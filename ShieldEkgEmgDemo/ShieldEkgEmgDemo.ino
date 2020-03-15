#include <compat/deprecated.h>
#include <EmgLibrary.h>

int th_min = -1;
int it = 0;
int pinADC=0;
int input_value = 0;
int LED1 = 13;
int CAL_SIG = 5; //??????
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
 
 while(it < 1000){
  th_min = calibr_max_signal(input_value, 0.05);
 }
 
}



void loop() {

 __asm__ __volatile__ ("sleep");

}

ISR(TIMER1_COMPA_vect)
{
  it++;
  input_value = adc_read(pinADC);
//  if( th_min == -1 && it < 5000){ 
//    th_min = calibr_max_signal(input_value, 0.05);
//  }
  //Serial.print(int(low_pass_filter(derivative(float(input_value)),0.05)));
  //Serial.println(input_value);
  //int temp = int(derivative(float(input_value)));
  
  int temp = input_value;
  if(temp < 700 && temp > 300){
    Serial.print(temp);
    Serial.print('\n');
  }
  else{
    Serial.print(0);
    Serial.print('\n');
  }
  
  //Serial.print(int(input_value));
 
}

void adc_init(){
     
    ADMUX = (1<<REFS0); //AVCC with external capacitor at AREF pin = 5V
    ADCSRA = (1<<ADEN)|(1<<ADPS0);// ADC enable and system clock div 2
}

uint16_t adc_read(int pin)
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
