#ifndef DDR_H
#define DDR_H

#include "Arduino.h"
#include "Node.h"

class Ddr: public Node
{
public:
    Ddr(); //contructor
    void init();
    short processRequest(byte* request, byte* sendBuff);
    void update();

public:

private:

};

#endif

