/*-----------------
  ELVH  I2C  Demo
  This simple code shows basic reading and data conversion
  for the standard calibration sensors.
  20221103
-------------------*/

uint16_t Prs, Adr;
float pressure;
#include <Wire.h>
void setup() {
  Wire.begin();// join i2c bus (address optional for master)
  Serial.begin(9600);// start serial for output
  Serial.println("\n--- ELVH I2C Arduino Test:---" );// print the character
  Serial.println ();
  Serial.println ("Scanning I2C addresses...");
  Adr = ScanI2CAdrs();
  if( 0 == Adr)
  {
    Serial.println (" XXX  Part not found on I2C. Check wiring.  XXX");
    while(1)
      ;
  }
}

int n = 0;

void loop() {
    
    Wire.requestFrom(Adr, 4);// request 4 bytes from sensor                          
    Prs = Wire.read();// receive a byte as character    
    Prs &= 0x3F;// Mask off status bits
    Prs = Prs *256 + Wire.read(); 
    pressure = (((Prs-1638.0)/13108.0)*500.0);
    if(pressure<0)
  {
    pressure = 0.00;
  }
    // Serial.print("\nPressure (mbar): " );// print the character
    Serial.print(n);
    Serial.print("\t");
    Serial.print(pressure); // print the character
    Serial.print("\n");
    delay(100);// Note that delay must be > sensor update rate;
             // for Arduino, 500ms just for display.
    n = n + 1;
}

int ScanI2CAdrs( void )
{
  int foundadr = 0, count = 0;
  for (byte i = 8; i < 120; i++)
  { Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
    {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      foundadr = i;
      count++;
      delay (1);
    } // end of good response
  } // end of for loop
  Serial.println ("Done.");
  Serial.print ("Found ");
  Serial.print (count, DEC);
  Serial.println (" device(s).");
  Serial.print ("Reading    pressure");
  Serial.print("\n");
  return foundadr;
}
