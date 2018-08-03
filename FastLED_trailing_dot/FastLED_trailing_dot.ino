#include "FastSPI_LED2.h"

#define trunk_size 20
CRGB pixelBuffer[trunk_size];

void setup() {
  //defines pixel strip, RGB order (GRB in this case) and # of LEDs (20)
  FastLED.addLeds<LPD8806,GRB>( pixelBuffer, trunk_size );
  FastLED.setBrightness( 128); 
  
  Serial.begin(19200);
}

byte i = 0;
short direction = 1;
byte trunk_stop = trunk_size - 1;
byte trunk_start = 0;
long bright_color = 0x0000FF;
long dim_color = 0x00001F;
long dimmer_color = 0x00000F;
long off_color = 0x000000;
word delay_ms = 200;

void loop()
{
    if( i >= trunk_stop) { 
      direction = -1;
    }
    if (i <= trunk_start) {
      direction = 1;
    }
    i = i + direction;

    //set dot to bright
    pixelBuffer[i] = bright_color;
  
    //set next dot to dim if moving down trunk
    if ((i < trunk_stop) and (direction < 0)) {
      pixelBuffer[i + 1] = dim_color;
    }

    //set 2nd next dot to dim if moving down trunk
    if ((i+1 < trunk_stop) and (direction < 0)) {
      pixelBuffer[i + 2] = dim_color;
    }
   
    //set previous dot to dim if moving up trunk 
    if ((i > trunk_start) and (direction > 0)) {
      pixelBuffer[i -1] = dim_color;
    }
  
    //set 2nd previous dot to dim if moving down trunk
    if ((i-1 < trunk_stop) and (direction < 0)) {
      pixelBuffer[i + 2] = dim_color;
    }

    //Debug to serial
    Serial.print("i: ");
    Serial.print(i);
    Serial.print("; direction: ");
    Serial.print(direction);
    Serial.print("; bright_color: ");
    Serial.print(bright_color);
    Serial.print("; dim_color: ");
    Serial.print(dim_color);
    Serial.print("; off_color: ");
    Serial.print(off_color);
    Serial.println("");
    
    FastLED.show();
    pixelBuffer[i] = off_color;
    if (i < trunk_stop) {
      pixelBuffer[i + 1] = off_color;
    }
    if ((i+1) < trunk_stop) {
      pixelBuffer[i + 2] = off_color;
    }
    if (i > trunk_start) {
      pixelBuffer[i - 1] = off_color;
    }
    if ((i-1) > trunk_start) {
      pixelBuffer[i - 2] = off_color;
    }
    
    //in milliseconds
    delay(delay_ms);
}
