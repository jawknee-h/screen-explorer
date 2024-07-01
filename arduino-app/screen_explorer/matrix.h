#include <LedControl.h>
#include <binary.h>

namespace matrix
{
  // Matrix display
  const int CS_PIN = 7;
  const int DIN_PIN = 2;
  const int CLK_PIN = 4;
  LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 0);

  // Character data
  int xpos = -1;
  int ypos = 0;

  void setup()
  {
    lc.shutdown(0,false);
    // Set brightness to a medium value
    lc.setIntensity(0,8);
    // Clear the display
    lc.clearDisplay(0);
  }

  void update()
  {
    lc.clearDisplay(0); // clear the display..
    lc.setLed(0, xpos, ypos, 1); // then draw a dot in the character's new position.
  }

  void right()
  {
    xpos += 1; // update the character's position.

    update(); // update the display to reflect the character's new position.
  }

  void left()
  {
    xpos -= 1;

    update(); // update the display to reflect the character's new position.
  }

  void walkTo(int target_x)
  {
    while (xpos != target_x)
    {
      if (target_x > xpos) // If target is to the right..
      {
        right();
      }
      else if (target_x < xpos) // If the target is to the left..
      {
        left();
      }

      delay(400); // delay between steps
    }
  }

  int AI()
  {
    // Infinite logic loop
    while (true)
    {
      randomSeed(millis());
      int number = random(100);

      // 0-30: random move
      if (number < 20)
      {
        walkTo(-1);
        return -1; // -1: right
      }
      // 60-100: random screen
      else if (number < 100)
      {
        walkTo(random(0, 7));
      }

      // Small random delay between actions
      delay(random(200, 2000));
    }
  }

  int checkbounds()
  {
    if (xpos < 1)
    {
      return -1;
    }
    else if (xpos > 7) // 7 is the last pixel on the display
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
};