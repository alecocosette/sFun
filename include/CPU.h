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

    //negative
     uint8_t constexpr FLAG_N = 0x80;
    //overflow
     uint8_t constexpr FLAG_V = 0x40;
    // A size
     uint8_t constexpr FLAG_M = 0x20;
    // Index size
     uint8_t constexpr FLAG_X = 0x10;
    // decimal
     uint8_t constexpr FLAG_D = 0x08;
    // IRQ disable (idk yet)
     uint8_t constexpr FLAG_I = 0x04;
    //zero
     uint8_t constexpr FLAG_Z = 0x02;
    //carry
     uint8_t constexpr FLAG_C = 0x01;


    // break or not emulation
    bool emulation;
    uint8_t setOPCODE(Memory * memory);
    void step();
    uint8_t fetch8();
    uint16_t fetch16();

    // good for optimization apparently
    inline bool accumulatoris8() const;
    inline bool indexis8() const;
 void applyWidthSideEffects(uint8_t oldP);
 //useful feature from snes9x, look more later
 void setZN(uint32_t value, int width);
    void fetchAccumulator();
    void fetchIndex();
   void emulationState();
    bool getFlag(uint8_t flag) const;
    void setFlag(uint8_t flag, bool value);
    void reset();
};



#endif //CPU_H
