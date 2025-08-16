#include "chip8.h"
#include "chip8stack.h"
#include <assert.h>

static void chip8_stack_in_bounds(struct chip8 * chip8){
    assert(chip8->registers.SP >= 0 && chip8->registers.SP < sizeof(chip8->stack.stack));
}

// we diverted from tutorial here, beacuse he assumes stack is -1 and first increment it, then do something.
void chip8_stack_push(struct chip8 * chip8, unsigned short val){
    chip8->stack.stack[chip8->registers.SP] = val;
    chip8->registers.SP += 1;
    chip8_stack_in_bounds(chip8);
}
unsigned short chip8_stack_pop(struct chip8 * chip8){
    chip8->registers.SP -= 1;
    unsigned short res = chip8->stack.stack[chip8->registers.SP]; 
    chip8_stack_in_bounds(chip8);
    return res;
}
