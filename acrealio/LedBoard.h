#ifndef LEDBOARD_H
#define LEDBOARD_H

#include "Arduino.h"
#include "Node.h"
#include "SoftPWMRGB.h"

class LedBoard: public Node
{
public:
    LedBoard(char* rCode); //contructor
    void init();
    short processRequest(byte* request, byte* sendBuff);
    void update();

public:

private:
    byte ledStatus[18];
    // Software PWM RGB light
    SoftPWMRGB LED6;
    // Values for the software PWM RGB light
    int valLED6_R;
    int valLED6_G;
    int valLED6_B;
};

#endif

