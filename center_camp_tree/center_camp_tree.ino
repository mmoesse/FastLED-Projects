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
 
//#define DATA_PIN    9
#define DATA_PIN    5
//uncomment for LPD8806
//#define CLK_PIN   4
#define LED_TYPE    WS2811
//#define LED_TYPE    LPD8806
#define COLOR_ORDER RGB

#define NUM_LEDS    100
#define PATTERN_DELAY_SEC 30
#define UPDATES_PER_SECOND 50
CRGB leds[NUM_LEDS];
 
#define BRIGHTNESS 255  //1-255
//#define FRAMES_PER_SECOND  120
#define FRAMES_PER_SECOND  120

//pallette
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;


 
void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
}
 


 
// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
//SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };
SimplePatternList gPatterns = { OceanColors, ForestColors, cylon, LavaColors, rainbowWithGlitter, CloudColors, SlowRainbow };
//SimplePatternList gPatterns = { LavaColors };
 
uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns


  
void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();
 
  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  //FastLED.delay(1000/FRAMES_PER_SECOND); 
 
  // do some periodic updates
  //EVERY_N_MILLISECONDS( 2 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( PATTERN_DELAY_SEC ) { nextPattern(); } // change patterns periodically
}
 
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


 
void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}



void solid() 
{
  // solid color
  //fill_solid( leds, NUM_LEDS, gHue);
  fill_solid( leds, NUM_LEDS, CHSV(gHue, 255, 255));
  //fill_solid( leds, NUM_LEDS, CHSV(100, 200, 255));
  //fill_solid( leds, NUM_LEDS, CRGB(255, 0, 0));
}

void gradient() 
{
  
  //fill_gradient_RGB(leds, 0, 1, NUM_LEDS-1, 0x110000);
  fill_gradient(leds, 0, CHSV(0, 1, 255), NUM_LEDS-1, CHSV(0, 255, 255), SHORTEST_HUES);  //full saturation red to no saturation
  
}

 
void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 100);
}
 
void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(10);
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
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS-1);
  leds[pos] += CHSV( gHue, 255, 192);
}

void sinelon1()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(4,0,NUM_LEDS-1);
  leds[pos] += CHSV( gHue, 255, 192);
}
 
void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}
 
void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}


void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void cylon() { 
  static uint8_t hue = 160;
  Serial.print("x");
  // First slide the led in one direction
  for(int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, hue++, 255);
    // Show the leds
    FastLED.show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(25);
  }
  Serial.print("x");

  // Now go in the other direction.  
  for(int i = (NUM_LEDS)-1; i >= 0; i--) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, hue++, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(8);
  }
}

void SlowRainbow()
{
  static uint8_t starthue = 0;
  fill_rainbow( leds, NUM_LEDS, ++starthue, 2);
  FastLED.setTemperature( Candle ); 
  
  FastLED.show();
  FastLED.delay(150);
  
  
}

void OceanColors()
{
    currentPalette = OceanColors_p;         currentBlending = LINEARBLEND;
    
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    
    FillLEDsFromPaletteColors( startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void ForestColors()
{
    currentPalette = ForestColors_p;         currentBlending = LINEARBLEND;
    
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    
    FillLEDsFromPaletteColors( startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void LavaColors()
{
    currentPalette = LavaColors_p;         currentBlending = LINEARBLEND;
    
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    
    FillLEDsFromPaletteColors( startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void CloudColors()
{
    currentPalette = CloudColors_p;         currentBlending = LINEARBLEND;
    
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    
    FillLEDsFromPaletteColors( startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}


void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}


