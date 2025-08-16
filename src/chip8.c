#include "chip8.h"
#include <memory.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2/SDL.h"


const unsigned char chip8_default_character_set[] = {
    0xf0, 0x90, 0x90, 0x90, 0xf0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xf0, 0x10, 0xf0, 0x80, 0xf0,
    0xf0, 0x10, 0xf0, 0x10, 0xf0,
    0x90, 0x90, 0xf0, 0x10, 0x10,
    0xf0, 0x80, 0xf0, 0x10, 0xf0,
    0xf0, 0x80, 0xf0, 0x90, 0xf0,
    0xf0, 0x10, 0x20, 0x40, 0x40,
    0xf0, 0x90, 0xf0, 0x90, 0xf0,
    0xf0, 0x90, 0xf0, 0x10, 0xf0,
    0xf0, 0x90, 0xf0, 0x90, 0x90,
    0xe0, 0x90, 0xe0, 0x90, 0xe0,
    0xf0, 0x80, 0x80, 0x80, 0xf0,
    0xe0, 0x90, 0x90, 0x90, 0xe0,
    0xf0, 0x80, 0xf0, 0x80, 0xf0,
    0xf0, 0x80, 0xf0, 0x80, 0x80};

void chip8_init(struct chip8 *chip8)
{
    memset(chip8, 0, sizeof(struct chip8));

    // copy characterset in memory bottom as per original design.
    memcpy(&chip8->memory.memory, chip8_default_character_set, sizeof(chip8_default_character_set));
}

void chip8_load(struct chip8 *chip8, const char *buffer, int size)
{
    assert(size + CHIP8_PROGRAM_LOAD_ADDRESS < CHIP8_MEMORY_SIZE);
    memcpy(&chip8->memory.memory[CHIP8_PROGRAM_LOAD_ADDRESS], buffer, size);
    chip8->registers.PC = CHIP8_PROGRAM_LOAD_ADDRESS;
}

void chip8_execute_entended_eight(struct chip8 *chip8, unsigned short opcode){
    unsigned char x = (opcode >> 8) & 0x0f;   // x gives index of V register we need to compare to kk
    unsigned char y = (opcode & 0x00f0) >> 4; // or (opcode >> 4) & 0x0f
    unsigned short tmp = 0;

    unsigned char last_four_bits = opcode & 0x000f;
    switch (last_four_bits)
    {
    // 8xy0
    case (0x00):
        chip8->registers.V[x] = chip8->registers.V[x];
        break;

    // 8xy1 - Vx = Vx | Vy
    case (0x01):
        chip8->registers.V[x] |= chip8->registers.V[y];
        break;

    case (0x02):
        chip8->registers.V[x] = chip8->registers.V[x] && chip8->registers.V[y];
        break;

    case (0x03):
        chip8->registers.V[x] = chip8->registers.V[x] ^ chip8->registers.V[y];
        break;

    // vx = vx + vy. Add carry to V[f], f is 0x0f register. (f is carry flag btw)
    case (0x04):
        {
        tmp = chip8->registers.V[x] + chip8->registers.V[y];
        chip8->registers.V[0x0f] = 0;
        if (tmp > 0xff)
        {
            chip8->registers.V[0x0f] = 1;
        }
        chip8->registers.V[x] = tmp;
    }
        break;

    // vx = vx - vy. Set V[f] = Not Borrow
    case (0x05):
        chip8->registers.V[0x0f] = 0; // necessary due to garbage Value.
        if (chip8->registers.V[x] >= chip8->registers.V[y])
        {
            chip8->registers.V[0x0f] = 1; // set carry flag
        }
        chip8->registers.V[x] = chip8->registers.V[x] - chip8->registers.V[y];
        break;

    // if LSB of VX is 1, then set V[f]
    // Shift Right Vx , 1 time (divided by 2)
    case (0x06):
        chip8->registers.V[0x0f] = chip8->registers.V[x] & 1 == 1 ? 1 : 0;
        // chip8->registers.V[x] /= 2; // also true
        chip8->registers.V[x] = chip8->registers.V[x] >> 1;

        break;

    // Vx = Vy - Vx, set V[f] = Not Borrow
    case (0x07):
        chip8->registers.V[0x0f] = chip8->registers.V[y] >= chip8->registers.V[x];
        chip8->registers.V[x] = chip8->registers.V[y] - chip8->registers.V[x];
        break;

    // 8xyE ->  V[f] = MSB 1
    // Multiply Vx by 2
    case (0x0E):
        // chip8->registers.V[0x0f] = (chip8->registers.V[x] >> 7) & 1; // 1 liner
        chip8->registers.V[0x0f] = 0;
        if ((chip8->registers.V[x] >> 7) == 1)
        {
            chip8->registers.V[0x0f] = 1;
        }
        // chip8->registers.V[x] *= 2;
        chip8->registers.V[x] <<= 1;
        break;
    }
}

