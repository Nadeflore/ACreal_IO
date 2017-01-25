#include "Arduino.h"
#include "Cipher.h"

void Cipher::setKeys(unsigned long reckey, unsigned long mykey)
{
    //set initial key array (array of 4 32bits key)
    keyarray[0] = mykey ^ 88675123;
    keyarray[1] = reckey  ^ 521288629; //second key : received XOR 521288629
    keyarray[2] = mykey ^ 362436069; // third key : cryptkey XOR random?
    keyarray[3] = reckey  ^ 123456789; // fourth key : received XOR random?
}

unsigned short Cipher::CRCCCITT(unsigned char *data, unsigned int length)
{

    int count;
    unsigned long crc = 0;
    unsigned long temp;

    for (count = 0; count < length; count++)
    {
        temp = (data[count] ^ (crc >> 8)) & 0xff;
        crc = crc_table[temp] ^ (crc << 8);
    }

    return (unsigned int)crc;

}


void Cipher::encrypt(unsigned char* data, unsigned int length)
{
    int i = 0;
    if (length > 0)
    {
        do
        {
            int ilow = i & 3;                       // 2lower bits of i

            if (ilow == 0)                          // shiftkeys every 4bytes
            {
                unsigned long key1 = keyarray[0];
                unsigned long key4 = keyarray[3];
                unsigned long key4new = (key4 << 11) ^ key4;
                keyarray [3] = keyarray[2];
                keyarray [2] = keyarray [1];
                keyarray [1] = key1;
                keyarray [0] = ((((key1 >> 11) ^ key4new) >> 8) ^ key4new ^ key1);      // new key
            }

            // xor with key
            data[i] = (unsigned char) ( keyarray[0] >> ((3 - ilow) << 3) ^ data[i] );
            i++;

        }
        while (i < length);
    }
}


