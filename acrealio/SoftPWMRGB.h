#ifndef SOFTPWMRGB_H
#define SOFTPWMRGB_H

class SoftPWMRGB
{
public:
  SoftPWMRGB();
  void setPins(int pinR, int pinG, int pinB);
  void setPWM(int valR, int valG, int valB); // Call this function in a loop to be sure that LED states are updated
    
private:
  unsigned long period; // Period of the PWM signal in microseconds

  int pin[3]; // We'll store the RGB LED's pins here
  int val[3]; // We'll store our requested RGB values here
  int previousVal[3]; // Used to check if the value has changed
  unsigned long onTime[3]; // Calculated ON times
  unsigned long offTime[3]; // Calculated OFF times
  unsigned long previousMicros[3]; // Used to keep track of time between each change of state
  int state[3]; // We'll store the RGB LEDs states here
};

#endif

