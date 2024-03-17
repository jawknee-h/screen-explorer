#include "oled.h"
#include "lcd.h"
#include "matrix.h"
#include "ledstrip.h"
//#include "nokia.h"

auto current_screen = String("pc");

void setup()   {
	Serial.begin(115200);

	//nokia::setup();
  oled::setup();
  lcd::setup();
  matrix::setup();
  ledstrip::setup();
}

void loop()
{
  
  // Recieve player input through serial
  if (Serial.available() > 0)
  {
    char recv = Serial.read();

    if (recv == 'a')
    {
      // Send the 'left' command to the current screen
      if (current_screen == "oled")
      {
        oled::left();

        if (oled::checkbounds() == -1)
        {
          current_screen = "pc";
          ledstrip::oled_to_pc();
          Serial.write('r');
        }
      }
      else if (current_screen == "lcd")
      {
        lcd::left();

        // If the character has walked off the lcd through exit '-1',
        // move to the oled screen.
        if (lcd::checkbounds() == -1)
        {
          current_screen = "oled";
        }
      }
      else if (current_screen == "matrix")
      {
        matrix::left();

        if (matrix::checkbounds() == -1)
        {
          //current_screen = "lcd"; //CHANGE THIS BACK TO LCD AFTER WIRING THE LCD AGAIN
          current_screen = "oled";
          ledstrip::matrix_to_oled();
          oled::left();
        }
      }
      //ledstrip::left();
    }
    else if (recv == 'd')
    {
      // Send the 'right' command to the current screen
      if (current_screen == "oled")
      {
        oled::right();
        //Serial.println(oled::xpos);
        // If the character has walked off the oled through exit '1',
        // move to the lcd screen.
        if (oled::checkbounds() == 1)
        {
          //current_screen = "lcd"; //CHANGE THIS BACK TO LCD AFTER WIRING THE LCD
          current_screen = "matrix";
          ledstrip::oled_to_matrix();
          matrix::right(); // to step on screen when finished travelling
        }
      }
      else if (current_screen == "lcd")
      {
        lcd::right();

        if (lcd::checkbounds() == 1)
        {
          //erial.println("moving to matrix");
          current_screen = "matrix";
        }
      }
      else if (current_screen == "matrix")
      {
        matrix::right();
      }
    }
    else if (recv == 'b') // The character has exited the computer screen
    {
      if (current_screen == "pc")
      {
        current_screen = "oled";
        ledstrip::pc_to_oled();
        oled::right();
      }
    }
  }

  oled::draw();
  lcd::drawCharacter();

  //delay(10);
  //oled::checkbounds();
  //delay(10);

  
}