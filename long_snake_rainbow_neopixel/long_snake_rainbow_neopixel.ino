#include "FastSPI_LED2.h"
//CRGB pixelBuffer[150];

#define NUMLEDS 150
#define BRIGHTNESS 125
#define START 0
#define END 149
#define HUE_WALK 2
#define SAT_WALK 2
#define REDUCE 0.9125
#define DELAY 8
#define VALUE 255

/* chasing trail of lights, random colors */

CRGB pixelBuffer[NUMLEDS];

void setup() {
  FastLED.addLeds<WS2811,10,GRB>(pixelBuffer, NUMLEDS);
  FastLED.setBrightness(BRIGHTNESS);
  
  Serial.begin(19200);
}

int i = START;
int j = 0;
byte inc = 1;
byte hue = 0;
byte sat = 180;
int sat_direction = 1;

void loop() {
  if (inc == 1) {
    i = i + 1;
    if (i >= END) { inc = -1; }
  } else {
    i = i - 1;
    if (i < START) { inc = 1; }
  }
  
  //create HSV object
  CHSV hsv(hue, sat, VALUE);
  //converrt HSV to RGB and add to pixelBuffer
  hsv2rgb_raw(hsv, pixelBuffer[i]);
  
  //set pixel to blue for debugging
  //;pixelBuffer[i] = CRGB::Blue;
  
  FastLED.show();

  //debug info
  Serial.println(i);
  Serial.println(hue);
  Serial.println(sat);
  Serial.println(pixelBuffer[i]);

  
  for (j = 0; j < NUMLEDS; j++) {
    pixelBuffer[j].r *= REDUCE;
    pixelBuffer[j].g *= REDUCE;
    pixelBuffer[j].b *= REDUCE;

  }

  hue = hue + random8(HUE_WALK) + 1;

  // set saturation direction; 1 = incrementing; -1 = decrementing;
  if (sat == (256 - SAT_WALK)) {
    sat_direction = - 1;
  } 
  if (sat == (SAT_WALK - 1)) {
    sat_direction = 1;
  }
  
  sat = sat + sat_direction * (random8(SAT_WALK) + 1);  
  
  delay(DELAY);
}


