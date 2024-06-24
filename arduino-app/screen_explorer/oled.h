#include <U8g2lib.h>

namespace oled
{

  U8G2_SSD1306_64X48_ER_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // EastRising 0.66" OLED breakout board, Uno: A4=SDA, A5=SCL, 5V powered

  const int SCRN_WIDTH = 64;
  const int SCRN_HEIGHT = 48;
  int xpos = -10;
  int ypos = SCRN_HEIGHT-5;
  int step_size = 4;

  struct Eyes {
    int xoffset = 0;
    int yoffset = 0;
    void draw()
    {
      u8g2.drawFilledEllipse(xpos-4, ypos-12 + yoffset, 3, 2, U8G2_DRAW_ALL); // eye fill
      u8g2.setDrawColor(0);
      u8g2.drawFilledEllipse(xpos-4, ypos-12 + yoffset, 2, 1, U8G2_DRAW_ALL); // eye outline
      u8g2.setDrawColor(1);
      u8g2.drawBox(xpos-5 + xoffset, ypos-12 + yoffset, 2, 1); // pupil

      // right eye
      u8g2.drawFilledEllipse(xpos+4, ypos-12 + yoffset, 3, 3, U8G2_DRAW_ALL); // eye fill
      u8g2.setDrawColor(0);
      u8g2.drawFilledEllipse(xpos+4, ypos-12 + yoffset, 2, 2, U8G2_DRAW_ALL); // eye outline
      u8g2.setDrawColor(1);
      u8g2.drawBox(xpos+4 + xoffset, ypos-13 + yoffset, 2, 1); // pupil
    }

    void lookLeft()
    {
      xoffset -= 1;
    }
    void lookRight()
    {
      xoffset += 1;
    }
    void lookDown()
    {
      yoffset += 1;
    }
    void lookUp()
    {
      yoffset -= 1;
    }

    void lookReset()
    {
      yoffset = 0;
      xoffset = 0;
    }
  } eyes;

  struct Arms {
    int Lx1, Ly1, Lx2, Ly2; // left arm
    int Rx1, Ry1, Rx2, Ry2; // right arm

    bool arms_raised;

    Arms()
    {
      raise();
      lower();
    }

    void update(const int new_body_xpos, const int new_body_ypos)
    {
      Lx1 = new_body_xpos-3;
      Ly1 = new_body_ypos-8;
      
      Rx1 = new_body_xpos+3;
      Ry1 = Ly1;

      // if (arms_raised) raise();
      // else lower();
    }

    void draw()
    {
      u8g2.drawLine(Lx1, Ly1, Lx2, Ly2);
      u8g2.drawLine(Rx1, Ry1, Rx2, Ry2);
    }

    /* I haven't made use of this function in this project, though it would be nice to. I just have very little time right now */
    void raise()
    {
      arms_raised = true;

      Lx2 = Lx1-6;
      Ly2 = Ly1-1;

      Rx2 = Rx1+6;
      Ry2 = Ry1-1;
      
    }

    void lower()
    {
      arms_raised = false;

      Lx2 = Lx1-4;
      Ly2 = Ly1+5;

      Rx2 = Rx1+3;
      Ry2 = Ry1+4;
    }
  } arms;

  void setup()
  {
    u8g2.begin();
  }

  // Running animation
  int rFootOffset = 0;
  int lFootOffset = 0;
  void stand()
  {
    rFootOffset = 0;
    lFootOffset = 0;
  }

  void stepLeft()
  {
    rFootOffset = -2;
    lFootOffset = 0;
  }

  void stepRight()
  {
    rFootOffset = 0;
    lFootOffset = -2;
  }

  void draw()
  {
    u8g2.clearBuffer();
    u8g2.setDrawColor(1);

    // legs
    u8g2.drawLine(xpos-1, ypos+lFootOffset, xpos-1, ypos-3); // left leg
    u8g2.drawLine(xpos+1, ypos+rFootOffset, xpos+1, ypos-3); // right leg
    // feet
    u8g2.drawLine(xpos-1, ypos+lFootOffset, xpos-2, ypos+lFootOffset); // left foot
    u8g2.drawLine(xpos+1, ypos+rFootOffset, xpos+2, ypos+rFootOffset); // right foot

    // torso
    u8g2.drawBox(xpos-3, ypos-9, 7, 6);

    // head
    u8g2.drawFilledEllipse(xpos, ypos-12, 5, 3, U8G2_DRAW_ALL);

    // left eye
    eyes.draw();

    // arms
    arms.draw();

    // antenna
    u8g2.drawLine(xpos-2, ypos-15, xpos-5, ypos-21); // left antenna
    u8g2.drawBox(xpos-6, ypos-22, 2, 2); // left bauble
    u8g2.drawLine(xpos+2, ypos-16, xpos+4, ypos-22); // right antenna
    u8g2.drawBox(xpos+4, ypos-24, 2, 2); // right bauble

    // mouth
    u8g2.setDrawColor(0);
    u8g2.drawPixel(xpos, ypos-9);

    u8g2.sendBuffer();
  }

