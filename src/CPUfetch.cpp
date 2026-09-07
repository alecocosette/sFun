//
// Created by yeims on 6/21/2026.
//
#include "Bus.h"
#include "../include/CPU.h"

uint16_t CPU::fetch16() {
    uint16_t low = fetch8();
    uint16_t high = fetch8();
    return low | (high << 8);
}
uint8_t CPU::fetch8(){
    uint32_t addr = (PB << 16) | PC;
    uint8_t data = bus->read8(addr);  // Changed from memory->read8
    PC++;
    return data;
}
// ADDRESSING MODES
uint32_t CPU::addr_absolute() {

    //change whenever JMP or JSR

    uint16_t abs = fetch16();
    return (DB << 16) | abs;
}
uint32_t CPU::addr_absolute_x() {

    //change whenever JMP or JSR

    uint16_t abs = fetch16();
    return (DB << 16) | ((abs + X) & 0xFFFF);
}

uint32_t CPU::addr_absolute_y() {

    //change whenever JMP or JSR

    uint16_t abs = fetch16();
    return (DB << 16) | ((abs + Y) & 0xFFFF);
}
uint32_t CPU::addr_long() {
    uint8_t low = fetch8();
    uint8_t mid = fetch8();
    uint8_t high = fetch8();
    return (high << 16) | (mid << 8) | low;
}

uint32_t CPU::addr_dp() {
    uint8_t offset = fetch8();

    return (DP + offset) & 0xFFFF;
}
uint32_t CPU::addr_dp_x() {
    uint8_t offset = fetch8();

    uint16_t index = indexis8()
        ? (X & 0x00FF)
        : X;

    return (DP + offset + index) & 0xFFFF;
}
uint32_t CPU::addr_dp_y() {
    uint8_t offset = fetch8();
    uint16_t index = indexis8() ? (Y & 0x00FF) : Y;
    return (DP + offset + index) & 0xFFFF;
}

uint32_t CPU::addr_ind_x() {
    uint8_t offset = fetch8();
    uint16_t zeroPageAddr = (DP + offset + X) & 0xFFFF;  // Can wrap

    // Read the 16-bit address from zero page
    uint8_t addrLow = memory->read8(zeroPageAddr);
    uint8_t addrHigh = memory->read8((zeroPageAddr + 1) & 0xFFFF);  // Wrap within page

    uint16_t addr = addrLow | (addrHigh << 8);
    return (DB << 16) | addr;
}
uint32_t CPU::addr_ind_y() {
    uint8_t offset = fetch8();
    uint16_t zeroPageAddr = (DP + offset) & 0xFFFF;

    // Read the 16-bit address from zero page
    uint8_t addrLow = memory->read8(zeroPageAddr);
    uint8_t addrHigh = memory->read8((zeroPageAddr + 1) & 0xFFFF);

    uint16_t addr = (addrLow | (addrHigh << 8)) + Y;
    return (DB << 16) | (addr & 0xFFFF);
}
uint32_t CPU::addr_stack_rel() {
    uint8_t offset = fetch8();
    uint16_t addr = (SP + offset) & 0xFFFF;
    return addr;  // Stack is always in bank 0
}