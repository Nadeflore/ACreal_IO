#include "Arduino.h"
#include "PN5180Reader.h"
#include "src/PN5180/PN5180.h"
#include "src/PN5180/PN5180FeliCa.h"
#include "src/PN5180/PN5180ISO15693.h"

PN5180Reader::PN5180Reader()
{
	card = 0;
	readcmd = false;
	pinset = false;
	readstatus = 0;
}

void PN5180Reader::setPins(int sensor, HardwareSerial* serialid)
{
	pinset=true;
}

void PN5180Reader::update()
{
	if (!pinset)
		return;
	if (!readcmd)
		return;

	//init reader
	uint8_t rdpn[8] = {0,0,0,0,0,0,0,0};
	PN5180FeliCa nfcFeliCa(PN5180_NSS_PIN, PN5180_BUSY_PIN, PN5180_RST_PIN);
	PN5180ISO15693 nfc15693(PN5180_NSS_PIN, PN5180_BUSY_PIN, PN5180_RST_PIN);
	nfcFeliCa.begin();
	nfcFeliCa.reset();
  
	switch (readstatus)
	{
		case 0: //look for ISO15693
		{
			nfc15693.reset();
			nfc15693.setupRF();
		
			//read ISO15693 inventory
			ISO15693ErrorCode rc = nfc15693.getInventory(rdpn);
			if (rc == ISO15693_EC_OK )
			{
				for (int i=0; i<8; i++) //fix uid as ISO15693 protocol sends data backwards
				{ 
					uid[i] = rdpn[7-i];
				}

				if (uid[0] == 0xE0 && uid[1] == 0x04) // if correct konami card, job is done
				{
					card = 1; //iso15693
					readcmd = false; //job done
					readstatus = 0;
				}
				else //tag found but bad ID
				{
					readstatus = 1; // try to find a FeliCa
				}
			}
			else //tag not found
			{
				readstatus = 1; // try to find a FeliCa
			}
			break; 
		}


		case 1: //look for FeliCa
		{
			nfcFeliCa.reset();
			nfcFeliCa.setupRF();
			uint8_t uidLength = nfcFeliCa.readCardSerial(rdpn);

			if (uidLength > 0) //tag found
			{
				for (int i=0; i<8; i++)
				{
					uid[i] = rdpn[i];
				}
				card = 2; //felica
				readcmd = false; //job done
				readstatus = 0;
			}
			else //tag not found
			{
				card = 0;
				readcmd = false; //we tried both protocols and found nothing, job done
				readstatus = 0;
			}
			break;
		}
	}
}

byte PN5180Reader::isCardPresent()
{
	return card;
}

void PN5180Reader::read()
{
	readcmd = true;
}

void PN5180Reader::getUID(byte* uida)
{
	memcpy(uida,uid,8);
}






