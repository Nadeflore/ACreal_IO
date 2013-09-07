#include "Node.h"
#include "Reader.h"
#include "LedBoard.h"
#include "IoBoard.h"
#include "SerialNode.h"
#include "CardDispenser.h"
#include "RR10.h"
#include "SL015M.h"



#define MINTIME 14                // Min time between 2 sent packet(Min is 14ms, Max is around 50ms) some games require this
#define MAX_NODES 2

///////////////////////////
// Serial protocol with host
///////////////////////////
byte request[256];                // buffer filles with request bytes from host
byte req_i = 0;                    // request buffer current size

boolean escByte = false;          // flag to indicate next byte is an escaped byte
boolean requestprocessed = false; // indicate whether request has been processed or not
boolean initDone = false;         // indicate whether init has been done

int nb00=0; // number of 0x00 received in a row

//timestamps
unsigned long lastSent;   
unsigned long lastRecv;  

/////////////////////////
// Nodes properties
//////////////////////

short nbnodes; // nodes count (currently supports 1 or 2)

Node *nodes[MAX_NODES];


//
// Arduino setup, run once at startup
// Init nodes depending on gametype
//
void setup()
{
  // init nodes depending on game type
  
  // init first reader used in all games types
  Reader* read1 = new Reader();
  
  //rfid module, 2 modules model are supported, configure the module to use
  switch(RFID_MODULE)
  {
    case 1:
    {
        SL015M* mod1 = new SL015M();
        mod1->setPins(R1_DET,&R1_SER);
        read1->setRfidModule(mod1);
        break;
    }
    case 2:
    {
        RR10* mod1 = new RR10();
        mod1->setPins(&R1_SER);
        read1->setRfidModule(mod1);
        break;
    }
  }
    
      
  switch(gametype)
  {
    case 4: //Sound voltex, set an IoBoard + a Reader
    {
      //1st node IOBOARD
      IoBoard* iob = new IoBoard("KFCA");
      nodes[1] = iob;
      
      //2nd node reader 
      read1->setrCode("ICCA",true);
        //keypad
        read1->setkeypadpins(K1_A,K1_B,K1_C,K1_1,K1_2,K1_3,K1_4);//3cols,4rows
     
      nodes[0] = read1;
      break;
    }
    
    
    case 3://jubeat, set a reader (without keypad) + a LedBoard
    {
      read1->setrCode("ICCB");
      
      nodes[0] = read1;
      nodes[1] = new LedBoard("LED"); // jubeat ledboard node
      
      break;
    }
    case 2://2readers game, Set reader 2 as node 2
    {
      Reader* read2 = new Reader();
      read2->setrCode("ICCA",true);
      read2->setkeypadpins(K2_A,K2_B,K2_C,K2_1,K2_2,K2_3,K2_4);//3cols,4rows
      //rfid module
      SL015M* mod2  = new SL015M();
      mod2->setPins(R2_DET,&R2_SER);
      read2->setRfidModule(mod2);
      nodes[1] = read2;
    }
	
    case 1: //1 Reader game, set reader 1 as node 1
    {
       read1->setrCode("ICCA",gametype == 2);
       //keypad
       read1->setkeypadpins(K1_A,K1_B,K1_C,K1_1,K1_2,K1_3,K1_4);//3cols,4rows
       nodes[0] = read1;
       break;
    }
    case 0: // pop'n music with card dispenser, 1 reader + 1 card dispenser
    {
       read1->setrCode("ICCA");
         //keypad
       read1->setkeypadpins(K1_A,K1_B,K1_C,K1_1,K1_2,K1_3,K1_4);//3cols,4rows
       nodes[0] = read1;
       nodes[1] = new CardDispenser("PCDA");
       
       break;
    }
     
   
  }
  
	//set number of nodes present
    if(gametype == 1)
		nbnodes = 1;
	else
		nbnodes = 2;
	

}

//
//Arduino Loop function, loop forever as long as program is running
//
void loop()
{
  if(initDone){
    for(int r=0;r<nbnodes;r++)
    {
      nodes[r]->update();//update every node
    }

    if((millis() - lastRecv) > 50000){      // If no comm for 50sec, timeout
      initDone = false;                     // Reset init
      //digitalWrite(13, LOW);
      Serial.end();                         // and close serial port
    }
  } else {
	//if reader is not initialized
     detRate();// Detect baud rate
     initNodes(); //init each node
     lastRecv = millis();
     
     initDone = true;
     //digitalWrite(13, HIGH);
     
  }
}


