#include <Adafruit_BusIO_Register.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

namespace nokia
{

  int xpos = 0;
  int ypos = 0;
  const int step_size = 10;

  // Define LCD screen obj for software SPI
  // Adafruit_PCD8544(CLK,DIN,D/C,CE,RST);
  Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

  void setup()
  {
    //display.setContrast(126); // set the contrast
    //display.setBias(0x17);
    display.begin(90, 0x14); // initialize the display
    display.clearDisplay(); // clear the display

    pinMode(3, OUTPUT);
    digitalWrite(3, LOW);
  }

  void draw()
  {
    //Draw Filled Rectangle
    display.clearDisplay();
    display.fillRect(xpos, ypos, 10, 20, BLACK);
    display.display();
  }

  void left()
  {
    xpos -= step_size;
  }

  void right()
  {
    xpos += step_size;
  }

}