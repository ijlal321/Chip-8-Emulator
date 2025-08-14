#include "chip8.h"
#include "chip8stack.h"
#include <assert.h>


// i think better than stack points to top of stack, not at latest entry. but lets follow dude for now
void chip8_stack_push(struct chip8 * chip8, unsigned short val){
    chip8->registers.SP += 1;
    chip8->stack.stack[chip8->registers.SP] = val;
}
unsigned short chip8_stack_pop(struct chip8 * chip8){
    unsigned short res = chip8->stack.stack[chip8->registers.SP]; 
    chip8->registers.SP -= 1;
    return res;
}
