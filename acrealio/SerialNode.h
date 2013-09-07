#include "Arduino.h"
#include "Node.h"


class SerialNode: public Node
{
public:
    SerialNode(); //contructor
    void init();
     boolean isActive();
    short processRequest(byte* request, byte* sendBuff);
    
public: 
 
private:

};
