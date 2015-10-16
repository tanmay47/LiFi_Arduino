volatile char data;
volatile char bit_pos;
volatile char temp_1;

void setup()
{
  cli();
  temp_1 = 0b00100000;
  //io
  Serial.begin(9600);
  DDRB = 0b00000000;
  PORTB = 0b00100000;//pullup datainput pin (13)
  DDRD = 0b00000000;
  PORTD = 0b00000100;//pullup int0 pin
  //Timer int setup
  TCCR1A = 0b00000000;//CTC disable
  TCCR1B = 0b00000000;  //disable int 
  TIMSK1 = 0b00000010;
  OCR1A = 1999; // adjust to mid way of pulse width
  OCR1B = 12999; // garbage disposal (anywhere between 7999 to 15999)
  bit_pos = 0b10000000;
  TCNT1 = 0;
  //hardware int
  EICRA = 0b00000011;//rising edge int0
  EIMSK = 0b00000001;
  sei();
}
void loop()
{
  while(bit_pos != 0b00000000)//blocking function
  {}
  cli();
  Serial.println(data);
  bit_pos = 0b10000000;
  TCCR1B = 0b00000000;  //disable int 
  sei();
}
ISR(vect_INT0)//at pos edge of regenarated clock
{//instruction count = 2
  TCNT1 = 0;
  TCCR1B = 0b00000001;
}
ISR(vect_TIMER1_COMPA)// push the incoming bit to msb of 'data'
{//instruction count = 5
   data = data>>1;
   data = (((PINB & temp_1)<<2)|data);
   bit_pos = bit_pos>>1;
}
ISR(vect_TIMER1_COMPB)// garbage disposal
{//instruction count = 2
  bit_pos = 0b00000000; //dispose
  TCCR1B = 0b00000000;  //disable int 
}

