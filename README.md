# CHIPU - A Chip8 Emulator!

Since 1977 the [Chip8](https://en.wikipedia.org/wiki/CHIP-8) platform has provided an outlet for programming creativity. To understand the nitty gritty of computters and to get into **low level programming** - Chip8 emulators have often served as safe Haven for many Young Aspiring Programmers! This repository is meant to serve a well documented example of Chip8 emulator - so If you someone is intrested in Chip8 or emulators in general, start your journey here!

## Quick Disclaimer
* This Porject is based on [Cowgod's Chip-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM). Its a must read to understand underlying artitecture. In code and here as well, we will be refrencing this Doc alot. Thanks CowGod Once again !

* A Free game **INVADERS** has been added in code as an example. This game is part of **Public domain** by creators. I do not own anything in that game!

# Table of Contents

- [CHIPU - A Chip8 Emulator!](#chipu---a-chip8-emulator)
  - [Quick Disclaimer](#quick-disclaimer)
- [Chip8](#chip8)
- [Why the Chip8 Emulator](#why-the-chip8-emulator)
- [Artitecture](#artitecture)
  - [Memory](#memory)
  - [Registers](#registers)
  - [Delay Timer & Sound Timer](#delay-timer--sound-timer)
  - [Stack](#stack)
  - [Keyboard & Display](#keyboard--display)
  - [SDL Library](#sdl-library)
- [Recommmended Order of Coding](#recommmended-order-of-coding)
- [The not-so-Scary Chip-8 Instructions](#The-not-so-Scary-Chip-8-Instructions)
- [How to Use](#how-to-use)
- [Dont forget the Config File](#dont-forget-the-config-file)
- [License](#License)


# Chip8

**CHIP-8** is an **interpreted programming language** designed in the mid-1970s by Joseph Weisbecker. It was mainly used on early 8-bit microcomputers like the COSMAC VIP and Telmac 1800. CHIP-8 allowed programmers to create simple video games more easily by providing a set of instructions that the computer could interpret directly. It runs on a **virtual machine** and uses a limited set of opcodes, making it easy to emulate and ideal for beginners learning about low-level systems and emulator development.

# Why the Chip8 Emulator

CHIP-8 is a programming language, and a CHIP-8 emulator isn’t really a thing. When someone refers to a CHIP-8 emulator, they really mean an interpreter for the language.

The reason CHIP-8 interpreters get called emulators is because the process of building one is very similar to the process of building a (very simple) emulator. Since the CHIP-8 language was made as a platform for developing video games on some early computers, it is an assembly-like language and requires input handling and display updating. These are some of the basic building blocks you’ll need to be familiar with if you try to write an emulator for a video game system, so a CHIP-8 interpreter is often an entry-point into emulator development.

# Artitecture
Again, **CowGod's** techinical refrence to Chip8 has done it best in my opinion. Go read it if you want to understand Chip8 better. Here are some main points you absolutely must be aware of.

### Memory
Chip8 has about **4kb** of ram - In which **first 512 bytes** are where the original interpreter is. This section cannot be used by programs. 

### Registers
A chip8 has
* 16 general purpose 8-bit registers.
* A 16-bit register called I.
* A flag resgister VF.
* A PC program counter to hold address of next instruction.
* A SP stack pointer that points top of stack.

### Delay Timer & Sound Timer
As name suggest, they will delay program (Kinda like sleep) and beep during program execution if they are non zero. hey are automatically decremented at a rate of **60Hz**.

### Stack
Unlike **Unix Like Systems** where stack is on memory. Chip8 has a different space for stacks. Mainly in the form of an **array** of **16 16-bit values**, used to store the address that the interpreter shoud return to when finished with a subroutine.

### Keyboard & Display
The original Chip8 keyboard has **16 keys** from 0 to F. In code, we will map our keyboard keys to these 16 keys.
```c
const  char  keyboard_map[CHIP8_TOTAL_KEYS] = {
	SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5,
	SDLK_6, SDLK_7, SDLK_8, SDLK_9, SDLK_a, SDLK_b,
	SDLK_c, SDLK_d, SDLK_e, SDLK_f
};
// SDL_x means x key is pressed on keyboard.
```
e.g if 5 is pressed on keyboard, it will invoke SDLK_5, meaning key 5 is activated on Chip8.

The original implementation of the Chip-8 language used a **64x32-pixel** monochrome display - meaning its only black and white. 
As this is such a small display, we will scale it using a multiplier to make it bigger on our screen.

## SDL Library
For handling displaying pixel on screen, we will be using some 3rd party library. Mainly SDL2. 

**For easier experiece and  more streamlined followup of this course, I have added a Mingw-Compatible version of SLD2 already in code - So you dont have to worry about library management for now if you are a beginner. Ofcourse feel free to tweek setup per your specification!**

# Recommmended Order of Coding
So you wanna code a chip8 emulator, but dont know where to start. Here is whats best order to make this chip in my own very personal and baised opinion. Enjoy !

1. Set up **SDL Library** in your code or Environment.
2. **Structure your project** (create bin, include, lib, src, Makefile) unless your IDE does that for you.
3. Start with **screen**. Try printing a simple Rectangle on screen.
4. Implement memory, refer **chip8memory.h** to know what to implement.
5.  Implement registers, refer **chip8register.h** for better understanding.
6. Implement stack, refer **chip8stack.h** if feel stuck - you Got it !
7. Create Keyboard - Map Physical Buttons to virutal buttons
8. Create the Display for Screen (Dont forget to Scale using a Multiplier) - **chip8screen.h**.
9. Create abstraction for creating **Sprites** on Screen.
10. Implement Display & Sound **timers**.
11. Implement the Chip8 **Instruction Set** -- (Most Important !)

# The not-so-Scary Chip-8 Instructions
Unarguably, this can be the most difficult - but most rewarding section of this project. Once you have refered the **CowGod's** guide to know what instrctions Chip8 provide. Make sure you have a good grasp on **Bitwise Operations** because we will be using them alot here. Lets implement them! 

Also its a good idea to first initialize all important naunces here. Refer docs if you dont understand what they are
```c
unsigned  short  nnn  =  opcode  &  0x0fff;
unsigned  char  x  = (opcode  >>  8) &  0x0f; to kk
unsigned  char  y  = (opcode  &  0x00f0) >>  4; // or (opcode >> 4) & 0x0f
unsigned  char  kk  = (opcode  &  0x00ff); 
unsigned  char  n  =  opcode  &  0x000f;
```

Refer **chip8.h** to see full implementation of instruction sets. Here is but a few important example

```c
switch (opcode)
{
	case  0x00E0:
		chip8_clear_screen(&chip8->screen);
	break;

	case  0x00EE:
		chip8->registers.PC  =  chip8_stack_pop(chip8);
	break;
...
	// jmp addr, 1nnn jump to nnn's

	case  0x1000:
		chip8->registers.PC  =  nnn;
	break;

	// CALL addr, 2nnn call subroutine at location nnn. Like calling a fn.
	case  0x2000:
		chip8_stack_push(chip8, chip8->registers.PC); // push cur on stack
		chip8->registers.PC  =  nnn; // start new instruction
	break;
	/* remember stack holds return address from subroutines. If a is called from b, then it has address of b, which will be executed when a returns*/


	// SE Vx, bye; skip next instruction if V[x] == kk
	case  0x3000:
		if (chip8->registers.V[x] ==  kk){
			chip8->registers.PC  +=  2;
		}
	break;
...
}
```
  
# How to Use
```bash
git clone https://github.com/ijlal321/Chip-8-Emulator
cd Chip-8-Emulator
```

If you are on windows or Mingw Compiler, you can do 
```bash
Make 
```
if not, make sure you got your SDL Library with you.

```bash
make run
```

This will Open Space invaders game in new terminal.

# Dont forget the Config File
You dont have to wait to be professional to write good code. You write good code to be professional. In such spirits always make a config file such as this.

```c
#ifndef CONFIG_H
#define  CONFIG_H
		
		// Screen 
#define  EMULATOR_WINDOW_TITLE  "Chip 8 Window"

#define  CHIP8_WIDTH  64

#define  CHIP8_HEIGHT  32

#define  WINDOW_MULTIPLIER  10

		// Memory
#define  CHIP8_MEMORY_SIZE  4096

// In memory, program data can be loaded after this position. (first 0x200 are for characterset, remember).
#define  CHIP8_PROGRAM_LOAD_ADDRESS  0x200
		// Registers
#define  CHIP8_TOTAL_DATA_REGISTERS  16
		// Stack
#define  CHIP8_TOTAL_STACK_DEPTH  16
		// Keyboard
#define  CHIP8_TOTAL_KEYS  16

#define  CHIP8_CHARACTER_SET_LOAD_ADDRESS  0x00
		
#define  CHIP8_DEFAULT_SPRITE_SIZE  5

#endif
```

## License
This project is licensed under the [MIT License](./LICENSE).


