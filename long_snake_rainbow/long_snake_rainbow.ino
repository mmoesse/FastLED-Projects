#include "FastSPI_LED2.h"

#define DATA_PIN    3
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define BRIGHTNESS 96
#define NUMLEDS 150
#define MAXRGB 192
#define REDUCE 0.98
#define DELAY 25
#define VALUE 50

/* chasing trail of lights, random colors */

CRGB pixelBuffer[NUMLEDS];

void setup() {
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(pixelBuffer, NUMLEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  
  Serial.begin(19200);
}

byte i = 0;
byte j = 0;
byte inc = 1;
byte hue = 0;
byte sat = 180;
int sat_direction = 1;

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
  //pixelBuffer[i] = CHSV( 160, 100, 50);
  
  //create HSV object
  CHSV hsv(hue, sat, VALUE);
  //converrt HSV to RGB and add to pixelBuffer
  hsv2rgb_raw(hsv, pixelBuffer[i]);
  
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

  hue = hue + 1;

  // set saturation direction; 1 = incrementing; -1 = decrementing;
  if (sat == 255) {
    sat_direction = -1;
  } 
  if (sat == 0) {
    sat_direction = 1;
  }
  
  sat = sat + sat_direction;  
  
  delay(DELAY);
}


