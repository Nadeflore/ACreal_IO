#ifndef RR10_H
#define RR10_H

#include "Arduino.h"
#include "RfidModule.h"

class RR10 : public RfidModule
{
public:
    RR10();
    void setPins(HardwareSerial* serialid);
    void read();
    void update();	
    boolean isCardPresent();
    void getUID(byte* uid);	

private:
    boolean card;               // card presence
    byte uid[8];
    int comstatus; // 0 : idle, 1 : handshake sent , 2 : command sent , 3 : handshake sent back, waiting for answer
    unsigned long timesent;
    byte rfidp[256];            // rfid response buffer
    byte rf_i;         //response buffer size
    HardwareSerial* rfSerial;    // rfid Serial
    
    boolean pinset;  // pin init done flag
    boolean readcmd; // read request from host flag


};

#endif
