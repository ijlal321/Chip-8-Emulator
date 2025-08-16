#ifndef CHIP8_H
#define CHIP8_H

#include "config.h"
#include "chip8memory.h"
#include "chip8registers.h"
#include "chip8stack.h"
#include "chip8keyboard.h"
#include "chip8screen.h"

struct chip8{
    struct chip8_memory memory; /* contains all program data. Except Stack*/
    struct chip8_registers registers;
    struct chip8_stack stack; /* remember stack holds return address from subroutines. If a is called from b, then it has address of b, which will be executed when a returns*/
    struct chip8_keyboard keyboard;
    struct chip8_screen screen;
};

void chip8_init(struct chip8* chip8);
void chip8_load(struct chip8* chip8, const char * buffer, int size);  // load the program into memory.
void chip8_exec(struct chip8* chip8, unsigned short opcode);    // perform a cpu operation


#endif