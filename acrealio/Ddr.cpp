#include "Arduino.h"
#include "Ddr.h"


//contructor
Ddr::Ddr()
{
    byte rType[] = {0x04, 0x02, 0x00, 0x00};
    byte rVersion[] = {0x01, 0x01, 0x00};
    setVersion(rType, 0x00, rVersion, "HDXB");

    //RGB LEDs
    pinMode(LED_DDR_HDXB_P1S_R, OUTPUT);
    pinMode(LED_DDR_HDXB_P1S_G, OUTPUT);
    pinMode(LED_DDR_HDXB_P1S_B, OUTPUT);
    pinMode(LED_DDR_HDXB_P2S_R, OUTPUT);
    pinMode(LED_DDR_HDXB_P2S_G, OUTPUT);
    pinMode(LED_DDR_HDXB_P2S_B, OUTPUT);
    pinMode(LED_DDR_HDXB_P1W_R, OUTPUT);
    pinMode(LED_DDR_HDXB_P1W_G, OUTPUT);
    pinMode(LED_DDR_HDXB_P1W_B, OUTPUT);
    pinMode(LED_DDR_HDXB_P2W_R, OUTPUT);
    pinMode(LED_DDR_HDXB_P2W_G, OUTPUT);
    pinMode(LED_DDR_HDXB_P2W_B, OUTPUT);

    //Button Lights
    pinMode(LT_DDR_HDXB_P1_START, OUTPUT);
    pinMode(LT_DDR_HDXB_P1_LR, OUTPUT);
    pinMode(LT_DDR_HDXB_P1_UD, OUTPUT);
    pinMode(LT_DDR_HDXB_P2_START, OUTPUT);
    pinMode(LT_DDR_HDXB_P2_LR, OUTPUT);
    pinMode(LT_DDR_HDXB_P2_UD, OUTPUT);
}

void Ddr::init()
{


}

void Ddr::update()
{
}


short Ddr::processRequest(byte* request, byte* answer)
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

    case 0x10: //ping
        answer[4] = 8;  
        answer[5]=0x45;
        answer[6]=0x00;
        answer[7]=0xdd;
        answer[8]=0x80; //only byte that changes from the write leds code reply....
        answer[9]=0xd4;
        answer[10]=0x00;
        answer[11]=0xcb;
        answer[12]=0x00;
        
    case 0x12: //write LEDs
         //request[5] is always 0, followed by GRB (yes... GRB from what I can tell...) &0x7F
         //lets multiply by 2 to get full 8 bit width for full brightness!
        analogWrite(LED_DDR_HDXB_P1S_G,((request[6]&0x7F)*2));
        analogWrite(LED_DDR_HDXB_P1S_R,((request[7]&0x7F)*2));
        analogWrite(LED_DDR_HDXB_P1S_B,((request[8]&0x7F)*2));
        analogWrite(LED_DDR_HDXB_P2S_G,((request[9]&0x7F)*2));
        analogWrite(LED_DDR_HDXB_P2S_R,((request[10]&0x7F)*2));
        analogWrite(LED_DDR_HDXB_P2S_B,((request[11]&0x7F)*2));
        analogWrite(LED_DDR_HDXB_P1W_G,((request[12]&0x7F)*2));
        analogWrite(LED_DDR_HDXB_P1W_R,((request[13]&0x7F)*2));
        analogWrite(LED_DDR_HDXB_P1W_B,((request[14]&0x7F)*2));
        analogWrite(LED_DDR_HDXB_P2W_G,((request[15]&0x7F)*2));
        analogWrite(LED_DDR_HDXB_P2W_R,((request[16]&0x7F)*2));
        analogWrite(LED_DDR_HDXB_P2W_B,((request[17]&0x7F)*2));

        //now lets do the static lights -- take the first 6 bytes and grab the high bit, if its set, then write 1, else 0
        digitalWrite(LT_DDR_HDXB_P1_START,(request[6]&0x80)>0?1:0);
        digitalWrite(LT_DDR_HDXB_P1_LR,(request[7]&0x80)>0?1:0);
        digitalWrite(LT_DDR_HDXB_P1_UD,(request[8]&0x80)>0?1:0);
        digitalWrite(LT_DDR_HDXB_P2_START,(request[9]&0x80)>0?1:0);
        digitalWrite(LT_DDR_HDXB_P2_LR,(request[10]&0x80)>0?1:0);
        digitalWrite(LT_DDR_HDXB_P2_UD,(request[11]&0x80)>0?1:0);
        
        answer[4] = 8;  
        answer[5]=0x45;
        answer[6]=0x00;
        answer[7]=0xdd;
        answer[8]=0x40;
        answer[9]=0xd4;
        answer[10]=0x00;
        answer[11]=0xcb;
        answer[12]=0x00;
        
        break;

        // init commands?
    case 0x00: //actually used here
    case 0x03: //actually used here
    case 0x16:
    case 0x20:
    case 0x28: //actually used here
    case 0x30:
        answer[4] = 1;
        answer[5] = 0x00;
        break;



    }



}

