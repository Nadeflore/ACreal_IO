/* this module fakes a card scan by pressing a button. Faked card type and UID is configured in pinoutconfig.h */
#ifndef StaticReader_H
#define StaticReader_H

#include "Arduino.h"
#include "pinoutconfig.h"
#include "RfidModule.h"

class StaticReader : public RfidModule
{
public:
	StaticReader();
	void setPins(int sensor, HardwareSerial* serialid);
	void read();
	void update();
	byte isCardPresent();
	void getUID(byte* uid);

private:
	byte card;        // 0 : no card 1:ISO15693 2:Felica
	byte uid[8] = STATIC_UID; //internal UID to be faked, as defined in pinoutconfig.h
	boolean pinset;   // pin init done flag
	boolean readcmd;  // read request from host flag
	byte readstatus;  // 0 : idle, 1 : looking for ISO15693, 2 : looking for Felica
};

#endif



