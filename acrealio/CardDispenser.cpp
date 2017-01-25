#include "Arduino.h"
#include "CardDispenser.h"


//contructor
CardDispenser::CardDispenser(char* rCode)
{
    byte rType[] = {0x04, 0x00, 0x00, 0x01};
    byte rVersion[] = {0x00, 0x00, 0x07};
	  setVersion(rType , 0x00, rVersion, rCode);
}

void CardDispenser::init()
{

}

void CardDispenser::update()
{
	
}


short CardDispenser::processRequest(byte* request, byte* answer)
{  
  answer[0] = request[0] | 0x80;        // reader id
  answer[1] = request[1];               //  ?
  answer[2] = request[2];               // command
  answer[3] = request[3];               // paquet id
  answer[4] = 0;                        // data length
 
  switch (answer[2])                   // switch on the command
  {
    //
    // get version
    case 0x02:
      answer[4] = 0x2C;
      memcpy(answer+5, getVersion(), 0x2C);
      break;

    //
    // init?
    case 0x00:
    case 0x03:
    case 0x16:
    case 0x20:
    case 0x30:
      answer[4] = 1;
      answer[5] = 0x00;
      break;
	  
	case 0x72://card dispenser status
	//first byte : status
	// 1 : busy
	// 2 : ok
	// 3 : empty
	// 4 : payout
	// 5 : jam
	// 6 : maintenance
	// second byte : disburbed finished?
	// 6 - 9 : ok
	// 10 11: ?? maintenance lock
	// 12 - 17 : busy, crash ingame
	
	answer[4] = 2;
	answer[5] = 0x02;
	answer[6] = 0x06;
	break;   

	case 0x71://card dispenser, dispense command
	answer[4] = 1;
	answer[5] = 0x00;
	break;   
	  
	
      
    }



}