//
//Arduino serialEvent(), called each time a byte is received on Serial
//
void serialEvent(){
  
  //switch to unintialized state when receiving lots of zeros
  
  if(Serial.peek() == 0x00)
     nb00++;
  else
    nb00=0;
    
  if(nb00 > 128)
  {
      initDone = false;                     // Reset init
      //digitalWrite(13, LOW);
      Serial.end();                         // and close serial port
  }
  
  
  
  
  
    if(initDone)
    {
      getRequest();
      
      if(isRequestComplete())
      {
        if(checkRequestChecksum() && !requestprocessed)
        {
          if(request[0] == 0 && request[2] == 0x01) // cmd 0x01 is a special command requesting the number of nodes present
            sendNodeNb();
          else
            forwardRequest();
        
        }
      }
    }
    lastRecv = millis();
}


//
// Send answer to the 0x01 "how many nodes" cmd
//
void sendNodeNb()
{
  byte answer[7] = {0x00,0x00,0x01,request[3],0x01,nbnodes,0xFF};
  sendAnswer(answer);
}


//
// Init each node
//
void initNodes(){
  for(int r=0;r<nbnodes;r++)
  {
  nodes[r]->init();
  }
}



//
// get request from serial port, and set it on request buffer
//
void getRequest() 
{
  if(Serial.available() > 0)
  {
    byte inByte = Serial.read();
     
    if(inByte == 0xAA)                    // AA -> new request is coming
    {
      if(req_i == 0)
        Serial.write(0xAA);               // send back AA if previous byte was AA too (init sequence)
        
      requestprocessed = false;
      escByte = false;
      req_i = 0;                          // clear request buffer
    }
    
    if(inByte == 0xFF)                    // if FF, this means next byte is an escaped byte
    {
      escByte = true;
    }
    
    
    if(inByte != 0xFF && inByte != 0xAA)  // if the value is not AA nor FF, add it to the request buffer
    {
      if(escByte)                         // if this is an escaped byte
      {
          inByte = ~inByte;
          escByte = false;
      }
      
      // add byte to the request buffer
      request[req_i] = inByte;
      req_i++;
      
     
    }
  }  
}

//
// Return true if request present in buffer is complete
//
boolean isRequestComplete()
{
  if(req_i >= 6)                // check if at least minimum size
  {
    if(req_i >= 6+ request[4])  // if long enough, including data length
    {
      return true;
    }
  }
  return false;
}

//
// Return true if request present in buffer have correct checksum
//
boolean checkRequestChecksum()
{
  byte sum = 0;
  int bufsize = 6 + request[4];
  
  for(int i=0;i<bufsize-1;i++)
  {
    sum += request[i];
  }
  
  return (sum == request[bufsize-1]);
}

//
// Forward Request to specified Node send back the answer
//
void forwardRequest(){
  Node *rd = nodes[request[0]-1];
  byte answer[256];
  rd->processRequest(request, answer);
  sendAnswer(answer);

}

//
// Send answer
//
void sendAnswer(byte* answer)
{
   // checksum calculation
  byte sum = 0;
  int bufsize = 6 + answer[4];
  
  for(int i=0;i<bufsize-1;i++)
  {
    sum += answer[i];
  }
  
   answer[bufsize-1] = sum;
   
   
   //checksum calc done, let's send it
   
   //delay if needed
  if(gametype !=4) //delay only if game is not sound voltex
  { 
    unsigned long now = millis();
    if((now - lastSent) < MINTIME && (now - lastSent) > 0)   // Check If last packet was too early
      delay(MINTIME - (now - lastSent));                     // delay required MINTIME between 2 packets
  }
    
   
   
  
  Serial.write(0xAA);
  
  for(int i=0;i<bufsize;i++)
  {
    byte outByte =  answer[i];
    
    if( outByte == 0xAA || outByte == 0xFF )  // escape these chars
    {
      outByte = ~outByte;
      Serial.write(0xFF);
    }
    
    Serial.write(outByte);
    
    
  }
  
  lastSent = millis();
  requestprocessed = true;

  
}
  
//
// Wait for data and detect serial baud rate
//
long detRate()
{
   long baudrates[] = {38400, 57600};//baudrates to try
  int i=0;
  boolean allAA;
  
  do  //for each baud rate
  {
   i++;
   if(i>1)
     i=0;
   //flush in and out buffer
   while(Serial.available())
     Serial.read();
   
   Serial.flush();
   Serial.end();
   Serial.begin(baudrates[i]);//try a baud rate
   
   while(Serial.available() < 10);//wait to receive 10 bytes
   
   //check if all byte have value 0xAA
   allAA = true;
   for(int j=0;j<10;j++)
   {
     if(Serial.read() != 0xAA)
     {
       allAA = false;
       break;
     }
   }
  }
  while(!allAA);
  
  //baudrate has been detected, and port is open at correct baudrate

   return baudrates[i];
  } 
