#include "FastSPI_LED2.h"

CRGB pixelBuffer[20];

void setup() {
  //defines pixel strip, RGB order (GRB in this case) and # of LEDs (20)
  FastLED.addLeds<LPD8806,GRB>( pixelBuffer, 20 );
  FastLED.setBrightness( 128); 
  
  Serial.begin(19200);
}

byte i = 0;
byte direction = 1;

void loop()
{
    if( i >= 19) { 
      direction = -1;
    }
    if (i <= 0) {
      direction = 1;
    }
    i = i + direction;

    //set dot at bright
    if ((i%2) > 0) {
      //alt: pixelBuffer[i].setRGB(0, 0, 255)
      //alt: hsv2rgb_rainbow( CHSV( 100, 255, 255), pixelBuffer[i] )
      pixelBuffer[i] = 0x0000FF;
    }
    else {
      pixelBuffer[i] = 0xFF0000;
    }
 
    //set next dot at dim
   
    //set previous dot at dim 
  
    //Can't figure out how to string concatinated with byte to screen
    Serial.println(i);
    
    FastLED.show();
    pixelBuffer[i] = 0x000000;
    
    //in milliseconds
    delay(50);
}
