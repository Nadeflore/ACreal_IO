#include "Arduino.h"
#include "LedBoard.h"

//contructor
LedBoard::LedBoard(char* rCode)
{
    byte rType[] = {0x04, 0x01, 0x00, 0x00};
    byte rVersion[] = {0x01, 0x00, 0x00};
    setVersion(rType, 0x00, rVersion, rCode);
}

void LedBoard::init()
{
    LED6.setPins(LED6_R, LED6_G, LED6_B);

    valLED6_R = 0;
    valLED6_G = 0;
    valLED6_B = 0;

    memset(ledStatus, 0, 18);
    pinMode(LED1_R, OUTPUT);
    pinMode(LED1_G, OUTPUT);
    pinMode(LED1_B, OUTPUT);
    pinMode(LED2_R, OUTPUT);
    pinMode(LED2_G, OUTPUT);
    pinMode(LED2_B, OUTPUT);
    pinMode(LED3_R, OUTPUT);
    pinMode(LED3_G, OUTPUT);
    pinMode(LED3_B, OUTPUT);
    pinMode(LED4_R, OUTPUT);
    pinMode(LED4_G, OUTPUT);
    pinMode(LED4_B, OUTPUT);
    pinMode(LED5_R, OUTPUT);
    pinMode(LED5_G, OUTPUT);
    pinMode(LED5_B, OUTPUT);
    pinMode(LED6_R, OUTPUT);
    pinMode(LED6_G, OUTPUT);
    pinMode(LED6_B, OUTPUT);
}

void LedBoard::update()
{
    // Update the software PWM RGB light
    LED6.setPWM(valLED6_R, valLED6_G, valLED6_B);
}


short LedBoard::processRequest(byte* request, byte* answer)
{
    answer[0] = request[0] | 0x80;        // reader id
    answer[1] = request[1];               //  ?
    answer[2] = request[2];               // command
    answer[3] = request[3];               // paquet id
    answer[4] = 0;                        // data length

    switch (answer[2])                   // switch on the command
    {
        //
        // get version
    case 0x02:
        answer[4] = 0x2C;
        memcpy(answer+5, getVersion(), 0x2C);
        break;

        //
        // init?
    case 0x00:
    case 0x03:
    case 0x16:
    case 0x20:
    case 0x30:
        answer[4] = 1;
        answer[5] = 0x00;
        break;

    case 0x22:  //set leds color (for jubeat ledboard)

        //byte 0-2 front RGB
        //byte 3-5 top RGB
        //byte 6-8 left RGB
        //byte 9-11 right
        //byte 12-14 title RGB
        //byte 15-17 woofer

        if (request[4] == 18)
            memcpy(ledStatus, request+5, 18); //save led colors

        analogWrite(LED1_R,ledStatus[0]);
        analogWrite(LED1_G,ledStatus[1]);
        analogWrite(LED1_B,ledStatus[2]);
        analogWrite(LED2_R,ledStatus[12]);
        analogWrite(LED2_G,ledStatus[13]);
        analogWrite(LED2_B,ledStatus[14]);
        analogWrite(LED3_R,ledStatus[3]);
        analogWrite(LED3_G,ledStatus[4]);
        analogWrite(LED3_B,ledStatus[5]);
        analogWrite(LED4_R,ledStatus[6]);
        analogWrite(LED4_G,ledStatus[7]);
        analogWrite(LED4_B,ledStatus[8]);
        analogWrite(LED5_R,ledStatus[9]);
        analogWrite(LED5_G,ledStatus[10]);
        analogWrite(LED5_B,ledStatus[11]);
        // Get the requested values for the software PWM RGB light
        valLED6_R = ledStatus[15];
        valLED6_G = ledStatus[16];
        valLED6_B = ledStatus[17];

        answer[4] = 1;
        answer[5] = 0x00;
        break;
    }
}

