#ifndef CONFIG_H
#define CONFIG_H

#define gametype 1               //0:pop'n with card dispenser 1: pop'n, drummania(1 reader) 2:iidx/ddr/gf(2readers) 3:jubeat (1reader+Ledboard) 4: sdvx (1reader+ioboard)

#define RFID_BAUD 115200		//Baud rate for RFID Module

#define RFID_MODULE1 2			//Rfid module used for reader1, 1:SL015M 2:RR10
#define RFID_MODULE2 2			//Rfid module used for reader1, 1:SL015M 2:RR10

#define SDVX_VOL_SENS 7        //Sensitivity for SDVX Volume buttons


//input pins for sdvx controls
#define BT_FXR A0
#define BT_FXL A1
#define BT_A A2
#define BT_B A3
#define BT_C A4
#define BT_D A5
#define BT_START 50
#define BT_TEST 52
#define BT_SVC 40

//input pins for volume encoders (phase A and phase B for each)
#define VOLR_A 42
#define VOLR_B 44
#define VOLL_A 46
#define VOLL_B 48

//pins for sdvx lights (outputs)
#define LT_START 53
#define LT_A 51
#define LT_B 49
#define LT_C 47
#define LT_D 45
#define LT_FXL 43
#define LT_FXR 41

//pins for sdvx/jubeat RGB LEDs (use PWM pins)
#define LED1_R 7
#define LED1_G 6
#define LED1_B 5

#define LED2_R 10
#define LED2_G 9
#define LED2_B 8

#define LED3_R 13
#define LED3_G 12
#define LED3_B 11




//pins for card reader 1 keypad (colls ABC, rows 1234)
/* 
### Keypad 3x4 Matrix ###

 (7)---(8)---(9)---> ROW 1
  |     |     |
 (4)---(5)---(6)---> ROW 2
  |     |     |
 (1)---(2)---(3)---> ROW 3
  |     |     |
 (0)--(00)--([])---> ROW 4
  |     |     |
  V     V     V
COL A  COL B  COL C

*/
#define K1_A 27
#define K1_B 25
#define K1_C 23
#define K1_1 29
#define K1_2 31
#define K1_3 33
#define K1_4 35


//pins for card reader 1 RFID Module
#define R1_DET 20 
#define R1_SER Serial1

//pins for card reader 2 keypad (colls ABC, rows 1234)
#define K2_A 26
#define K2_B 24
#define K2_C 22
#define K2_1 28
#define K2_2 30
#define K2_3 32
#define K2_4 34


//pins for card reader 2 RFID Module
#define R2_DET 21
#define R2_SER Serial2

#endif
