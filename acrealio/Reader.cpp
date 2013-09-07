#include "Arduino.h"
#include "Reader.h"


//contructor

Reader::Reader()
{ 
		keypadInitDone = false;
                new_reader = false;
}

//cmd61 is used to specify behaviour on command 0x61
void Reader::setrCode(char* rCode, boolean cmd61_s)
{
    setVersion((byte[]) {0x03, 0x00, 0x00, 0x00}, 0x00, (byte[]) {0x01, 0x06, 0x00}, rCode);
    cmd61 = cmd61_s;
}

void Reader::init()
{
    acceptcard = false;
    holdcard = false;

    keypad = 0x0000;
    keypad_old = 0x0000;
    keydown = 0x00;
    keycpt = 0x08;
	
}
////////////////////////
//keypad pinout setting
/////////////////////

void Reader::setkeypadpins(int col1, int col2, int col3, int row1, int row2, int row3, int row4)
{
    colPins[0] = col1; // COL 1 brown wire
    colPins[1] = col2; // COL 2 black wire
    colPins[2] = col3; // COL 3 blue wire

    rowPins[0] = row1; // ROW 4 purple wire
    rowPins[1] = row2; // ROW 3 yellow wire
    rowPins[2] = row3; // ROW 2 red wire
    rowPins[3] = row4; // ROW 1 grey wire
	
	
    // init Keypad pins
    for(int i=0;i<3;i++)
    {
        pinMode(colPins[i], INPUT); // High impedance
    }

    for(int i=0;i<4;i++)
    {
        // set to input with pullup
        pinMode(rowPins[i], INPUT);
        digitalWrite(rowPins[i], HIGH);
    }
	
	keypadInitDone = true;
}


void Reader::setRfidModule(RfidModule* mod)
{
	rfmodule =mod;
}

//////////////////////
///// Override update function from Node to update what a reader needs to update
//////////////////////
void Reader::update()
{
	readKeypad();
	rfmodule->update();
}


//////////////////////
///// update "keypad" with state of physical keys
//////////////////////
void Reader::readKeypad()
{
	if(!keypadInitDone)
		return;
		
		
    keypad = 0x00;
    for(int i=0;i<3;i++)//for each collumn
    {
        //set the collumn we're going to read low
        pinMode(colPins[i], OUTPUT);
        digitalWrite(colPins[i], LOW);
        for(int j=0;j<4;j++)//for each row
        {
            if(! digitalRead(rowPins[j]))  //if key is down
                keypad |= ( 0x0001 << (j + i*4) );
        }
        pinMode(colPins[i], INPUT); // after reading collumn put it back to high impedance
    }
  
  //simulate card with unused key
    // if(keypad & 0x0100) // if unused key is pressed
      // card = true;
    // else
    // {
      // card = false;
    // }

  
}

//
// Return status buffer
//
void Reader::getStatus(byte* buf)
{

    // manage keypad rising edge (keydown)
    word rising = ~keypad_old & keypad;
    if(rising)
    {
        keydown = 0x00;
        for(byte i=0;i<12;i++)      // find which key was pressed
        {
            if((0x0001 << i) & rising) // for each key, check if it went from low to high
            {
                keydown = (keycpt << 4) | i;
                break;
            }
        }
        keycpt++;
        keycpt |= 0x08;
    }
    else // if no key went from low to high since last read
    {
        keydown = 0x00;
    }
  
    keypad_old = keypad;
  
  
    // set status in buffer



    // if a card is present, copy its uid
    if(rfmodule->isCardPresent()){
        rfmodule->getUID(buf+2);
      } else {
        for(int i=0;i<8;i++)
        {
          buf[2+i] = 0x00;
        }
      }
      
      
      
      
      
    //card presence flags (different behaviour on old and new readers)
    buf[0] = 0x04; // card presence (1 or 4 :no card, 2 :card)
    buf[1] = 0x00; //sensors (for old readers)
    
    if(new_reader)
    {
          if(rfmodule->isCardPresent())
            buf[0]=0x02;
          else
            buf[0]=0x04;
    }
    else
    {
      
      // old readers sensors emulation
      if(acceptcard && rfmodule->isCardPresent())//if reader is accepting cards and a card is detected, simulate old reader holding card
      {
        holdcard = true;
      } 
      
      if(holdcard) //when holding card, both sensors are on and card is present
      {
        buf[0] = 0x02;
        buf[1] = 0x30;
      }
      else
      {
        if(rfmodule->isCardPresent())//if card is present, but reader is not accepting card, just set the front sensor
        {
         buf[0] = 0x02;
         buf[1] = 0x10;
        }
        else{
          buf[0] = 0x04;
        }
      }
    

      
    } 
      


    buf[10] = 0x00;
    buf[11] = 0x03; //0x03 required for new readers, or else will be stuck at gameover screen
    buf[12] = keydown;
    buf[13] = 0x00;
    buf[14] = keypad >> 8;
    buf[15] = keypad;
  
}



