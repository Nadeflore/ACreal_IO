#ifndef RDIDMODULE_H
#define RDIDMODULE_H

#include "Arduino.h"
#include "pinoutconfig.h"

class RfidModule
{
public:
    virtual void setPins(int sensor, HardwareSerial* serialid);
    virtual void read();
    virtual void update();
    virtual byte isCardPresent();
    virtual void getUID(byte* uid);
};

#endif

