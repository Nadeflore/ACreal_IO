#include "Arduino.h"
#include "Node.h"



class IoBoard: public Node
{
public:
    IoBoard(char* rCode); //contructor
    void init();
    short processRequest(byte* request, byte* sendBuff);
    void update();
	
public: 
 
private:
	//pushbuttons
	byte keys;
	byte keysLights;
    boolean test;
	boolean svc;
	//for volume encoders
	int aVolRlast;
	int bVolRlast;
	unsigned int volR;

	int aVolLlast;
	int bVolLlast;
	unsigned int volL;

};
