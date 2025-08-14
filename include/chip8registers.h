#ifndef CHIP8REGISTER_H
#define CHIP8REGISTER_H

#include "config.h"

struct chip8_registers{
    unsigned char V [CHIP8_TOTAL_DATA_REGISTERS];  // 16 8 bit register (General Purpose)
    unsigned short I;   // 16 bit register. Used to store memory addresses
    unsigned char delay_timer;  // 8 bit general purpose. When timer non zero, it decrement by 60hz.
    unsigned char sound_timer;  // same
    unsigned short PC;  // program counter -> current executing address
    unsigned char SP;  // stack pointer. Point to top of stack 
};

#endif