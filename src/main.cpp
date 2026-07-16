#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "../include/CPU.h"
#include "../include/Memory.h"

#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    // ----------------------------
    // SDL Setup
    // ----------------------------
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL Init Error: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "SNES Emulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        620,
        420,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        SDL_Log("Window Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // ----------------------------
    // Emulator Setup
    // ----------------------------
    Memory memory;
    CPU cpu(&memory);

    // Test program
    std::vector<uint8_t> program = {
        0xA9, 0x42,     // LDA #$42
        0xAA,           // TAX
        0xE8,           // INX
        0xCA,           // DEX
        0xEA            // NOP
    };

    memory.loadROM(program);

    cpu.reset();

    std::cout << "Beginning CPU test...\n";

    // Execute every instruction
    for (size_t i = 0; i < program.size(); i++) {
        cpu.step();
        cpu.printState();      // Debug output after each instruction
    }

    std::cout << "CPU test finished.\n";

    // ----------------------------
    // Keep window alive
    // ----------------------------
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}