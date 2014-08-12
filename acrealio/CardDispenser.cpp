#include "Arduino.h"
#include "CardDispenser.h"


//contructor
CardDispenser::CardDispenser(char* rCode)
{
	setVersion((byte[]) {0x04, 0x00, 0x00, 0x01}, 0x00, (byte[]) {0x00, 0x00, 0x07}, rCode);
	//buttons
	pinMode(BB_COIN, INPUT);
	digitalWrite(BB_COIN,HIGH);
	pinMode(BB_SERVICE, INPUT);
	digitalWrite(BB_SERVICE,HIGH);
	pinMode(BB_TEST, INPUT);
	digitalWrite(BB_TEST,HIGH);
	pinMode(BB_P1_RED, INPUT);
	digitalWrite(BB_P1_RED,HIGH);
	pinMode(BB_P1_GREEN, INPUT);
	digitalWrite(BB_P1_GREEN,HIGH);
	pinMode(BB_P1_BLUE, INPUT);
	digitalWrite(BB_P1_BLUE,HIGH);
	pinMode(BB_P1_YELLOW, INPUT);
	digitalWrite(BB_P1_YELLOW,HIGH);
	pinMode(BB_P2_RED, INPUT);
	digitalWrite(BB_P2_RED,HIGH);
	pinMode(BB_P2_GREEN, INPUT);
	digitalWrite(BB_P2_GREEN,HIGH);
	pinMode(BB_P2_BLUE, INPUT);
	digitalWrite(BB_P2_BLUE,HIGH);
	pinMode(BB_P2_YELLOW, INPUT);
	digitalWrite(BB_P2_YELLOW,HIGH);
	pinMode(BB_P3_RED, INPUT);
	digitalWrite(BB_P3_RED,HIGH);
	pinMode(BB_P3_GREEN, INPUT);
	digitalWrite(BB_P3_GREEN,HIGH);
	pinMode(BB_P3_BLUE, INPUT);
	digitalWrite(BB_P3_BLUE,HIGH);
	pinMode(BB_P3_YELLOW, INPUT);
	digitalWrite(BB_P3_YELLOW,HIGH);
	pinMode(LED1_R, OUTPUT);
	//lights
	pinMode(BB_P1_LT_RED, OUTPUT);
	pinMode(BB_P1_LT_GREEN, OUTPUT);
	pinMode(BB_P1_LT_BLUE, OUTPUT);
	pinMode(BB_P1_LT_YELLOW, OUTPUT);
	pinMode(BB_P2_LT_RED, OUTPUT);
	pinMode(BB_P2_LT_GREEN, OUTPUT);
	pinMode(BB_P2_LT_BLUE, OUTPUT);
	pinMode(BB_P2_LT_YELLOW, OUTPUT);
	pinMode(BB_P3_LT_RED, OUTPUT);
	pinMode(BB_P3_LT_GREEN, OUTPUT);
	pinMode(BB_P3_LT_BLUE, OUTPUT);
	pinMode(BB_P3_LT_YELLOW, OUTPUT);

}

void CardDispenser::init()
{

}

void CardDispenser::update()
{
	
}


