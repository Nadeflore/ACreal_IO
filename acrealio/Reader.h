#ifndef READER_H
#define READER_H

#include "Arduino.h"
#include "Cipher.h"
#include "Node.h"
#include "RfidModule.h"

class Reader : public Node
{
public:
    Reader(); //contructor
    void setrCode(char* rCode, byte cmd61 = 0);
    void init();
    void update(); //update things like keypadstate and rfid

    void setkeypadpins(int col1, int col2, int col3, int row1, int row2, int row3, int row4);
    void readKeypad();

    void setRfidModule(RfidModule* mod);

    short processRequest(byte* request, byte* sendBuff);

    virtual void getStatus(byte* buf);
    void readRfid();

private:
    boolean acceptcard;         // reader state, accepting cards or not (for emulating old readers)
    boolean holdcard;           // reader state, holding card or not (for emulating old readers)
    byte uid[8];                // store uid of holding card (for emulating old readers)



    boolean keypadInitDone;
    word keypad;                // keys currently pressed
    word keypad_old;            // keys pressed before last read
    byte keydown;               // used to indicated rising edge on keys
    byte keycpt;                // this cpt is incremented each time a key is pressed
    short colPins[3];           // matrix has 3 cols...
    short rowPins[4];           // ...and 4 rows

    byte cmd61; //used to specify behaviour on command 0x61
    boolean new_reader; //set to true in case new wave pass card reader behaviour is requested from game
    Cipher crypt;

    RfidModule* rfmodule;

};

#endif

