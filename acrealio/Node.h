#ifndef NODE_H
#define NODE_H
#include "pinoutconfig.h"
#include "Arduino.h"
#include "Cipher.h"

class Node
{
public:
    virtual void init();

    const char* getVersion();
    void setVersion(byte* rType, byte rFlag, byte* rVersion, char* rCode);
	
    virtual short processRequest(byte* request, byte* sendBuff);
    virtual void update(); //update things like keypadstate and rfid 
        
    
private:
    
	
	
    char nodeVersion[0x2C];

};

#endif
