#include "oled.h"
#include "lcd.h"
#include "matrix.h"
#include "ledstrip.h"

auto current_screen = String("pc");

void setup()   {
	Serial.begin(115200);

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

    // If recieved a LEFT command
    if (recv == 'a')
    {
      // Send the 'left' command to the current screen

      /* oled */
      if (current_screen == "oled")
      {
        oled::left();

        // If the character has walked off the LEFT side of the oled..
        if (oled::checkbounds() == -1)
        {
          // switch to the pc screen.
          current_screen = "pc";
          ledstrip::oled_to_pc();
          serialFlush();
          Serial.write('r'); // this tells the openFrameworks app to re-activate the character.
        }
      }
      /* lcd */
      else if (current_screen == "lcd")
      {
        lcd::left();

        // If the character has walked off the LEFT side of the lcd..
        if (lcd::checkbounds() == -1)
        {
          // move to the oled screen.
          current_screen = "oled";
        }
      }
      /* matrix */
      else if (current_screen == "matrix")
      {
        matrix::left();

        // If the character walked off the LEFT side of the matrix..
        if (matrix::checkbounds() == -1)
        {
          // Move to the oled screen.
          current_screen = "oled";
          ledstrip::matrix_to_oled();
          serialFlush();
          oled::left();
        }
      }
    }

    // If recieved a RIGHT command
    else if (recv == 'd')
    {
      // Send the 'right' command to the current screen
      /* oled */
      if (current_screen == "oled")
      {
        oled::right();
        // If the character has walked off the right side of the oled..
        if (oled::checkbounds() == 1)
        {
          // move to the lcd screen.
          current_screen = "matrix";
          ledstrip::oled_to_matrix();
          serialFlush();
          matrix::right(); // to step on screen when finished travelling
        }
      }
      else if (current_screen == "lcd")
      {
        lcd::right();

        // If the character has walked off the right side of the lcd..
        if (lcd::checkbounds() == 1)
        {
          // move to the matrix.
          current_screen = "matrix";
        }
      }
      /* matrix */
      else if (current_screen == "matrix")
      {
        matrix::right();
      }
    }
    /* Recieved the switch command (when the character has exited the computer screen) */
    else if (recv == 'b')
    {
      if (current_screen == "pc")
      {
        // Transition to the oled.
        current_screen = "oled";
        ledstrip::pc_to_oled();
        serialFlush();
        oled::right();
      }
    }
  }

  oled::draw();
  lcd::drawCharacter();
}

void serialFlush()
{
  while(Serial.available() > 0) {
    char data = Serial.read();
  }
}