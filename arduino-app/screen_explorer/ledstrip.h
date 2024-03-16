#include <FastLED.h>

namespace ledstrip
{

  const int LED_PIN = 3;
  //char sequence[] = { 'l', 'r', 'l', 'l', 'r', 'r', 'l', 'r', 'l', 'l', 'l'};
  const int NUM_LEDS = 59; // length of sequence

  CRGB leds[NUM_LEDS];

  void setup() {
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

    leds[0] = CRGB(255, 0, 0);
    FastLED.show();
  }

  void beginSequence()
  {
    int index = NUM_LEDS-1;

    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CRGB(255, 0, 0);
      FastLED.show();
      delay(100);
    }
    for (int i = NUM_LEDS; i > 0; i--)
    {
      leds[i] = CRGB(0, 0, 0);
      FastLED.show();
      delay(100);
    }

    
  }

  void travel(int fromIndex, int toIndex)
  {
    // if travelling right/forward
    if (fromIndex < toIndex)
    {
      for (int i = fromIndex; i < toIndex; i++)
      {
        FastLED.clear();
        leds[i] = CRGB(255, 0, 0);
        FastLED.show();
        delay(100);
      }
    }
    // if travelling left/backward
    else if (fromIndex > toIndex)
    {
      for (int i = fromIndex; i > toIndex; i--)
      {
        FastLED.clear();
        leds[i] = CRGB(255, 0, 0);
        FastLED.show();
        delay(100);
      }
    }

    FastLED.clear();
    FastLED.show();
  }

  void pc_to_oled()
  {
    travel(0, 22);
  }

  // DELETE THIS ONCE I ADD THE MATRIX!
  void oled_to_matrix()
  {
    travel(23, 59);
  }

  // DELETE THIS TOO
  void matrix_to_oled()
  {
    travel(59, 23);
  }
}