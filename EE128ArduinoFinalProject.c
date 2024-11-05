#include <LiquidCrystal.h>
#include <SPI.h>

const int rs = 8, en = 9, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
char buff[255];
volatile byte indx;
volatile boolean process;

void setup(void)
{
   Serial.begin(115200);
   pinMode(MISO, OUTPUT); // have to send on master in so it set as output
   SPCR |= _BV(SPE);      // turn on SPI in slave mode
   indx = 0;              // buffer empty
   process = false;
   SPI.attachInterrupt(); // turn on interrupt
   lcd.begin(16,2);
//   lcd.print("Hello");
}

ISR(SPI_STC_vect) // SPI interrupt routine
{
   byte c = SPDR; // read byte from SPI Data Register

   if (indx < sizeof(buff))
   {
      buff[indx++] = c; // save data in the next index in the array buff
      if (c == '\n')
      {
         buff[indx - 1] = 0; // replace newline ('\n') with end of string (0)
         process = true;
      }
   }
}

void loop(void)
{
   if (process)
   {
      process = false;      // reset the process
      Serial.println(buff); // print the array on serial monitor
      lcd.clear();
      delay(100);
      lcd.setCursor(0,0);
      lcd.print(buff);
      delay(100);
      indx = 0;             // reset button to zero
   }
}