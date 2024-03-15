#include <FastLED.h>

namespace ledstrip
{

  const int LED_PIN = 3;
  char sequence[] = { 'l', 'r', 'l', 'l', 'r', 'r', 'l', 'r', 'l', 'l', 'l'};
  const int NUM_LEDS = 11; // length of sequence

  CRGB leds[NUM_LEDS];

  void setup() {
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

    leds[0] = CRGB(255, 0, 0);
    FastLED.show();
  }

  void beginSequence()
  {
    int index = NUM_LEDS-1;

    
  }
}