#include "Arduino.h"
#include "Sat.h"


//contructor
Sat::Sat()
{
    byte rType[] = {0x05, 0x01, 0x00, 0x00};
    byte rVersion[] = {0x01, 0x00, 0x02};
    setVersion(rType, 0x00, rVersion, "DDRS");


}

void Sat::setVersion(byte* rType, byte rFlag, byte* rVersion, const char* rCode)
{
    char compileDate[0x10] = {0x41, 0x75, 0x67, 0x20, 0x32, 0x35, 0x20, 0x32, 0x30, 0x30, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00};
    char compileTime[0x10] = {0x31, 0x34, 0x3A, 0x32, 0x38, 0x3A, 0x34, 0x35, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    memcpy(nodeVersion, rType, 4);
    nodeVersion[4] = rFlag;
    memcpy(nodeVersion+4+1, rVersion, 3);
    memcpy(nodeVersion+8, rCode,4);
    memcpy(nodeVersion+8+4, compileDate, 0x10);
    memcpy(nodeVersion+8+4+16, compileTime, 0x10);
}

short Sat::processBroadcast(byte* payload, int length, byte* answer)
{
  answer[0]=0xFE;
  answer[1]=0x00;
  return 2;
  
}


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

