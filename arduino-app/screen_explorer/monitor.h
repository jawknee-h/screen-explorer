namespace monitor
{
  // Normalised screen position.
  float n_xpos = 0.0;
  float n_ypos = 0.0;

  void left()
  {
    Serial.write('v');
  }

  void right()
  {
    Serial.write('b');
  }
}