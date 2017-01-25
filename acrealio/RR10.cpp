#include "Arduino.h"
#include "RR10.h"

RR10::RR10()
{
    card = 0;
    readcmd = false;
    comstatus = 0;
    rf_i = 0;
    pinset = false;
    incmd = false;
    readstatus = 0;
}

void RR10::setPins(int sensor, HardwareSerial* serialid)
{
    rfSerial = serialid;
    rfSerial->begin(115200); //the only possible baud rate for this module

    pinset=true;

}

void RR10::update()
{
    if (!pinset)
        return;
    if (!readcmd)
        return;


    switch (readstatus)
    {
    case 0:
    {
        //let's read ISO15693 first

        byte cmd[7] = {0x07,0x06,0x00,0x00,0x00}; //command 0x06 : ISO15693 Tag Inventory (params : normal mode, no AFI)
        sendCmd(cmd);

        if ( cmdUpdate() )//when reading is finished
        {

            if (rfidp[2] != 0)//tag found
            {

                //at least one tag is found, let's read the uid of the first tag (extra tags are ignored)
                for (int i=0;i<8;i++)
                {
                    uid[i] = rfidp[11-i];
                }


                if (uid[0] == 0xE0 && uid[1] == 0x04) // if correct konami card, reading is done, don't every bother to check for Felica
                {
                    card = 1;
                    readcmd = false;
                    readstatus = 0;
                }
                else
                {
                    readstatus = 1;
                }

            }
            else
            {
                readstatus = 1;
            }

        }

        break;


    }


    case 1:
    {
        //no ISO15696 found, let's try to find some FeliCa instead

        byte cmd[4] = {0x04,0x0E,0x00,0x0A}; //command 0x0E : FeliCa Tag Inventory
        sendCmd(cmd);

        if ( cmdUpdate() )//when reading is finished
        {

            if (rfidp[2] != 0)//tag found
            {

                //at least one tag is found, let's read the uid of the first tag (extra tags are ignored)
                for (int i=0;i<8;i++)
                {
                    uid[i] = rfidp[3+i];
                }

                card = 2;
                readcmd = false;
                readstatus = 0;


            }
            else
            {
                card = 0;
                readcmd = false;
                readstatus = 0;
            }

        }

        break;


    }



    }







}

byte RR10::isCardPresent()
{
    return card;
}

void RR10::read()
{
    readcmd = true;
}

void RR10::getUID(byte* uida)
{
    memcpy(uida,uid,8);
}


void RR10::sendCmd(byte* cmd)
{
    if (incmd)
        return;

    memcpy(command,cmd,cmd[0] - 2);//store command

    //compute checksum
    word chksm = 0;
    for (int i=0;i<command[0]-2;i++)
        chksm += command[i];
    command[command[0]-2]=chksm;
    command[command[0]-1]=chksm>>8;

    incmd = true;

}

//perform next step of the cmd, return true if cmd is finished
boolean RR10::cmdUpdate()
{
    if (!incmd)
        return false;

    switch (comstatus)
    {
    case 0://nothing sent yet, need to send handshake
        rf_i = 0;
        rfSerial->write(0x55);
        timesent = millis();
        comstatus = 1;
        break;
    case 1://handshake sent, waiting for ack back, then send request
        if (rfSerial->available() == 0 ) //nothing yet
            break;

        if (rfSerial->read()!=0xAA) // incorrect answer
        {
            comstatus = 0; //let's try again from the beginning
            break;
        }


        rfSerial->write(command,command[0]); // send command to serial port


        comstatus = 2;
        break;
    case 2: // Command sent, waiting for handshake, then send hanshake back
        if (rfSerial->available() == 0 ) //nothing yet
            break;

        if (rfSerial->read()!=0xA5) // incorrect answer
        {
            comstatus = 0; //let's try again from the beginning
            break;
        }

        rfSerial->write(0x5A); //send handshake back

        comstatus = 3;
        break;

    case 3: // Handshake sent back, waiting for answer
        if (rfSerial->available() == 0 ) //nothing yet
            break;

        rfidp[rf_i] = rfSerial->read(); //add received data to the buffer
        rf_i++;

        if (rf_i < rfidp[0])//response not fully received yet
            break;

        //response has been fully received, let's check it

        {//compute checksum
            word chksm=0;
            for (int i=0;i<rfidp[0]-2;i++)
                chksm += rfidp[i];

            if (chksm != ((((word)rfidp[rfidp[0]-1])<<8) + rfidp[rfidp[0]-2]) ) //if checksum mismatch
            {
                comstatus = 0; //let's try again from the beginning
                break;
            }
        }

        //everythings went fine, cmd was sucessful

        comstatus = 0;
        incmd = false;
        return true;
    }

    if (millis() - timesent >1000) //timeout, let's try again
        comstatus = 0;

    return false;

}





