//timer interrupt = the frequency of data rate - init this with
//both interrupts with same timer OCR1A OCR1B
//1. CTC (OCR1A)
//2. 1 kbps
//3. 1 KHz freq == 500 uSec ()
//volatile int to carry status in the ISR
//etc....

volatile char data;
volatile char bit_pos;
volatile char temp_1;
volatile char temp_2;

void setup()
{
  cli();
  temp_1 = 0b00000001;//for msb (<<5)
  temp_2 = 0b00010000;//lsb
  //io
  Serial.begin(9600);
  DDRB = 0b00110000;
  PORTB = 0b00000000;//0
  //Timer int setup
  TCCR1A = 0b00000000;
  TCCR1B = 0b00001001;
  TIMSK1 = 0b00000110;
  OCR1A = 7999; // frequency of operation
  OCR1B = 3999; //width of pulse
  TCNT1 = 0;
  sei();
}
void loop()
{
  if(Serial.available() > 0)
  {
    while(bit_pos != 0b00000000)
    {}
    cli();
    PORTB = 0b00000000;//0
    data = Serial.read();
    bit_pos = 0b10000000;
    sei();
  }
}
ISR(vect_TIMER1_COMPA)
{//instruction count = 1
  PORTB = 0b00100000;//0 = 0b10; 1 = 0b11; -1 = 0b01;
}
ISR(vect_TIMER1_COMPB)
{//instruction count = 5
  PORTB = (((data & temp_1)<<5)|temp_2);
  data = data>>1;
  bit_pos = bit_pos>>1;
}
