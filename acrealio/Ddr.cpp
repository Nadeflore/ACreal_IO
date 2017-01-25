#include "Arduino.h"
#include "Ddr.h"


//contructor
Ddr::Ddr()
{
    byte rType[] = {0x04, 0x02, 0x00, 0x00};
    byte rVersion[] = {0x01, 0x01, 0x00};
	  setVersion(rType, 0x00, rVersion, "HDXS");
}

void Ddr::init()
{
}

void Ddr::update()
{
}


short Ddr::processRequest(byte* request, byte* answer)
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
	  
   case 0x10:  //unknow

      break;  
	  
	
      
    }



}
