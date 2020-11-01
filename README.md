ACreal IO
========
ACreal IO is a firmware code for Arduino to build Konami ACIO compatible boards.

Konami ACIO is a protocol used for communication between arcade games and harware components such as e-Amusement readers.
Its uses rs232 and allows several "nodes" to be connected a single serial port.

Games support
-------------
Currently this project supports the following games :

pop'n music 15-18 (one old slotted reader)

pop'n music 19-25 (one new wavepass reader with encrypted communication (only game I know which uses this encryption) + optional card dispenser)

iidx 13-18 (2 old slotted readers)

iidx 19-20 (2 new wavepass readers)

jubeat 1 - knit (one old slotted reader (no keypad) + one led board (game won't boot if the led board is not present) 

jubeat copious - saucer (one new reader (no keypad) + one led board (game won't boot if the led board is not present) 

DDR SN-SN2 (2 old slotted readers)

DDR X-X3 (2 readers (old or new) in sd, 2 readers (old or new) + led board ? in HD)

drum mania (1 old reader)

guitar freaks (2 old readers)

Sound voltex (1 new wavepass reader + 1 IoBoard)

Nostalgia (1 IoBoard + 1 new wavepass reader)

How to use
----------

Flash the firmware on an arduino mega 2560, scratch the "reset-en" bridge on the pcb, change COM port to COM1 (COM2 for Nostalgia)

PN5180 support
--------------

Currently a single PN5180 nfc module is supported.
You can set #define RFID_MODULE1 3 in pinoutconfig to use it.

Arduino | PN5180 pin mapping:
--- | ---
5V      |      5V
3.3V    |        3.3V
GND    |         GND
SCLK,52    |  SCLK
MISO,50      |    MISO
MOSI,51    |  MOSI
SS,10   |  NSS (=Not SS -> active LOW)
BUSY,9  |         BUSY
Reset,7 |  RST

SS, BUSY and Reset pins can be changed in pinoutconfig.h