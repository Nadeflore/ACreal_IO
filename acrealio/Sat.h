#include "Arduino.h"
#include "Node.h"

class Sat: public Node
{
public:
    Sat(); //contructor
    short processRequest(byte* request, byte* sendBuff);
    short processBroadcast(byte* payload, int length, byte* answer);
    void setVersion(byte* rType, byte rFlag, byte* rVersion, const char* rCode);
private:
  

};

