#include <FastLED.h>

namespace ledstrip
{

  const int LED_PIN = 3;
  const int NUM_LEDS = 59;

  CRGB leds[NUM_LEDS];

  void setup() {
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

    FastLED.clear();
    FastLED.show();
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

  /* Transition from the pc screen to the oled */
  void pc_to_oled()
  {
    travel(0, 21);
  }

  /* Transition from the oled to the pc screen */
  void oled_to_pc()
  {
    travel(20, 0);
  }

  /* Transition from the oled to the 8x8 matrix */
  void oled_to_matrix()
  {
    travel(21, 59);
  }

  /* Transition from the 8x8 matrix to the oled */
  void matrix_to_oled()
  {
    travel(59, 20);
  }
}