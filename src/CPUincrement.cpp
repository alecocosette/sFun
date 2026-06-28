//
// Created by yeims on 6/28/2026.
//
#include "../include/CPU.h"

void CPU::op_INC_a() {
    if (accumulatoris8()) {
        uint8_t value = (A & 0xFF) + 1;
        A = (A & 0xFF00) | value;
        setZN(A, 8);
    }
    else {
        A = (A + 1) & 0xFFFF;
        setZN(A, 16);
    }
}
void CPU::op_INX() {
    if (indexis8()) {
        uint8_t value = (X & 0xFF) + 1;

        X = (X & 0xFF00) | value;

        setZN(value, 8);
    } else {
        X = (X + 1) & 0xFFFF;

        setZN(X, 16);
    }
}
void CPU::op_INY() {
    if (indexis8()) {
        uint8_t value = (Y & 0xFF) + 1;
        Y = (Y & 0xFF00) | value;
        setZN(value, 8);
    }
    else {
        Y = (Y + 1) & 0xFFFF;
        setZN(Y, 16);
    }
}
void CPU::op_DEC_a() {
    if (accumulatoris8()) {
        uint8_t value = (A & 0xFF) - 1;
        A = (A & 0xFF00) | value;
        setZN(A, 8);
    }
    else {
        A = (A - 1) & 0xFFFF;
        setZN(A, 16);
    }
}
void CPU::op_DEX(){
    if (indexis8()) {
        uint8_t value = (X & 0xFF) - 1;
        X = (X & 0xFF00) | value;
        setZN(value, 8);
    }
    else {
        X = (X - 1) & 0xFFFF;
        setZN(X, 16);
    }
}
void CPU::op_DEY() {
    if (indexis8()) {
        uint8_t value = (Y & 0xFF) - 1;
        Y = (Y & 0xFF00) | value;
        setZN(value, 8);
    }
    else {
        Y = (Y - 1) & 0xFFFF;
        setZN(Y, 16);
    }
}