static char chip8_wait_for_key_press(struct chip8 *chip8)
{
    SDL_Event event;
    while (SDL_WaitEvent(&event))
    {
        if (event.type != SDL_KEYDOWN)
            continue;
        char c = event.key.keysym.sym;
        char chip8_key = chip8_keyboard_map(&chip8->keyboard, c);
        if (chip8_key != -1)
        {
            return chip8_key;
        }
    }
    return -1;
}

static void chip8_exec_extended_F(struct chip8* chip8, unsigned short opcode)
{
    unsigned char x = (opcode >> 8) & 0x000f;
    switch (opcode & 0x00ff)
    {
        // fx07 - LD Vx, DT. Set Vx to the delay timer value
        case 0x07:
            chip8->registers.V[x] = chip8->registers.delay_timer;
        break;

        // fx0a - LD Vx, K
        case 0x0A:
        {
            char pressed_key = chip8_wait_for_key_press(chip8);
            chip8->registers.V[x] = pressed_key;
        }
        break; 

        // fx15 - LD DT, Vx, set the delay timer to Vx
        case 0x15:
            chip8->registers.delay_timer = chip8->registers.V[x];
        break;

        // fx18 - LD ST, Vx, set the sound timer to Vx
        case 0x18:
            chip8->registers.sound_timer = chip8->registers.V[x];
        break;


        // fx1e - Add I, Vx
        case 0x1e:
            chip8->registers.I += chip8->registers.V[x];
        break;

        // fx29 - LD F, Vx
        case 0x29:
            chip8->registers.I = chip8->registers.V[x] * CHIP8_DEFAULT_SPRITE_SIZE;
        break;

        // fx33 - LD B, Vx
        case 0x33:
        {
            unsigned char hundreds = chip8->registers.V[x] / 100;
            unsigned char tens = chip8->registers.V[x] / 10 % 10;
            unsigned char units = chip8->registers.V[x] % 10;
            chip8_memory_set(&chip8->memory, chip8->registers.I, hundreds);
            chip8_memory_set(&chip8->memory, chip8->registers.I+1, tens);
            chip8_memory_set(&chip8->memory, chip8->registers.I+2, units);
        }
        break;

        // fx55 - LD [I], Vx
        case 0x55:
        {
            for (int i = 0; i <= x; i++)
            {
                chip8_memory_set(&chip8->memory, chip8->registers.I+i, chip8->registers.V[i]);
            }
        }
        break;

        // fx65 - LD Vx, [I]
        case 0x65:
        {
            for (int i = 0; i <= x; i++)
            {
                chip8->registers.V[i] = chip8_memory_get(&chip8->memory, chip8->registers.I+i);
            }
        }
        break;

    }
}

