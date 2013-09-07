#include "Arduino.h"
#include "SL015M.h"

SL015M::SL015M()
{
  card = false;
  rfcmdsent = false;
  rf_i = 0;
  pinset = false;
  readcmd = false;
}

void SL015M::setPins(int sensor, HardwareSerial* serialid)
{
    rfSENSOR = sensor;
    rfSerial = serialid;
	
    pinMode(rfSENSOR,INPUT);                            // Rfid sensor
    rfSerial->begin(RFID_BAUD);
    
    pinset=true;

}

void SL015M::update()
{
  if(!pinset)
    return;
  if(!readcmd)
    return;
  
    if(digitalRead(rfSENSOR)==LOW)             // card presence
    {
        
        if(!rfcmdsent)
        {
        
          rfSerial->write(0xBA);                  // pream
          rfSerial->write(0x02);                  // size
          rfSerial->write(0x31);                  // rfidcommand: get tag info
          rfSerial->write(0xBA ^ 0x02 ^ 0x31);    // checksum
          
          rf_i = 0;
          rfcmdsent= true;
          timesent = millis();
        }
        else // if command has been sent, let's read answer
        {

          if(rfSerial->available()>0)
          {
            
            rfidp[rf_i] = rfSerial->read();
            rf_i++;
            
            if(rfidp[0] != 0xBD)                 // restart filling buffer from begining until received byte is 0xBD (preamb)
              rf_i = 0;
            
            if(rf_i>2)                           // if we know paquet length
            {
              
              byte length = rfidp[1];
              if(rf_i >= length + 2)             // if paquet complete
              {
          
            // checksum check
                byte chk = 0x00; 
                for(int i=0;i< length+1;i++)
                  chk ^= rfidp[i];
                
                if(chk != rfidp[length+1])
                {
                  rfcmdsent = false;             // checksum error, send cmd again
                }
                else
                {
                  if(rfidp[3]!=0x00)             // Operation not succeeded
                  {
                    rfcmdsent = false;           // error, send again
                  }
                  else
                  {
            
            //success, let's copy the uid
                    for(int i=0;i<8;i++)
                    {
                      uid[i] = rfidp[11-i];
                    }
            
                    if(uid[0] == 0xE0 && uid[1] == 0x04)  // if correct konami card
                      card = true;
                    else
                      card = false;
                      
                    readcmd = false;//reading finished (card found)

                  }
                }
              }
            }
          }
          else 
          {
            // command sent, but nothing to receive
            if(millis() - timesent >500)         // if last command was sent more than 500ms ago, send a new one
              rfcmdsent = false;
          }
        }
    }
    else // if no tag in range
    {
        card = false;
        rfcmdsent = false;
        readcmd = false;//reading finished (no card found)
    }
}

boolean SL015M::isCardPresent()
{
	return card;
}

void SL015M::read()
{
  readcmd=true;
}

void SL015M::getUID(byte* uida)
{
	memcpy(uida,uid,8);
}
