//
// Created by yeims on 12/23/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../include/CPU.h"

#include <iostream>
#include <ostream>

// MEMORY CALLS



bool CPU::accumulatoris8() const {
   return emulation || (P & FLAG_M);
}
bool CPU::indexis8() const {
   return emulation || (P & FLAG_X);
}


uint32_t CPU::fetchAccumulator() {
   return accumulatoris8() ? fetch8() : fetch16();
}
void CPU::emulationState() {
   emulation = true;
   P = P | FLAG_M | FLAG_X;
   SP = 0x0100 | (SP & 0x00FF);
}

uint32_t CPU::fetchIndex() {
   return indexis8() ? fetch8() : fetch16();
}
void CPU::applyWidthSideEffects(uint8_t oldP) {
   // Accumulator width change
   bool OLD_M = oldP & FLAG_M;
   bool NEW_M = P & FLAG_M;

   if (!OLD_M && NEW_M) {
      // set up accumulator to 8 bit
      A &= 0x00FF;
   }
   bool oldX = oldP & FLAG_X;
   bool newX = P & FLAG_X;

   if (!oldX && newX) {
      // set up index to 8 bit
      X &= 0x00FF;
      Y &= 0x00FF;
   }
}
CPU::CPU(Memory *mem):
    A(0), X(0), Y(0),
      SP(0), PC(0), DP(0),
      P(0), PB(0), DB(0),
      emulation(true),
      memory(mem)
{
   initOpcodeArray();
}





// OPCODES
//no ops
void CPU::op_NOP() {
   std::cout << "no ops" << std::endl;
}



void CPU::op_CLC() {
   setFlag(FLAG_C, false);
}
void CPU::op_SEC() {
   setFlag(FLAG_C, true);
}
void CPU::op_CLV() {
   setFlag(FLAG_V, false);
}
void CPU::op_CLI() {

}
void CPU::op_SEI() {

}

void CPU::printState() {
   std::cout
       << std::hex
       << "PC: " << PC
       << " A: " << A
       << " X: " << X
       << " Y: " << Y
       << " SP: " << SP
       << " P: " << (int)P
       << std::endl;
}

void CPU::unimplemented_op() {
   std::cout<<"havent done twin"<< std::endl;
}

