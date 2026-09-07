#include <iostream>
#include <vector>
#include "TestHarness.h"
#include "../include/CPU.h"
#include "../include/Memory.h"
#include "../include/Bus.h"

int main(int argc, char* argv[]) {
    // If argument provided, run JSON tests
    if (argc > 1) {
        std::string testPath = argv[1];
        return runTestSuite(testPath);
    }
    Bus bus;
    CPU cpu(&bus);
    // Otherwise, run manual bytecode test (for debugging)
    Memory memory;

    cpu.PC = 0x0000;
    cpu.PB = 0x7E;
    cpu.DB = 0x7E;
    cpu.A = 0;
    cpu.X = 0;
    cpu.Y = 0;
    cpu.SP = 0x01FF;
    cpu.P = 0x34;
    cpu.DP = 0;
    cpu.emulation = true;

    std::vector<uint8_t> program = {
        0xA9, 0x42,  // LDA #$42
        0xAA,        // TAX
        0xE8,        // INX
        0xCA,        // DEX
        0xEA,        // NOP
    };

    for (size_t i = 0; i < program.size(); i++) {
        uint32_t addr = (0x7E << 16) | (cpu.PC + i);
        cpu.memory->write8(addr, program[i]);
    }

    std::cout << "Manual bytecode test:" << std::endl;
    cpu.printState();
    std::cout << std::endl;

    for (size_t i = 0; i < program.size(); i++) {
        cpu.step();
        cpu.printState();
    }

    return 0;
}