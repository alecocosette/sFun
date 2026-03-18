//
// Created by yeims on 12/23/2025.
//

#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../include/Memory.h"

struct CPU {
    uint16_t A=0, X=0, Y = 0;
    uint16_t PC=0;
    uint16_t SP=0x01FF, PB=0, DP=0, DB=0;
    uint8_t P;
    Memory * memory;
    CPU(Memory* mem);
    //negative
     static uint8_t constexpr FLAG_N = 0x80;
    //overflow
     static uint8_t constexpr FLAG_V = 0x40;
    // A size
     static uint8_t constexpr FLAG_M = 0x20;
    // Index size
     static uint8_t constexpr FLAG_X = 0x10;
    // decimal
     static uint8_t constexpr FLAG_D = 0x08;
    // IRQ disable (idk yet)
     static uint8_t constexpr FLAG_I = 0x04;
    //zero
     static uint8_t constexpr FLAG_Z = 0x02;
    //carry
     static uint8_t constexpr FLAG_C = 0x01;


    // break or not emulation
    bool emulation;
    uint8_t setOPCODE(Memory * memory);
    void step();
    uint8_t fetch8();
    uint16_t fetch16();
    uint8_t pop8();
 uint16_t pop16();
 void push8(uint8_t val);
 void push16(uint16_t val);
    // good for optimization apparently
    inline bool accumulatoris8() const;
    inline bool indexis8() const;
 void applyWidthSideEffects(uint8_t oldP);
 //useful feature from snes9x
 void setZN(uint32_t value, int width);
    uint32_t fetchAccumulator();
    void fetchIndex();
   void emulationState();
    bool getFlag(uint8_t flag) const;
    void setFlag(uint8_t flag, bool value);
    void reset();
    typedef void(CPU::*OpcodeHandler)();
 OpcodeHandler opcodeArray[256];
 void initOpcodeArray();
void unimplemented_op();
 void op_NOP();
 void LDA(uint32_t val);
 void op_LDA_imm();

    void op_LDA_dp();

    void op_JSR_abs();

    void op_JSL_abs();

    void op_JMP();

    void op_RTS();

    void op_BRA();

    void op_branchZeroOrNot();

    void op_BCC();

    void op_BCS();

    void op_BMI();

    void op_BPL();

    void op_BEQ();

    void op_JSR(uint32_t address);

    void op_REP();
 void op_SEP();
};



#endif //CPU_H
