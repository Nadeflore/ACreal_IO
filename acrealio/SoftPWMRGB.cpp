#include "Arduino.h"
#include "SoftPWMRGB.h"

SoftPWMRGB::SoftPWMRGB()
{
  period = 2000; // Period set to 2000us to obtain a 500Hz signal)
  
  for(int i=0;i<3;i++)
  {
    val[i] = 0;
    previousVal[i] = -1;
    onTime[i] = 0;
    offTime[i] = 0;
    previousMicros[i] = 0;
    state[i] = LOW;
  }
}

void SoftPWMRGB::setPins(int pinR, int pinG, int pinB)
{
  pin[0] = pinR;
  pin[1] = pinG;
  pin[2] = pinB;
}

void SoftPWMRGB::setPWM(int valR, int valG, int valB)
{
  unsigned long currentMicros = micros(); // Get the current time
  
  val[0] = valR;
  val[1] = valG;
  val[2] = valB;
  
  for(int i=0;i<3;i++)
  {
    if(previousVal[i] != val[i]) // Only calculate the ON and OFF time if the value has changed
    {
      onTime[i] = (period*(unsigned long)val[i])/255UL; // Not sure if that much variable type casting is necessary, but it works
      offTime[i] = period - onTime[i];
      
      previousVal[i] = val[i];
    }
    
    if(state[i] == LOW && currentMicros - previousMicros[i] >= offTime[i] && offTime[i] != period) // If the LED is OFF AND we exceeded the OFF time AND the OFF time isn't equal to the period, turn the LED ON
    {
      state[i] = HIGH;
      previousMicros[i] = currentMicros;
    }
    else if(state[i] == HIGH && currentMicros - previousMicros[i] >= onTime[i] && onTime[i] != period) // Else, if the LED is ON AND we exceeded the ON time AND the ON time isn't equal to the period, turn the LED OFF
    {
      state[i] = LOW;
      previousMicros[i] = currentMicros;
    }
  }
  
  // Write to the pins
  digitalWrite(pin[0], state[0]);
  digitalWrite(pin[1], state[1]);
  digitalWrite(pin[2], state[2]);
}

