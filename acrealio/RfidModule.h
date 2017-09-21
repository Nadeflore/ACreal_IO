#ifndef RDIDMODULE_H
#define RDIDMODULE_H

#include "Arduino.h"
#include "pinoutconfig.h"

#if RFID_MODULE1==1
  #define RFID_MODULE1_TYPE RR10
#elif RFID_MODULE1==2
  #define RFID_MODULE1_TYPE SL015M
#else
  #define RFID_MODULE1_TYPE RR10
#endif

#if RFID_MODULE2==1
  #define RFID_MODULE2_TYPE RR10
#elif RFID_MODULE2==2
  #define RFID_MODULE2_TYPE SL015M
#else
  #define RFID_MODULE2_TYPE RR10
#endif

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