short CardDispenser::processRequest(byte* request, byte* answer)
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
	  
	case 0x72://card dispenser status
	//first byte : status
	// 1 : busy
	// 2 : ok
	// 3 : empty
	// 4 : payout
	// 5 : jam
	// 6 : maintenance
	// second byte : disburbed finished?
	// 6 - 9 : ok
	// 10 11: ?? maintenance lock
	// 12 - 17 : busy, crash ingame
	
	answer[4] = 2;
	answer[5] = 0x02;
	answer[6] = 0x06;
	break;   


	case 0x13:
		answer[4] = 0x0F;
		
		//lights commands
		//byte 3 (request[7])
		// 0x01 P1 RED
		// 0x02 P1 GREEN
		// 0x04 P1 BLUE
		// 0x08 P1 YELLOW
		// 0x10 P2 RED
		// 0x20 P2 GREEN
		// 0x40 P2 BLUE
		// 0x80 P2 YELLOW     
		//byte 4 (request[8])
		// 0x01 P3 RED
		// 0x02 P3 GREEN
		// 0x04 P3 BLUE
		// 0x08 P3 YELLOW
		

		digitalWrite(BB_P1_LT_RED,request[7] & 0x01);
		digitalWrite(BB_P1_LT_GREEN,request[7] & 0x02);
		digitalWrite(BB_P1_LT_BLUE,request[7] & 0x04);
		digitalWrite(BB_P1_LT_YELLOW,request[7] & 0x08);
		digitalWrite(BB_P2_LT_RED,request[7] & 0x10);
		digitalWrite(BB_P2_LT_GREEN,request[7] & 0x20);
		digitalWrite(BB_P2_LT_BLUE,request[7] & 0x40);
		digitalWrite(BB_P2_LT_YELLOW,request[7] & 0x80);		
		digitalWrite(BB_P3_LT_RED,request[8] & 0x01);
		digitalWrite(BB_P3_LT_GREEN,request[8] & 0x02);
		digitalWrite(BB_P3_LT_BLUE,request[8] & 0x04);
		digitalWrite(BB_P3_LT_YELLOW,request[8] & 0x08);		
		
	   //status buffer default          
		answer[5] = 0x80;
		answer[6] = 0x00;
		answer[7] = 0x80;
		answer[8] = 0x00;
		answer[9] = 0x80;
		answer[10] = 0x00;
		answer[11] = 0x80;
		answer[12] = 0x00;
		answer[13] = 0x00;
		answer[14] = 0x00;
		answer[15] = 0x00;
		answer[16] = 0x00;
		answer[17] = 0x00;
		answer[18] = 0x00;
		answer[19] = 0x00;
		
		
		//byte2 (answer[6])
		// 0x04 coin
		// 0x10 service
		// 0x20 test
		
		//byte 9 (answer[13])
		// 0x01 P1 RED
		// 0x02 P1 GREEN
		// 0x04 P1 BLUE
		// 0x08 P1 YELLOW
		// 0x10 P2 RED
		// 0x20 P2 GREEN
		// 0x40 P2 BLUE
		// 0x80 P2 YELLOW        
		
		//P3 commands (0x00 : pressed, 0x80 :unpressed)
		// byte 1 P3 RED
		// byte 3 P3 GREEN
		// byte 5 P3 BLUE
		// byte 7 P3 YELLOW
						
		if(!digitalRead(BB_P1_RED))
			answer[13] |= 0x01;
		if(!digitalRead(BB_P1_GREEN))
			answer[13] |= 0x02;
		if(!digitalRead(BB_P1_BLUE))
			answer[13] |= 0x04;
		if(!digitalRead(BB_P1_YELLOW))
			answer[13] |= 0x08;
		if(!digitalRead(BB_P2_RED))
			answer[13] |= 0x10;
		if(!digitalRead(BB_P2_GREEN))
			answer[13] |= 0x20;
		if(!digitalRead(BB_P2_BLUE))
			answer[13] |= 0x40;
		if(!digitalRead(BB_P2_YELLOW))
			answer[13] |= 0x80;
		if(!digitalRead(BB_P3_RED))
			answer[5] = 0x00;
		if(!digitalRead(BB_P3_GREEN))
			answer[7] = 0x00;
		if(!digitalRead(BB_P3_BLUE))
			answer[9] = 0x00;
		if(!digitalRead(BB_P3_YELLOW))
			answer[11] = 0x00;
		if(!digitalRead(BB_COIN))
			answer[6] |= 0x04;   //coin
		if(!digitalRead(BB_TEST))
			answer[6] |= 0x20;   //test
		if(!digitalRead(BB_SERVICE))
			answer[6] |= 0x10;//service   
		
		break;
		
	case 0x40://check secplug ?
	answer[4] = 0;
	answer[5] = 0x00;
	break;

	case 0x48://check secplug ? security key ?
	answer[4] = 0x21;
	answer[5] = 0x00;//needs to be 0 first (may be version/materiel key)
		answer[6] = 'G';
		answer[7] = 'Q';
		answer[8] = 'I';
		answer[9] = 'B';
		answer[10] = 'B';
		answer[11] = 'A';
		answer[12] = 'A';
		answer[13] = 'A';
		memset(answer+14,0xBB,0x18);
	break;  
		
	
	case 0x71://card dispenser, dispense command
	answer[4] = 1;
	answer[5] = 0x00;
	break;   
	  
	
	  
	}



}
