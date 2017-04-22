#ifndef LEDBOARD_H
#define LEDBOARD_H

#include "Arduino.h"
#include "Node.h"

class LedBoard: public Node
{
public:
    LedBoard(char* rCode); //contructor
    void init();
    short processRequest(byte* request, byte* sendBuff);
    void update();

public:

private:
    byte ledStatus[18];

};

#endif

