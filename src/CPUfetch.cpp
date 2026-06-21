//
// Created by yeims on 6/21/2026.
//
#include "../include/CPU.h"

uint16_t CPU::fetch16() {
    uint16_t low = fetch8();
    uint16_t high = fetch8();
    return low | (high << 8);
}
uint8_t CPU::fetch8(){
    uint32_t addr = (PB << 16) | PC;
    uint8_t data = memory->read8(addr);
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