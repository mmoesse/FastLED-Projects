#include "FastSPI_LED2.h"

#define NUMLEDS 20
#define MAXRGB 192
#define REDUCE 0.75
#define DELAY 100

/* chasing trail of lights, random colors */

CRGB pixelBuffer[NUMLEDS];

void setup() {
  FastLED.addLeds<LPD8806,GRB>(pixelBuffer, NUMLEDS);
  FastLED.setBrightness(32);
  
  Serial.begin(19200);
}

byte i = 0;
byte j = 0;
byte inc = 1;
long prev_color = 0x000000;

void loop() {
  if (inc == 1) {
    i = i + 1;
    if (i >= NUMLEDS-1) { inc = 0; }
  } else {
    i = i - 1;
    if (i < 1) { inc = 1; }
  }
  
/*  pixelBuffer[i].r = random(MAXRGB);
  pixelBuffer[i].g = random(MAXRGB);
  pixelBuffer[i].b = random(MAXRGB); */
  pixelBuffer[i] = 0x0000FF;
  
  FastLED.show();
  
  for (j = 0; j < NUMLEDS; j++) {
    pixelBuffer[j].r *= REDUCE;
    pixelBuffer[j].g *= REDUCE;
    pixelBuffer[j].b *= REDUCE;
  }
  
  Serial.println(i);
  
  delay(DELAY);
}


