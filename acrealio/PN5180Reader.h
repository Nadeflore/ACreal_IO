#ifndef PN5180Reader_H
#define PN5180Reader_H

#include "Arduino.h"
#include "RfidModule.h"
#include "src/PN5180/PN5180.h"
#include "src/PN5180/PN5180FeliCa.h"
#include "src/PN5180/PN5180ISO15693.h"

class PN5180Reader : public RfidModule
{
public:
	PN5180Reader();
	void setPins(int sensor, HardwareSerial* serialid);
	void read();
	void update();
	byte isCardPresent();
	void getUID(byte* uid);

private:
	byte card;        // 0 : no card 1:ISO15693 2:Felica
	byte uid[8];
	boolean pinset;   // pin init done flag
	boolean readcmd;  // read request from host flag
	byte readstatus;  // 0 : idle, 1 : looking for ISO15693, 2 : looking for Felica
};

#endif



