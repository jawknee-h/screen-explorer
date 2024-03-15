#include <LiquidCrystal.h>

/// HOW TO USE //
// - call setup()

namespace lcd
{
    const int rs = 7;
    const int e = 8;
    const int d4 = 9;
    const int d5 = 10;
    const int d6 = 11;
    const int d7 = 12;
    const int contrast_pin = 6;    
    LiquidCrystal lcd(rs, e, d4, d5, d6, d7);

    // character data
    int xpos = -1;
    int step_time = 300; // time between movements in ms

    int bodyIndex = 1;
    int antennaIndex = 2;
    int doorIndex = 3;
    int ladderIndex = 4;
    int lampIndex = 5;
    int tableIndex = 6;

    byte shadow[8] = {
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
    };

    byte shadow1[8] = {
      B11111,
      B11100,
      B11000,
      B10000,
      B10000,
      B00000,
      B00000,
      B00000,
    };

    byte shadow2[8] = {
      B11111,
      B00111,
      B00011,
      B00001,
      B00001,
      B00000,
      B00000,
      B00000,
    };

    byte shadow3[8] = {
      B00000,
      B00000,
      B00000,
      B10000,
      B10000,
      B11000,
      B11100,
      B11111,
    };

    byte shadow4[8] = {
      B00000,
      B00000,
      B00000,
      B00001,
      B00001,
      B00011,
      B00111,
      B11111,
    };

    // character 'sprites'
    byte lilguy[8] = {
      B01110,
      B11111,
      B00100,
      B01101,
      B11111,
      B01110,
      B01010,
      B01010,
    };

    byte guy0[8] = {
      B00000,
      B01111,
      B10011,
      B00001,
      B00101,
      B10011,
      B01110,
      B00111,
    };

    byte guy1[8] = {
      B10000,
      B11110,
      B11001,
      B10000,
      B10100,
      B11001,
      B01110,
      B11100,
    };

    byte guy2[8] = {
      B01111,
      B11111,
      B10111,
      B10111,
      B10010,
      B00010,
      B00010,
      B00110,
    };

    byte guy3[8] = {
      B11110,
      B11111,
      B11101,
      B11101,
      B01001,
      B01000,
      B01000,
      B01100,
    };

    byte lilguy1[8] = {
      B11111,
      B00100,
      B01101,
      B11011,
      B11111,
      B01110,
      B01010,
      B01010,
    };

    byte lilguyant[8] = {
      B00000,
      B00000,
      B00000,
      B00000,
      B10001,
      B10001,
      B01001,
      B01010,
    };

    byte ladder[8] = {
      B11111,
      B10001,
      B11111,
      B10001,
      B11111,
      B10001,
      B11111,
      B10001,
    };

    byte door[8] = {
      B11111,
      B11111,
      B11111,
      B11111,
      B11101,
      B11111,
      B11111,
      B11111,
    };

    byte table[8] = {
      B00000,
      B00000,
      B00000,
      B00000,
      B11111,
      B00100,
      B00100,
      B01010,
    };

    byte lamp[8] = {
      B01110,
      B11111,
      B11111,
      B11111,
      B10100,
      B00100,
      B00100,
      B01110,
    };

    byte entrance[8] = {
      B10000,
      B10000,
      B10000,
      B10000,
      B11000,
      B10000,
      B10000,
      B10000,
    };

    // functions
    void setup()
    {
      // Set the display's contrast
      analogWrite(contrast_pin, 100);

      //lcd.createChar(0, guy0);
      //lcd.createChar(1, guy1);
      //lcd.createChar(2, guy2);
      //lcd.createChar(3, guy3);
      //lcd.createChar(4, lilguy);
      // lcd.createChar(2, entrance);
      // lcd.createChar(4, table);
      lcd.createChar(bodyIndex, lilguy1);
      lcd.createChar(antennaIndex, lilguyant);
      lcd.createChar(doorIndex, door);
      lcd.createChar(ladderIndex, ladder);
      lcd.createChar(lampIndex, lamp);
      lcd.createChar(tableIndex, table);

      lcd.begin(16, 2);
      lcd.clear();
    }

    void drawBackground()
    {
      // door
      //lcd.setCursor(2, 1);
      //lcd.write(byte(ladderIndex));

      // lamp
      lcd.setCursor(12, 1);
      lcd.write(byte(lampIndex));

      // entrance
      lcd.setCursor(0, 1);
      lcd.write(byte(doorIndex));

      // table
      lcd.setCursor(8, 1);
      lcd.write(byte(tableIndex));
    }

    void drawCharacter()
    {
      drawBackground();

      if (xpos < 0) return; // Don't draw the character if they are off the screen to the left

      // upper character
      lcd.setCursor(xpos, 0);
      lcd.write(byte(antennaIndex));
      // lower character
      lcd.setCursor(xpos, 1);
      lcd.write(byte(bodyIndex));
    }

    void right()
    {
      lcd.clear();
      xpos = min(16, xpos + 1); // moving right but only allowing him to go one box out of frame
      drawCharacter();
    }

    void left()
    {
      lcd.clear();
      xpos = max(-1, xpos - 1); // moving left but only allowing him to go one box out of frame
      if (xpos != -1) // not drawing if it should be off-screen (because the sprite can't technically go offscreen to the left)
      {
        drawCharacter();
      }
      else // making sure to still draw background even when the character is off screen
      {
        drawBackground();
      }
    }

    void moveTo(int new_xpos)
    {
      while (new_xpos > xpos)
      {
        right();
        delay(step_time);
      }
      while (new_xpos < xpos)
      {
        left();
        delay(step_time);
      }
    }

    int checkbounds()
    {
      if (xpos < 0)
      {
        return -1;
      }
      else if (xpos > 15) // 15 is the width of the display.
      {
        return 1;
      }
      else
      {
        return 0;
      }
    }
}