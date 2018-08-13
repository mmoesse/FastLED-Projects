#include "FastLED.h"
 
// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014
 
#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

//Use data pin 9 for Beetle, 3 for Uno, 22 for Mega 
#define DATA_PIN    22
//Use clock pin 10 for Beetle, 24 for Mega 
#define CLK_PIN   24
//LED type for xmas tree lights and NeoPixel strips
//#define LED_TYPE    WS2811
#define LED_TYPE    WS2812
//LED type for module lights (ex. coat)
//#define LED_TYPE    WS2801
#define COLOR_ORDER GRB
//150 LEDs per coil - 600 (3 brightness); 300 (96 brightness)
#define NUM_LEDS    993
CRGB leds[NUM_LEDS];
 
#define BRIGHTNESS          170
//#define FRAMES_PER_SECOND  120
//unverified but believed 1,000 LEDs require 30 microsec/LED to update
//#define FRAMES_PER_SECOND  120
#define FRAMES_PER_SECOND  120
#define SEC_PER_PATTERNS  600
//#define SEC_PER_PATTERNS  10
//default change in hue for rainbox patter
//#define RAINBOW_DELTA_HUE  7
#define RAINBOW_DELTA_HUE  1
#define BEATS_PER_MIN   12
 
void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //Required for chipsets with clock pin (ex. WS2801, LPD1886)
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}
 
 
// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
//SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };
SimplePatternList gPatterns = { juggle, bpm };
//SimplePatternList gPatterns = { rainbow };
//can't get the random glitter frequent enough
//SimplePatternList gPatterns = { rainbowWithGlitter };
//no
//SimplePatternList gPatterns = { confetti };  
//increase length of tail
//SimplePatternList gPatterns = { sinelon }; 
//modified to increase density of lights
//SimplePatternList gPatterns = { juggle };   
//longer snake?
//SimplePatternList gPatterns = { bpm };  
 
uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  
void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();
 
  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 
 
  // do some periodic updates
  //EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( SEC_PER_PATTERNS ) { nextPattern(); } // change patterns periodically
}
 
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
 
void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}
 
void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, RAINBOW_DELTA_HUE);
}
 
void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  //frequency of glitter
  //addGlitter(80);
  addGlitter(80);
}
 
void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}
 
void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}
 
void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  //speed of fade to black; higher number means fades to black faster; i.e. shorter tail
  //fadeToBlackBy( leds, NUM_LEDS, 20);
  fadeToBlackBy( leds, NUM_LEDS, 1);
  //int pos = beatsin16(13,0,NUM_LEDS);
  int pos = beatsin16(2,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}
 
void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  //uint8_t BeatsPerMinute = 62;
  uint8_t BeatsPerMinute = BEATS_PER_MIN;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    //inreasing the first multiple increase the density of colors in each tail
    //decreasing the second mulitple increases the length of the tail
    //leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*1.25));
  }
}
 
void juggle() {
  // eight (since changed) colored dots, weaving in and out of sync with each other
  //speed of fade to black; higher number means fades to black faster
  //fadeToBlackBy( leds, NUM_LEDS, 20);
  fadeToBlackBy( leds, NUM_LEDS, 7);
  //fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  //density of colored dots;  high number means more density
  //for( int i = 0; i < 8; i++) {
  //for( int i = 0; i < 20; i++) {
  for( int i = 0; i < 20; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