void CPU::initOpcodeArray() {
    // Default every opcode to "unimplemented"
    for (int i = 0; i < 256; i++)
        opcodeArray[i] = &CPU::unimplemented_op;

    // ----------------------------
    // Misc
    // ----------------------------
    opcodeArray[0xEA] = &CPU::op_NOP;

    // ----------------------------
    // Load
    // ----------------------------
    opcodeArray[0xA9] = &CPU::op_LDA_imm;
    opcodeArray[0xA5] = &CPU::op_LDA_dp;

    opcodeArray[0xA2] = &CPU::op_LDX_imm;

    // If you implement LDY immediate
    // opcodeArray[0xA0] = &CPU::op_LDY_imm;

    // ----------------------------
    // Store
    // ----------------------------
    opcodeArray[0x85] = &CPU::op_STA_dp;
    opcodeArray[0x86] = &CPU::op_STX_dp;
    opcodeArray[0x84] = &CPU::op_STY_dp;

    // ----------------------------
    // Register Transfers
    // ----------------------------
    opcodeArray[0xAA] = &CPU::op_TAX;
    opcodeArray[0xA8] = &CPU::op_TAY;
    opcodeArray[0x8A] = &CPU::op_TXA;
    opcodeArray[0x98] = &CPU::op_TYA;

    opcodeArray[0x9B] = &CPU::op_TXY;
    opcodeArray[0xBB] = &CPU::op_TYX;

    opcodeArray[0x1B] = &CPU::op_TCS;
    opcodeArray[0x5B] = &CPU::op_TCD;
    opcodeArray[0x3B] = &CPU::op_TSC;
    opcodeArray[0x7B] = &CPU::op_TDC;

    opcodeArray[0xBA] = &CPU::op_TSX;
    opcodeArray[0x9A] = &CPU::op_TXS;

    // ----------------------------
    // Increment / Decrement
    // ----------------------------
    opcodeArray[0x1A] = &CPU::op_INC_a;
    opcodeArray[0x3A] = &CPU::op_DEC_a;

    opcodeArray[0xE8] = &CPU::op_INX;
    opcodeArray[0xC8] = &CPU::op_INY;

    opcodeArray[0xCA] = &CPU::op_DEX;
    opcodeArray[0x88] = &CPU::op_DEY;

    // ----------------------------
    // Compare
    // ----------------------------
    opcodeArray[0xC9] = &CPU::op_CMP_IMM;
    opcodeArray[0xE0] = &CPU::op_CPX_imm;
    opcodeArray[0xC0] = &CPU::op_CPY_imm;

    // ----------------------------
    // Flag Instructions
    // ----------------------------
    opcodeArray[0x18] = &CPU::op_CLC;
    opcodeArray[0x38] = &CPU::op_SEC;
    opcodeArray[0x58] = &CPU::op_CLI;
    opcodeArray[0x78] = &CPU::op_SEI;
    opcodeArray[0xB8] = &CPU::op_CLV;

    // ----------------------------
    // Arithmetic / Logic
    // ----------------------------
    opcodeArray[0x69] = &CPU::op_ADC_imm;
    opcodeArray[0xE9] = &CPU::op_SBC_imm;

    opcodeArray[0x29] = &CPU::op_AND_imm;
    opcodeArray[0x09] = &CPU::op_ORA_imm;
    opcodeArray[0x49] = &CPU::op_EOR_imm;

    // ----------------------------
    // Branches
    // ----------------------------
    opcodeArray[0x80] = &CPU::op_BRA;

    opcodeArray[0xD0] = &CPU::op_BNE;
    opcodeArray[0xF0] = &CPU::op_BEQ;

    opcodeArray[0x90] = &CPU::op_BCC;
    opcodeArray[0xB0] = &CPU::op_BCS;

    opcodeArray[0x30] = &CPU::op_BMI;
    opcodeArray[0x10] = &CPU::op_BPL;

    opcodeArray[0x50] = &CPU::op_BVC;
    opcodeArray[0x70] = &CPU::op_BVS;

    // ----------------------------
    // Jumps / Calls
    // ----------------------------
    opcodeArray[0x20] = &CPU::op_JSR_abs;
    opcodeArray[0x22] = &CPU::op_JSL_abs;

    opcodeArray[0x4C] = &CPU::op_JMP;

    opcodeArray[0x60] = &CPU::op_RTS;
    opcodeArray[0x6B] = &CPU::op_RTL;

    // ----------------------------
    // Stack
    // ----------------------------
    opcodeArray[0x48] = &CPU::op_PHA;
    opcodeArray[0x68] = &CPU::op_PLA;

    opcodeArray[0xDA] = &CPU::op_PHX;
    opcodeArray[0xFA] = &CPU::op_PLX;

    opcodeArray[0x5A] = &CPU::op_PHY;
    opcodeArray[0x7A] = &CPU::op_PLY;

    // ----------------------------
    // Status Register
    // ----------------------------
    opcodeArray[0xC2] = &CPU::op_REP;
    opcodeArray[0xE2] = &CPU::op_SEP;
   // Long absolute addressing
   opcodeArray[0xAF] = &CPU::op_LDA_long;  // LDA $xxxxxx
   opcodeArray[0x6F] = &CPU::op_ADC_long; // ADC $xxxxxx
   opcodeArray[0xEF] = &CPU::op_SBC_long; // SBC $xxxxxx

   // Indexed indirect
   opcodeArray[0xA1] = &CPU::op_LDA_ind_x;  // LDA ($xx,X)
   opcodeArray[0x61] = &CPU::op_ADC_ind_x;  // ADC ($xx,X)
   opcodeArray[0xE1] = &CPU::op_SBC_ind_x;  // SBC ($xx,X)

   // Indirect indexed
   opcodeArray[0xB1] = &CPU::op_LDA_ind_y;  // LDA ($xx),Y
   opcodeArray[0x71] = &CPU::op_ADC_ind_y;  // ADC ($xx),Y
   opcodeArray[0xF1] = &CPU::op_SBC_ind_y;  // SBC ($xx),Y

   // Stack relative
   opcodeArray[0xA3] = &CPU::op_ADC_stack;  // ADC $xx,S
   opcodeArray[0xE3] = &CPU::op_SBC_stack;  // SBC $xx,S
}

void CPU::step() {
   uint8_t opcode = fetch8();

   // TODO: there is apparently a better way which i will look into it later
   // rn just stick to some opcodes
   //also add cycles later
   (this->*opcodeArray[opcode])();
}

void CPU::reset(){
   A = X = Y = 0;
   DB = PB = 0;
   DP = 0;
   SP = 0x01FF;
   emulation = true;
   P = FLAG_I | FLAG_Z;
   uint8_t low = memory->read8(0x00FFFC);
   uint8_t high = memory->read8(0x00FFFD);
   PC = (high << 8) | low;
   PB = 0x00;
}







