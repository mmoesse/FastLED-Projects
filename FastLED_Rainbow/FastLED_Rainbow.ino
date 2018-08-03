#include "FastSPI_LED2.h"

#define NUMLEDS 150
#define BRIGHTNESS 255

CRGB pixelBuffer[20];

void setup() {
  //defines pixel strip, RGB order (GRB in this case) and # of LEDs (20)
  //FastLED.addLeds<LPD8806,GRB>( pixelBuffer, 20 );
  //NEOPIXEL
  FastLED.addLeds<WS2811,10,GRB>(pixelBuffer, NUMLEDS);
  FastLED.setBrightness(BRIGHTNESS); 
  
  Serial.begin(19200);
}

byte i = 0;
byte startingHue = 0;

void loop()
{
  i = i + 1;
  if( i >= 20) { i = 0; }
  
  pixelBuffer[i].setRGB( 100, 20, 0) ;
  
  hsv2rgb_rainbow( CHSV( 100, 255, 255), pixelBuffer[i] );
  
  startingHue = startingHue + 1;
  fill_rainbow( pixelBuffer, 20, startingHue);
    
  FastLED.show();
  pixelBuffer[i] = 0x000000;
    
  //in milliseconds
  delay(10);
}
