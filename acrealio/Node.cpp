#include "Arduino.h"
#include "Node.h"


//
// Construct node version
//
void Node::setVersion(byte* rType, byte rFlag, byte* rVersion, const char* rCode)
{
    char compileDate[0x10] = __DATE__;
    char compileTime[0x10] = __TIME__;
    memcpy(nodeVersion, rType, 4);
    nodeVersion[4] = rFlag;
    memcpy(nodeVersion+4+1, rVersion, 3);
    memcpy(nodeVersion+8, rCode,4);
    memcpy(nodeVersion+8+4, compileDate, 0x10);
    memcpy(nodeVersion+8+4+16, compileTime, 0x10);
}

//
// Return node version
//
const char* Node::getVersion()
{
    return nodeVersion;
}


