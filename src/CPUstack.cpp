//
// Created by yeims on 6/28/2026.
//
#include "../include/CPU.h"

void CPU::push8(uint8_t value) {
    memory->write8(0x0100 | (SP&0xFF), value);
    SP--;
}
void CPU::push16(uint16_t value) {
    uint8_t high = value >> 8;
    uint8_t low = value & 0xFF;
    memory->write8(0x0100 + SP, high);
    SP--;
    memory->write8(0x0100 + SP, low);
    SP--;
}
// POINTING TO THE NEXT LOCATION OOOOOOOO
uint8_t CPU::pop8() {
    SP++;
    return memory->read8(0x0100 | (SP&0xFF));
}
uint16_t CPU::pop16() {
    uint8_t low = pop8();
    uint8_t high = pop8();
    return (high << 8) | low;
}

// push/pull ops
void CPU::op_PHA() {
    if (!getFlag(FLAG_M)) {
        push16(A);
    }
    else {
        push8(A);
    }

}
void CPU::op_PHX() {
    if (!getFlag(FLAG_X)) {
        push16(X);
    }
    else {
        push8(X);
    }
}
void CPU::op_PHY() {
    if (!getFlag(FLAG_X)) {
        push16(Y);
    }
    else {
        push8(Y);
    }
}


void CPU::op_PLA() {
    if (accumulatoris8()) {
        uint8_t value = pop8();
        A = (A & 0xFF00) | value;
        setZN(value, 8);
    } else {
        uint16_t value = pop16();
        A = value;
        setZN(value, 16);
    }
}
void CPU::op_PLX() {
    if (indexis8()) {
        uint8_t value = pop8();
        X = (X & 0xFF00) | value;
        setZN(value, 8);
    } else {
        uint16_t value = pop16();
        X = value;
        setZN(value, 16);
    }
}
void CPU::op_PLY() {
    if (indexis8()) {
        uint8_t value = pop8();
        Y = (Y & 0xFF00) | value;
        setZN(value, 8);
    } else {
        uint16_t value = pop16();
        Y = value;
        setZN(value, 16);
    }
}