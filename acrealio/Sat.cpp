#include "Arduino.h"
#include "Sat.h"


//contructor
Sat::Sat()
{
  //special type and version, node name is DDRS for all satellites
    byte rType[] = {0x05, 0x01, 0x00, 0x00};
    byte rVersion[] = {0x01, 0x00, 0x02};
    setVersion(rType, 0x00, rVersion, "DDRS");


}

//needed an overloaded set version here, one version of DDR was VERY particular about having everything perfect
void Sat::setVersion(byte* rType, byte rFlag, byte* rVersion, const char* rCode)
{
    //hard coded date and time
    char compileDate[0x10] = {0x41, 0x75, 0x67, 0x20, 0x32, 0x35, 0x20, 0x32, 0x30, 0x30, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00};
    char compileTime[0x10] = {0x31, 0x34, 0x3A, 0x32, 0x38, 0x3A, 0x34, 0x35, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    memcpy(nodeVersion, rType, 4);
    nodeVersion[4] = rFlag;
    memcpy(nodeVersion+4+1, rVersion, 3);
    memcpy(nodeVersion+8, rCode,4);
    memcpy(nodeVersion+8+4, compileDate, 0x10);
    memcpy(nodeVersion+8+4+16, compileTime, 0x10);
}

//every node will get it's individual payload, it should not exceed 40 (0x28) bytes
short Sat::processBroadcast(byte* payload, int length, byte* answer)
{
  answer[0]=0xFE;
  answer[1]=0x00;

  // Do something fun here, like maybe spam random lights on this node!
  // Someone help decipher the decision forest in the spire MCU please
  // There are fancy animations on the spires, its likely an arduino cannot
  // address and controll every light individually to perform these animations
  // and patterns, as well as brightness control. Each node will probably need
  // some sort of MCU on it's own communicated to from here to perform an animation
  // In otherwords, get ready for lots of software serials and a whole OTHER device
  // to build on top of this
  
  return 2; // the length of the answer we provide
  
}

//not a lot of these, most of this is just responding to 
short Sat::processRequest(byte* request, byte* answer)
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
    

        // init commands?
    default:
        answer[4] = 1;
        answer[5] = 0x00;
        break;



    }



}