  int checkbounds()
  {
    int buffer = 6; // how far past the edge of the screen the character must go before being transported. Compensates for the character sprite's width.
    if (xpos < 0-buffer)
    {
      return -1;
    }
    else if (xpos > SCRN_WIDTH+buffer)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }

  /* movement */
  void walk(int offset)
  {
    xpos += offset;

    // // Toggle arms with every step
    // if (arms.arms_raised)
    // {
    //   arms.lower();
    // }
    // else
    // {
    //   arms.raise();
    // }

    // updating visuals
    arms.update(xpos, ypos);
  }

  void left()
  {
    walk(-step_size);    
  }

  void right()
  {
    walk(step_size);
  }

  void walkTo(int target_x)
  {
    // While the distance to the target is greater than the bug's step size..
    while (abs(target_x - xpos) > step_size)
    {
      if (target_x > xpos) // If target is to the right..
      {
        right();
      }
      else if (target_x < xpos) // If the target is to the left..
      {
        left();
      }

      // Animate the bug as he runs..
      // - Raise and lower arms
      // - Bob head up and down
      // - Lift alternating legs
      if (arms.arms_raised)
      {
        arms.lower();
        eyes.lookUp();
        stepLeft();
      }
      else
      {
        arms.raise();
        eyes.lookDown();
        stepRight();
      }
      eyes.lookReset();
      //arms.update(xpos, ypos);

      draw(); // Update the visuals to represent the new position.
      delay(200); // Pause briefly before taking another step.
    }
    arms.lower(); // DELETE THIS POSSIBLY. Can't tell if it looks better or worse. Or add delay?
    
    // The loop above will get the bug close to the target, within the range of their step size.
    // Now we just close the final gap by finding the distance to the target from here.
    walk(target_x - xpos);
    stand(); // set legs to be straight again
    arms.update(xpos, ypos);
    draw(); // update visuals
    arms.lower(); // lower arms slightly delayed after reaching destination, for follow through.
    delay(200);
    draw();

    // while (xpos != new_xpos)
    // {
    //   if (abs(xpos - new_xpos) < step_size) // if the distance to the target is less than the step size..
    //   {
    //     xpos = new_xpos; // set the position to the target position. This avoids infinite jittering back and forth because it overshoots the target.
    //     arms.update(xpos, ypos);
    //   }
    //   else
    //   {
    //     if (xpos < new_xpos)
    //     {
    //       right();
    //     }
    //     else if (xpos > new_xpos)
    //     {
    //       left();
    //     }
    //   }
    //   draw();

    //   delay(250); // The delay here determines how rapidly it takes steps
    // }
  }

  void walkToRandom()
  {
    const int PADDING = 14;
    int newGeneratedPos = random(PADDING, SCRN_WIDTH-PADDING);

    walkTo(newGeneratedPos);
  }

  void goToLeftScreen()
  {
    walkTo(-8);
  }

  void goToRightScreen()
  {
    walkTo(SCRN_WIDTH+7);
  }

  int goToRandScreen()
  {
    // Wave arms around first
    for (int i = 0; i < 4; i++)
    {
      arms.raise();
      eyes.lookDown();
      draw();
      delay(200);
      arms.lower();
      eyes.lookUp();
      draw(); // Update the visuals
      delay(150); // Pause briefly before taking another step.
    }

    if (random(10) > 5)
    {
      goToRightScreen();
      return 1; // 1 for right
    }
    else
    {
      goToLeftScreen();
      return -1; // -1 for left
    }
  }

  void glance()
  {
    eyes.lookLeft();
    draw();
    delay(1000);
    eyes.lookRight();
    draw();
    eyes.lookRight();
    draw();
    delay(1300);
    eyes.lookLeft();
    draw();
    delay(1000);
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
        return goToRandScreen(); // return the direction of the new screen (-1: left, or 1: right)
      }
      // 30-80: glance
      else if (number < 30)
      {
        arms.lower();
        arms.update(xpos, ypos);
        glance();
      }
      // 60-100: random screen
      else if (number < 100)
      {
        walkToRandom();
      }
      

      // look left and right

      // *bounce antenna???

      // *jump???

      // change screen

      // Small random delay between actions
      delay(random(200, 1000));
    }
  }

  void randomAction()
  {
    

  }
}