void chip8_exec_extended(struct chip8 *chip8, unsigned short opcode)
{
    unsigned short nnn = opcode & 0x0fff;
    unsigned char x = (opcode >> 8) & 0x0f;   // x gives index of V register we need to compare to kk
    unsigned char y = (opcode & 0x00f0) >> 4; // or (opcode >> 4) & 0x0f
    unsigned char kk = opcode & 0x00ff;     // compare V[x] == kk
    unsigned char n = opcode & 0x000f;

    switch (opcode & 0xf000)
    {
    // jmp addr, 1nnn jump to nnn's
    case 0x1000:
        chip8->registers.PC = nnn;
        break;

    // CALL addr, 2nnn call subroutine at location nnn. Like calling a fn.
    case 0x2000:
        chip8_stack_push(chip8, chip8->registers.PC); // push cur on stack
        chip8->registers.PC = nnn;                    // start new instruction
        break;
    /* remember stack holds return address from subroutines. If a is called from b, then it has address of b, which will be executed when a returns*/

    // SE Vx, bye; skip next instruction if V[x] == kk
    case 0x3000:
        if (chip8->registers.V[x] == chip8->registers.V[y])
        {
            chip8->registers.PC += 2;
        }
        break;

    // SE Vx, bye; skip next instruction if V[x] != kk
    case 0x4000:
        if (chip8->registers.V[x] != kk)
        {
            chip8->registers.PC += 2;
        }
        break;

    // skip next instruction if V[x] != V[y]
    case 0x5000:
        printf("x: 0x%02x \n", x);
        printf("y: 0x%02x \n", y);
        if (chip8->registers.V[x] != kk)
        {
            chip8->registers.PC += 2;
        }
        break;

    // 6xkk, loads value kk in register x
    case 0x6000:
        chip8->registers.V[x] = kk;
        break;

    // 7xkk, add kk to V[x]
    case 0x7000:
        chip8->registers.V[x] += kk;
        break;

    case 0x8000:
    {
        chip8_execute_entended_eight(chip8, opcode);
    }
    break;

    // 9xyz, skip next instruction if vx != vy
    case 0x9000:
        if (chip8->registers.V[x] != chip8->registers.V[y])
        {
            chip8->registers.PC += 2;
        }
        break;

    // sets i reigster to nnn
    // I register used for sprites btw.
    case 0xA000:
        chip8->registers.I = nnn;
        break;

    // jmp to location nnn+V0
    case 0xB000:
        chip8->registers.PC = chip8->registers.V[0] + nnn;
        break;

    // yes, chip 8 has ability to generate random numbers, why BCZ ITS A GAME DUDE.
    // NEED TO KEEP THINGS INTRESTED.
    // Vx = generated random no from 0 to 255 and && with kk
    // CXkk
    case 0xC000:{
        srand(clock());
        chip8->registers.V[x] = (rand() % 256) && kk;
        }
        break;

    // Dxyn - Draw n bytes at Vx,Vy starting from address at register I. Store in V[f] if collision
    case 0xD000:
    {
        const char *sprite = (const char *)&chip8->memory.memory[chip8->registers.I];
        int is_collision = chip8_screen_draw_sprite(&chip8->screen, chip8->registers.V[x], chip8->registers.V[y], sprite, n);
        chip8->registers.V[0x0f] = is_collision;
    }
        break;

    // keyboard ops
    case 0xE000:
    {
        // Ex9E - skip nezt ins if key with value v[x] is pressed
        switch (opcode & 0x00ff)
        {
        case 0x9e:
            if (chip8_keyboard_is_down(&chip8->keyboard, chip8->registers.V[x]))
            {
                chip8->registers.PC += 2;
            }
            break;
        case 0xA1: // skip if key not down
            if (!chip8_keyboard_is_down(&chip8->keyboard, chip8->registers.V[x]))
            {
                chip8->registers.PC += 2;
            }
            break;
        }
    }
    break;

    case 0xF000:
        chip8_exec_extended_F(chip8, opcode);
        break;
    }
}

void chip8_exec(struct chip8 *chip8, unsigned short opcode)
{
    switch (opcode)
    {
    case 0x00E0:
        chip8_clear_screen(&chip8->screen);
        break;

    case 0x00EE:
        chip8->registers.PC = chip8_stack_pop(chip8);
        break;

    default:
        chip8_exec_extended(chip8, opcode);
    }
}