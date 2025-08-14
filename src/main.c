#include<stdio.h>
#include "SDL2/SDL.h"


int main(int argc, char *argv[]){

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window * window = SDL_CreateWindow(
        "Chip 8 Window",  
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,320,SDL_WINDOW_SHOWN
    );

    // create renderer. One who wrote everything to window (only when SDL_RenderPresent is called)
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // change (brush color) to black
    SDL_RenderClear(renderer);  // paint whole screen black (color of brush)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0); // change color to white
    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = 20;
    r.h = 20;
    SDL_RenderFillRect(renderer, &r); // make a rectangle
    SDL_RenderPresent(renderer);    // draw on screen
    SDL_Event event;

    while(SDL_PollEvent(&event)){
        if (event.type == SDL_QUIT){
            goto out;
        }

    }

out:
    SDL_DestroyWindow(window);
    return 0;
}