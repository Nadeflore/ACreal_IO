#include "Arduino.h"
#include "StaticReader.h"

StaticReader::StaticReader()
{
	card = 0;
	pinMode(STATIC_SCAN_PIN, INPUT_PULLUP);
	readcmd = false;
	pinset = false;
	readstatus = 0;
}

void StaticReader::setPins(int sensor, HardwareSerial* serialid)
{
	pinset=true;
}

void StaticReader::update()
{
	if (!pinset)
		return;
	if (!readcmd)
		return;
	
	//internal uid has been set in constructor, just has to return card type when button is pressed, 0 otherwise
	
	if (digitalRead(STATIC_SCAN_PIN) == LOW)
	{
		card = STATIC_TYPE; // 1=iso15693  2=FeliCa
	}
	else
	{
		card = 0;
	}
	readcmd = false; //job done
}

byte StaticReader::isCardPresent()
{
	return card;
}

void StaticReader::read()
{
	readcmd = true;
}

void StaticReader::getUID(byte* uida)
{
	memcpy(uida,uid,8);
}
