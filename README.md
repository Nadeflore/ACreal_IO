ACreal IO
========
ACreal IO is a firmware code for Arduino to build Konami ACIO compatible boards.

Konami ACIO is a protocol used for communication between arcade games and harware components such as e-Amusement readers.
Its uses rs232 and allows several "nodes" to be connected a single serial port.

Games support
-------------
Currently this project supports the following games :

pop'n music 15-18 (one old slotted reader)

pop'n music 19-21 (one new wavepass reader with encrypted communication (only game I know which uses this encryption) + optional card dispenser)

iidx 13-18 (2 old slotted readers)

iidx 19-20 (2 new wavepass readers)

jubeat 1 - knit (one old slotted reader (no keypad) + one led board (game won't boot if the led board is not present) 

jubeat copious - saucer (one new reader (no keypad) + one led board (game won't boot if the led board is not present) 

DDR SN-SN2 (2 old slotted readers)

DDR X-X3 (2 readers (old or new) in sd, 2 readers (old or new) + HDXB button board and RGB speakers lights in HD)

drum mania (1 old reader)

guitar freaks (2 old readers)

Sound voltex (1 new wavepass reader + 1 IoBoard )