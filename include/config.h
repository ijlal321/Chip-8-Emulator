#ifndef CONFIG_H
#define CONFIG_H

#include "config.h"

#define EMULATOR_WINDOW_TITLE "Chip 8 Window"
#define CHIP8_MEMORY_SIZE 4096
#define CHIP8_WIDTH 64
#define CHIP8_HEIGHT 32
#define WINDOW_MULTIPLIER 10


#define CHIP8_TOTAL_DATA_REGISTERS 16

#define CHIP8_TOTAL_STACK_DEPTH 16

#define CHIP8_TOTAL_KEYS 16

#define CHIP8_CHARACTER_SET_LOAD_ADDRESS 0x00  /* the character set data starts from 0 in memory (at bottom, if need visual). 
Also its on memory, not on stack. Remember these are diff in chip8. Said its better for game dev at that time. 
Stack used for handling subroutine only. its an array of size 16. Meaning max 16 nested subroutines we can go.
mem is used for all else. Program data etc.
*/

#endif