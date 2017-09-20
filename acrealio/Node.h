#ifndef NODE_H
#define NODE_H
#include "pinoutconfig.h"
#include "Arduino.h"
#include "Cipher.h"

class Node
{
public:
    //making these default empty to save on space if someone wants to go through and kill all those empty implementations
    virtual void init(){};
    virtual void update(){}; //update things like keypadstate and rfid


    const char* getVersion();
    virtual void setVersion(byte* rType, byte rFlag, byte* rVersion, const char* rCode)
    {
          char compileDate[0x10] = __DATE__;
          char compileTime[0x10] = __TIME__;
          memcpy(nodeVersion, rType, 4);
          nodeVersion[4] = rFlag;
          memcpy(nodeVersion+4+1, rVersion, 3);
          memcpy(nodeVersion+8, rCode,4);
          memcpy(nodeVersion+8+4, compileDate, 0x10);
          memcpy(nodeVersion+8+4+16, compileTime, 0x10);
    };

    virtual short processRequest(byte* request, byte* sendBuff);//everyone should implement this
    virtual short processBroadcast(byte* payload, int length, byte* answer){return 0;};// by default a class doesn't need to implement this
    

    char nodeVersion[0x2C];

};

#endif

