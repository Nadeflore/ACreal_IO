#ifndef IOBOARD_H
#define IOBOARD_H

#include "Arduino.h"
#include "Node.h"
#include "SoftPWMRGB.h"

class IoBoard: public Node
{
public:
    IoBoard(const char* rCode); //contructor
    void init();
    short processRequest(byte* request, byte* sendBuff);
    void update();

public:

private:
    //pushbuttons
    byte keys;
    byte keysLights;
    boolean test;
    boolean svc;
    //for volume encoders
    unsigned int volR;
    byte volRState;
    unsigned int volL;
    byte volLState;
    // Software PWM RGB light
    SoftPWMRGB LED6;
    // Values for the software PWM RGB light
    int valLED6_R;
    int valLED6_G;
    int valLED6_B;
};

#endif

