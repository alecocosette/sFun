#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "../include/CPU.h"
#include "../include/Memory.h"
#include <iostream>
int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        620, 420, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Could not create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    Memory memory;
    CPU cpu(&memory);
    std::vector<uint8_t> testROM = {
        0xA9, 0x01, // LDA #$01
        0xEA,       // NOP
        0xEA,       // NOP
        0x00        // BRK
    };
    memory.loadROM(testROM);



    cpu.reset();
    for (int i = 0; i < testROM.size(); i++) {
        cpu.step();
    }
    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
    }
    SDL_Quit();
    return 0;
}