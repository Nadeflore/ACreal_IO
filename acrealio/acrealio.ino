#include "Node.h"
#include "Reader.h"
#include "LedBoard.h"
#include "IoBoard.h"
#include "CardDispenser.h"
#include "RR10.h"
#include "SL015M.h"
#include "Ddr.h"



#define MINTIME 14                // Min time between 2 sent packet(Min is 14ms, Max is around 50ms) some games require this
#define MAX_NODES 3

///////////////////////////
// Serial protocol with host
///////////////////////////
byte request[256];                // buffer filled with request bytes from host
byte req_i = 0;                    // request buffer current size

boolean escByte = false;          // flag to indicate next byte is an escaped byte
boolean initDone = false;         // indicate whether init has been done

//timestamps
unsigned long lastSent;   
unsigned long lastRecv;  

/////////////////////////
// Nodes properties
//////////////////////

byte nbnodes; // nodes count (currently supports 1 or 2)
byte node_id; //id of first node (may be != 1 when used with other physical nodes)

Node *nodes[MAX_NODES];//nodes array

//nodes delaraction (static allocation)

#if GAMETYPE == 0  //pop'n music with card dispenser

Reader nod1;//first reader 
CardDispenser nod2("HBHI"); //card dispenser

#elif GAMETYPE == 1 //1 reader

Reader nod1;//first reader 

#elif GAMETYPE == 2 //2 readers

Reader nod1;//first reader 

Reader nod2;//second reader 

#elif GAMETYPE == 3 // reader + leboard

Reader nod1;//first reader 
LedBoard nod2("LEDB");//led board

#elif GAMETYPE == 4 // reader + ioboard

Reader nod1;//first reader 
IoBoard nod2("KFCA");//io board

#else //2 readers + DDR LED board ?

Reader nod1;//first reader 

Reader nod2;//second reader

Ddr nod3;
//LedBoard nod3("LEDB");//led board

#endif


//1P rfid module allocation

#if RFID_MODULE1 == 1
SL015M mod1;
#else
RR10 mod1;
#endif



//2P rfid module allocation
#if GAMETYPE == 2 || GAMETYPE == 5
#if RFID_MODULE2 == 1
SL015M mod2;
#else
RR10 mod2;
#endif
#endif


//
// Arduino setup, run once at startup
// Init nodes depending on gametype
// feel free to change this code to get the nodes that suits your needs
//
void setup()
{
  // set nodes configuration
  
  //set first rfid module
  mod1.setPins(R1_DET,&R1_SER);
  nod1.setRfidModule(&mod1);
  
#if GAMETYPE == 0  //pop'n music with card dispenser

   nod1.setrCode("ICCA",0);
   nod1.setkeypadpins(K1_A,K1_B,K1_C,K1_1,K1_2,K1_3,K1_4);//3cols,4rows
   nodes[0] = &nod1;
   nodes[1] = &nod2;
   
   nbnodes = 2;

#elif GAMETYPE == 1 //1 reader

   nod1.setrCode("ICCA",0);
   nod1.setkeypadpins(K1_A,K1_B,K1_C,K1_1,K1_2,K1_3,K1_4);//3cols,4rows
   nodes[0] = &nod1;
   
   nbnodes = 1;

#elif GAMETYPE == 2 //2 readers
   //1p reader
   nod1.setrCode("ICCA",1);
   nod1.setkeypadpins(K1_A,K1_B,K1_C,K1_1,K1_2,K1_3,K1_4);//3cols,4rows
   nodes[0] = &nod1;
   
   //set rfid module 2
   mod2.setPins(R2_DET,&R2_SER);
   nod2.setRfidModule(&mod2);
   
   //2p reader
   nod2.setrCode("ICCA",1);
   nod2.setkeypadpins(K2_A,K2_B,K2_C,K2_1,K2_2,K2_3,K2_4);//3cols,4rows
   nodes[1] = &nod2;
   
   nbnodes = 2;

#elif GAMETYPE == 3 // reader + leboard

   nod1.setrCode("ICCB",2);
   nodes[0] = &nod1;
   nodes[1] = &nod2;
   
   nbnodes = 2;
   

#elif GAMETYPE == 4 // reader + ioboard

   //1p reader
   nod1.setrCode("ICCA",1);
   nod1.setkeypadpins(K1_A,K1_B,K1_C,K1_1,K1_2,K1_3,K1_4);//3cols,4rows
   nodes[0] = &nod1;
   nodes[1] = &nod2;
   
   nbnodes = 2;
   
#else // 2readers + DDR ??? board

   //1p reader
   nod1.setrCode("ICCB",0);
   nod1.setkeypadpins(K1_A,K1_B,K1_C,K1_1,K1_2,K1_3,K1_4);//3cols,4rows
   nodes[0] = &nod1;
   
   //set rfid module 2
   mod2.setPins(R2_DET,&R2_SER);
   nod2.setRfidModule(&mod2);
   
   //2p reader
   nod2.setrCode("ICCB",0);
   nod2.setkeypadpins(K2_A,K2_B,K2_C,K2_1,K2_2,K2_3,K2_4);//3cols,4rows
   nodes[1] = &nod2;
   
   nodes[2] = &nod3;
   
   nbnodes = 3;


#endif


	

}

//
//Arduino Loop function, loop forever as long as program is running
//
void loop()
{
  if(initDone){
    for(int r=0;r<nbnodes;r++)
    {
      nodes[r]->update();//update each node
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
    
  
    if(initDone)
    {
      getRequest();
      
      if(isRequestComplete())
      {
        if(checkRequestChecksum() )
        {
          //a correct command have been received and needs to be processed
          
          if(request[0] == 0x00 && request[2] == 0x01) // node enumeration command
            nodeEnum();
          else
          {
            if(request[0] >= node_id  && request[0] < node_id+nbnodes)//command recipient is one of our nodes
              processRequest();
            else //if it's not for us, (cmd aimed at another node or at the host) send it to next node
              forwardRequest();
          }
        
          req_i = 0; //empty request buffer
        }
      }
    }
    lastRecv = millis();
}


//
// process node enumeration command
//
void nodeEnum()
{
  node_id = request[5] + 1; //first node id is previous node id + 1
  request[5] += nbnodes; //increment node count by how many nodes this board emulate.
  sendAnswer(request); // send command to the next node
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
// get the specified Node to process the request send back the answer
//
void processRequest(){
  Node *rd = nodes[request[0] - node_id];//get the node to which the command is adressed
  byte answer[256];
  rd->processRequest(request, answer);//have it process the request
  sendAnswer(answer);

}


//
// when a command is not for any of our nodes, send it to next node as is
//
void forwardRequest(){
  sendAnswer(request);
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
  if(GAMETYPE !=4) //delay only if game is not sound voltex
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

  
}
  
//
// Wait for data and detect serial baud rate
//
long detRate()
{
  long baudrates[] = {57600,38400,19200};//baudrates to try
  int i=0;
  boolean allAA;
  
  do  //for each baud rate
  {
   i++;
   if(i> (sizeof(baudrates)/sizeof(baudrates[0])) -1 )
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
