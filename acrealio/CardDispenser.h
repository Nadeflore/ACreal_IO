#include "Arduino.h"
#include "Node.h"

class CardDispenser: public Node
{
public:
    CardDispenser(char* rCode); //contructor
    void init();
    short processRequest(byte* request, byte* sendBuff);
    void update();
	
public: 
 
private:

};
