//
// Created by yeims on 6/28/2026.
//
#include "../include/CPU.h"
void CPU::op_BRA() {
    int8_t offset =(int8_t) fetch8();
    PC+=offset;
}


void CPU::op_BNE() {
    int8_t offset = (int8_t)fetch8();
    if (!getFlag(FLAG_Z)) {
        PC += offset;
    }
}
void CPU::op_BEQ() {
    int8_t offset = (int8_t)fetch8();
    if (getFlag(FLAG_Z)) {
        PC += offset;
    }
}
void CPU::op_BCC() {
    int8_t offset = (int8_t)fetch8();
    if (!getFlag(FLAG_C)) {
        PC += offset;
    }
}
void CPU::op_BVC() {
    int8_t offset = (int8_t)fetch8();
    if (!getFlag(FLAG_V)) {
        PC += offset;
    }
}
void CPU::op_BVS() {
    int8_t offset = (int8_t)fetch8();
    if (getFlag(FLAG_V)) {
        PC += offset;
    }
}

void CPU::op_BCS() {
    int8_t offset = (int8_t)fetch8();
    if (getFlag(FLAG_C)) {
        PC += offset;
    }
}
//branch if minus
void CPU::op_BMI() {
    int8_t offset = (int8_t)fetch8();
    if (getFlag(FLAG_N)) {
        PC += offset;
    }
}
//branch if plus
void CPU::op_BPL() {
    int8_t offset = (int8_t)fetch8();
    if (!getFlag(FLAG_N)) {
        PC += offset;
    }
}
