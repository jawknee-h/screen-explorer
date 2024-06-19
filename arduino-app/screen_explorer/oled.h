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

      if (arms_raised) raise();
      else lower();
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

  void draw()
  {
    u8g2.clearBuffer();
    u8g2.setDrawColor(1);

    // legs
    u8g2.drawLine(xpos-1, ypos, xpos-1, ypos-3); // left leg
    u8g2.drawLine(xpos+1, ypos, xpos+1, ypos-3); // right leg
    // feet
    u8g2.drawLine(xpos-1, ypos, xpos-2, ypos); // left foot
    u8g2.drawLine(xpos+1, ypos, xpos+2, ypos); // right foot

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

  /* movement */
  void walk(int offset)
  {
    xpos += offset;

    // Toggle arms with every step
    if (arms.arms_raised)
    {
      arms.lower();
    }
    else
    {
      arms.raise();
    }

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

  int checkbounds()
  {
    int buffer = 5; // how far past the edge of the screen the character must go before being transported. Compensates for the character sprite's width.
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
}