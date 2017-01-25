#include "Arduino.h"
#include "LedBoard.h"


//contructor
LedBoard::LedBoard(char* rCode)
{
    byte rType[] = {0x04, 0x01, 0x00, 0x00};
    byte rVersion[] = {0x01, 0x00, 0x00};
	  setVersion(rType, 0x00, rVersion, rCode);
}

void LedBoard::init()
{
	memset(ledStatus, 0, 18); 
	pinMode(LED1_R, OUTPUT);
	pinMode(LED1_G, OUTPUT);
	pinMode(LED1_B, OUTPUT);
	pinMode(LED2_R, OUTPUT);
	pinMode(LED2_G, OUTPUT);
	pinMode(LED2_B, OUTPUT);
	pinMode(LED3_R, OUTPUT);
	pinMode(LED3_G, OUTPUT);
	pinMode(LED3_B, OUTPUT);
}

void LedBoard::update()
{
}


short LedBoard::processRequest(byte* request, byte* answer)
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
	  
	case 0x22:  //set leds color (for jubeat ledboard)

        //byte 0-2 front RGB
        //byte 3-5 top RGB
        //byte 6-8 left RGB
        //byte 9-11 right
        //byte 12-14 title RGB
        //byte 15-17 woofer
	
	  if(request[4] == 18)
	        memcpy(ledStatus, request+5, 18); //save led colors
			
		analogWrite(LED1_R,ledStatus[6]);
		analogWrite(LED1_G,ledStatus[7]);
		analogWrite(LED1_B,ledStatus[8]);
		analogWrite(LED2_R,ledStatus[0]);
		analogWrite(LED2_G,ledStatus[1]);
		analogWrite(LED2_B,ledStatus[2]);
		analogWrite(LED3_R,ledStatus[9]);
		analogWrite(LED3_G,ledStatus[10]);
		analogWrite(LED3_B,ledStatus[11]);
			
	  
      answer[4] = 1;
      answer[5] = 0x00;
      break;  
	  
	
      
    }



}
