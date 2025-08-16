#include<stdio.h>
#include "SDL2/SDL.h"
#include "chip8.h"
#include <Windows.h>

const char keyboard_map[CHIP8_TOTAL_KEYS] = {
    SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5,
    SDLK_6, SDLK_7, SDLK_8, SDLK_9, SDLK_a, SDLK_b,
    SDLK_c, SDLK_d, SDLK_e, SDLK_f
};

int main(int argc, char *argv[]){
    if (argc < 2){
        printf("You must provide a File to Load\n");
        return -1;
    }
    const char * filename = argv[1];
    FILE* f = fopen(filename, "rb");  // open in read mode.

    fseek(f,0, SEEK_END);  // moves file pointer to end of file
    long size = ftell(f);   // get cur file position -> size of file
    fseek(f,0, SEEK_SET);   // moves file pointer back to start of file

    char * buf = calloc(1, size);   // !Dont forget to free! 
    int res = fread(buf, size, 1, f);
    if (argc < 2){
        printf("Failed to read file\n");
        return -1;
    }

    struct chip8 chip8;
    chip8_init(&chip8);
    chip8.registers.PC = CHIP8_PROGRAM_LOAD_ADDRESS;
    chip8_load(&chip8, buf, size);

    free(buf);  // copied to chip8 memory in chip8_load() - no longer needed.

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window * window = SDL_CreateWindow(
        EMULATOR_WINDOW_TITLE,  
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        CHIP8_WIDTH * WINDOW_MULTIPLIER,
        CHIP8_HEIGHT * WINDOW_MULTIPLIER, SDL_WINDOW_SHOWN
    );
    // create renderer. One who wrote everything to window (only when SDL_RenderPresent is called)
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);
    
    SDL_Event event;
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // change (brush color) to black
    SDL_RenderClear(renderer);  // paint whole screen black (clear screen per frame)
    while(1){
    

        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                  goto out;
                  break;

                case SDL_KEYDOWN:
                    {
                        char key = event.key.keysym.sym;
                        int vkey = chip8_keyboard_map(keyboard_map, key);
                        if (vkey != -1){
                            chip8_keyboard_down(&chip8.keyboard, vkey);
                        }
                    }
                break;
                
                case SDL_KEYUP:
                    {
                        char key = event.key.keysym.sym;
                        int vkey = chip8_keyboard_map(keyboard_map, key);
                        if (vkey != -1){
                            chip8_keyboard_up(&chip8.keyboard, vkey);
                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // change (brush color) to black
                            SDL_RenderClear(renderer); 
                            memset(&chip8.screen, 0, sizeof(chip8.screen));
                            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0); // change (brush color) to black
                            chip8_screen_draw_sprite(&chip8.screen, 20, 20, &chip8.memory.memory[vkey*5], 5);
                            printf("printed\n");
                        }
                    }
                break;
    
            } ;
        } 
    
        // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // change (brush color) to black
        // SDL_RenderClear(renderer);  // paint whole screen black (clear screen per frame)
        
        // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0); // change color to white
        
        for (int x = 0; x < CHIP8_WIDTH; x++){
            for (int y = 0; y < CHIP8_HEIGHT; y++){
                if (chip8_screen_is_set(&chip8.screen, x, y)){
                    SDL_Rect r;
                    r.x = x * WINDOW_MULTIPLIER;
                    r.y = y * WINDOW_MULTIPLIER;
                    r.w = 10;
                    r.h = 10;
                    SDL_RenderFillRect(renderer, &r); // make a rectangle
                }
            }
        }
        SDL_RenderPresent(renderer);    // draw on screen

        // delay timer
        if (chip8.registers.delay_timer > 0){
            Sleep(100);
            printf("delayed...\n");
            chip8.registers.delay_timer -= 1;
        }

        // sound timer
        if (chip8.registers.sound_timer > 0){
            
            printf("Beeped...\n");
            chip8.registers.sound_timer = 0;
        }

        unsigned short opcode = chip8_memory_get_short(&chip8.memory, chip8.registers.PC);
        chip8_exec(&chip8, opcode);
        chip8.registers.PC += 2;
        printf("Opcode is: %x\n", opcode);
    }
    
    getchar();
out:
    SDL_DestroyWindow(window);
    return 0;
}