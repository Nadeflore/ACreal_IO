#ifndef SL015M_H
#define SL015M_H

#include "Arduino.h"
#include "RfidModule.h"

class SL015M : public RfidModule
{
public:
    SL015M();
    void setPins(int sensor, HardwareSerial* serialid);
    void read();
	void update();	
    boolean isCardPresent();
    void getUID(byte* uid);	

private:
    boolean card;               // card presence
    byte uid[8];
    boolean rfcmdsent;
    unsigned long timesent;
    byte rfidp[256];            // rfid response buffer
    byte rf_i;         //response buffer size
    short rfSENSOR;             // rfid OUT pin
    HardwareSerial* rfSerial;    // rfid Serial
    
    boolean pinset;
    boolean readcmd; // read request from host flag

};

#endif
