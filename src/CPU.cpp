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
void CPU::setFlag(uint8_t flag, bool value) {
   if (value) P |= flag;
   else P &= ~flag;
}
uint16_t CPU::fetch16() {
   uint16_t low = fetch8();
   uint16_t high = fetch8();
   return low | (high << 8);
}
bool CPU::getFlag(uint8_t flag) const {
   return P & flag;
}
bool CPU::accumulatoris8() const {
   return emulation || (P & FLAG_M);
}
bool CPU::indexis8() const {
   return emulation || (P & FLAG_X);
}
uint8_t CPU::fetch8(){
   uint32_t addr = (PB << 16) | PC;
   uint8_t data = memory->read8(addr);
   PC++;
   return data;
}

uint32_t CPU::fetchAccumulator() {
   return accumulatoris8() ? fetch8() : fetch16();
}
void CPU::emulationState() {
   emulation = true;
   P = P | FLAG_M | FLAG_X;
   SP = 0x0100 | (SP & 0x00FF);
}
void CPU::fetchIndex() {
   if (indexis8()) {
      fetch8();
   }
   else {
      fetch16();
   }
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
//main lda call
void CPU::LDA(uint32_t value) {
   if (accumulatoris8()) {
      A = (A & 0xFF00) | (value & 0xFF);
      setZN(A & 0xFF, 8);
   }
   else {
      A = value & 0xFFFF;
      setZN(A, 16);
   }
}
void CPU::op_LDA_imm() {
   uint32_t value = fetchAccumulator();
   LDA(value);
}
void CPU::op_LDA_dp() {
   uint8_t addr = fetch8();
   uint32_t value = memory->read8(addr);
   LDA(value);
}
void CPU::op_JSR_abs() {
   uint16_t adress = fetch16();
   uint16_t returnAdd = PC - 1;
   push16(returnAdd);
   PC = adress;
}
void CPU::op_JSL_abs() {
   uint32_t adress = fetch16();
   uint8_t bank = fetch8();
   push8(PB);
   push16(PC-1);
   PB = bank;
   PC = adress;
}
void CPU::op_JMP() {
   uint16_t adress = fetch16();
   PC = adress;
}
void CPU::op_RTS() {
   uint8_t low = pop8();
   uint8_t high = pop8();
   uint16_t returnAdd = high << 8 | low;
   PC = returnAdd + 1;

}
void CPU::op_BRA() {
   int8_t offset =(int8_t) fetch8();
   PC+=offset;
}
void CPU::op_branchZeroOrNot() {
   int8_t offset = (int8_t)fetch8();
   if (getFlag(FLAG_Z)) {
      PC += (uint16_t) offset;
   }
}
void CPU::op_BCC() {
   int8_t offset = (int8_t)fetch8();
   if (!getFlag(FLAG_C)) {
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

void CPU::op_REP() {
   uint8_t mask = fetch8();
   uint16_t oldP = P;
   P = P & ~mask;
   if (getFlag(emulation)){
      P |= FLAG_M;
      P |= FLAG_X;
   }
   applyWidthSideEffects(oldP);
}
void CPU::op_SEP() {
   uint8_t mask2 = fetch8();
   uint16_t oldPSEP = P;
   P |= mask2;
   if (getFlag(emulation)) { P |= FLAG_M | FLAG_X; }
   applyWidthSideEffects(oldPSEP);
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
void CPU::setZN(uint32_t value, int width) {
   if (width == 8) {
      setFlag(FLAG_Z, (value & 0xFF) == 0);
      setFlag(FLAG_N, value & 0x80);
   } else { // 16-bit
      setFlag(FLAG_Z, (value & 0xFFFF) == 0);
      setFlag(FLAG_N, value & 0x8000);
   }
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