short Reader::processRequest(byte* request, byte* answer)
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
      
    //
    // read card uid (old readers)
    case 0x31:
    
    rfmodule->read();
    
    
      answer[4] = 0x10;               // 16 bytes of data
      //setStatus();
      answer[5] = 0x01;
        if(rfmodule->isCardPresent())
        {
         answer[6] = 0x10; // light up front sensor (just in case)
         answer[5] = 0x01; // if 2 -> action loop on 31 (if 0 too btw)
        }
              
    break;
    //
    // set action (old readers) return same as 0x34
    case 0x35:
      switch(request[6])
      {
        case 0x00: 
          acceptcard = false;
          break;
        case 0x11:                     // accept card mode
          acceptcard = true;
          break;
        case 0x12:                     // eject card
          holdcard = false;
          acceptcard = false; 
          break;
      }



    //
    // get status (all except pop'n music new readers)
    case 0x34:
      answer[4] = 0x10;               // 16 bytes of data
      
      if(!new_reader) //when simulating old slotted readers, rfid need to be read continously to detect card (since games using old readers only send reading command when sensors are activated)
        rfmodule->read();   
    
      
      getStatus(answer+5);
      
      

      break;

    //
    // key exchange (for popn new wavepass readers)
    case 0x60:
      if(request[4] == 4)//check if there is four bytes of data (32bits key)
      {
              unsigned long reckey = ((unsigned long) request[5]) <<24 | ((unsigned long) request[6]) <<16 | ((unsigned long) request[7]) <<8 | (unsigned long) request[8];
              //send random key
              answer[4]=4;
              answer[5]=0x12;//not random yet
              answer[6]=0xBC;//not random yet
              answer[7]=0x22;//not random yet
              answer[8]=0x01;//not random yet
              unsigned long mykey = ((unsigned long) answer[5]) <<24 | ((unsigned long) answer[6]) <<16 | ((unsigned long) answer[7]) <<8 | (unsigned long) answer[8];

              crypt.setKeys(reckey,mykey);
              
      }
      break;

    //
    // Rfid read cards UID (for new wave pass readers)
    case 0x61:
    new_reader = true; //only new readers issue this command
    
    rfmodule->read();    
    
     if(cmd61)// for pop'n music and jubeat, should not return any data, for iidx and sdvx, should return one byte of data with value 0x01
     {
        answer[4] = 1;
        answer[5] = 0x01;
     }
     
     
      break;
        
    //
    // get status (for pop'n new wavepass readers)
    case 0x64:
      if(request[4] == 1)
      {
          getStatus(answer+5);
          
          
          //append CRC
          unsigned int CRC = crypt.CRCCCITT(answer+5,16);
          answer[21] = (unsigned char) (CRC >>8) ;
          answer[22] = (unsigned char) CRC  ;
      
          crypt.encrypt(answer+5,18);
          answer[4]=18;
      }
      break;
    }



}
