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



void CPU::unimplemented_op() {
   std::cout<<"havent done twin"<< std::endl;
}

void CPU::initOpcodeArray() {
   for (int i = 0; i < 256; i++)
      opcodeArray[i] = &CPU::unimplemented_op;

   opcodeArray[0xEA] = &CPU::op_NOP;
   opcodeArray[0xA9] = &CPU::op_LDA_imm;
   opcodeArray[0xC2] = &CPU::op_REP;
   opcodeArray[0xE2] = &CPU::op_SEP;
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







