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




//HELPERS / ALU STUFF
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
void CPU::LDY(uint32_t value) {
   if (indexis8()) {
      Y = (Y & 0xFF00) | (value & 0xFF);
      setZN(Y & 0xFF, 8);
   }
   else {
      Y = value & 0xFFFF;
      setZN(Y, 16);
   }
}
void CPU::CMP(uint32_t val) {
   if (accumulatoris8()) {
      uint8_t mem = A & 0xFF;
      uint8_t result = mem - (val & 0xFF);
      setFlag(FLAG_C, mem >= (val & 0xFF));
      setZN(result, 8);
   }
   else {
      uint16_t mem = A & 0xFFFF;
      uint16_t result = mem - (val & 0xFFFF);
      setFlag(FLAG_C, mem >= (val & 0xFFFF));
      setZN(result, 16);
   }
}
void CPU::CPY(uint32_t val) {
   if (indexis8()) {
      uint8_t mem = Y & 0xFF;
      uint8_t result = mem - (val & 0xFF);
      setFlag(FLAG_C, mem >= (val & 0xFF));
      setZN(result, 8);
   }
   else {
      uint16_t mem = Y & 0xFFFF;
      uint16_t result = mem - (val & 0xFFFF);
      setFlag(FLAG_C, result >= (val & 0xFFFF));
      setZN(result, 16);
   }
}
void CPU::CPX(uint32_t val) {
   if (indexis8()) {
      uint8_t mem = X & 0xFF;
      uint8_t result = mem - (val & 0xFF);
      setFlag(FLAG_C, mem >= (val & 0xFF));
      setZN(result, 8);
   }
   else {
      uint16_t mem = X & 0xFFFF;
      uint16_t result = mem - (val & 0xFFFF);
      setFlag(FLAG_C, result >= (val & 0xFFFF));
      setZN(result, 16);
   }
}
void CPU::ADC(uint32_t val) {
   A = A + val + (getFlag(FLAG_C) ? 1 : 0);
}
void CPU::SBC(uint32_t val) {
   A = A + (~val) + (getFlag(FLAG_C) ? 1 : 0);
}
void CPU::AND(uint32_t val) {
   if (accumulatoris8()) {
      uint8_t result = (A & 0xFF) & (val & 0xFF);
      A = (A & 0xFF00) | result;
      setZN(result, 8);
   } else {
      uint16_t result = (A & 0xFFFF) & (val & 0xFFFF);
      A = result;
      setZN(result, 16);
   }
}
void CPU::ORA(uint32_t val) {
   if (accumulatoris8()) {
      uint8_t result = (A & 0xFF) | (val & 0xFF);
      A = (A & 0xFF00) | result;
      setZN(result, 8);
   } else {
      uint16_t result = (A & 0xFFFF) | (val & 0xFFFF);
      A = result;
      setZN(result, 16);
   }

}
void CPU::EOR(uint32_t val) {
   if (accumulatoris8()) {
      uint8_t result = (A & 0xFF) ^ (val & 0xFF);
      A = (A & 0xFF00) | result;
      setZN(result, 8);
   } else {
      uint16_t result = (A & 0xFFFF) ^ (val & 0xFFFF);
      A = result;
      setZN(result, 16);
   }
}

// OPCODES
//no ops
void CPU::op_NOP() {
   std::cout << "no ops" << std::endl;
}

void CPU::op_LDA_imm() {
   uint32_t value = fetchAccumulator();
   LDA(value);
}
void CPU::op_LDA_dp() {
   uint16_t addr = DP + fetch8();

   uint32_t value = accumulatoris8()
       ? memory->read8(addr)
       : memory->read16(addr);
   LDA(value);
}
void CPU::op_LDX_imm() {
   if (indexis8()) {
      uint8_t value = fetch8();
      X = (X & 0xFF00) | value;
      setZN(value, 8);
   } else {
      uint16_t value = fetch16();
      X = value;
      setZN(value, 16);
   }
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
void CPU::op_RTL() {
   uint8_t low = pop8();
   uint8_t high = pop8();
   uint16_t returnAdd = high << 8 | low;
   PC = returnAdd + 1;
   PB = pop8();

}
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

void CPU::op_STA_dp() {
   uint32_t addr = addr_dp();

   if (accumulatoris8()) {
      memory->write8(addr, A & 0xFF);
   } else {
      memory->write16(addr, A & 0xFFFF);
   }
}
void CPU::op_STX_dp() {
   uint32_t addr = addr_dp();
   if (indexis8()) {
      memory->write8(addr, X & 0xFF);
   }
   else {
      memory->write16(addr, X & 0xFFFF);
   }
}
void CPU::op_STY_dp() {
   uint32_t addr = addr_dp();
   if (indexis8()) {
      memory->write8(addr, Y & 0xFF);
   }
   else {
      memory->write16(addr, Y & 0xFFFF);
   }
}
void CPU::op_TAX() {
   if (indexis8()) {
      X = (X & 0xFF00) | (A & 0x00FF);
      setZN(X & 0xFF, 8);
   } else {
      X = A;
      setZN(X, 16);
   }
}
void CPU::op_TAY() {
   if (indexis8()) {
      Y = (Y & 0xFF00) | (A & 0x00FF);
      setZN(Y & 0xFF, 8);
   }
   else {
      Y = A;
      setZN(Y, 16);
   }
}
void CPU::op_TXA() {
   if (accumulatoris8()) {
      A = (A & 0xFF00) | (X & 0x00FF);
      setZN(A & 0xFF, 8);
   }
   else {
      A = X;
      setZN(A, 16);
   }
}
void CPU::op_TYA() {
   if (accumulatoris8()) {
      A = (A & 0xFF00) | (Y & 0x00FF);
      setZN(A & 0xFF, 8);
   }
   else {
      A = Y;
      setZN(A, 16);
   }
}
void CPU::op_TXY() {
   if (indexis8()) {
      Y = (Y & 0xFF00) | (X & 0x00FF);
      setZN(Y & 0xFF, 8);
   }
   else {
      Y = X;
      setZN(Y, 16);
   }
}
void CPU::op_TYX() {
   if (indexis8()) {
      X = (X & 0xFF00) | (Y & 0x00FF);
      setZN(X & 0xFF, 8);
   }
   else {
      X = Y;
      setZN(X, 16);
   }
}
void CPU::op_TCS() {
   if (accumulatoris8()) {
      A = A << 8;
   }
   SP = A;
   setZN(SP, 16);
}
void CPU::op_TCD() {
   if (accumulatoris8()) {
      A = A << 8;
   }
   DP = A;
   setZN(DP, 16);
}
void CPU::op_TSC() {

   A = SP;
   setZN(A, 16);
}
void CPU::op_TDC() {
   A = DP;
   setZN(A, 16);
}
void CPU::op_TSX() {
   if (indexis8()) {
      X = (X & 0xFF00) | (SP & 0x00FF);
      setZN(X & 0xFF, 8);
   }
   else {
      X = SP;
      setZN(X, 16);
   }
}
void CPU::op_TXS() {
   if (indexis8()) {
      SP = (SP & 0xFF00) | (X & 0x00FF);
      setZN(SP, 16);
   }
   else {
      SP = X;
      setZN(SP, 16);
   }
}
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
void CPU::op_CMP_IMM() {
   uint32_t value = fetchAccumulator();
   CMP(value);
}

void CPU::op_CPX_imm() {
   uint32_t value = fetchIndex();
   CPX(value);
}

void CPU::op_CPY_imm() {
   uint32_t value = fetchIndex();
   CPY(value);
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
   FLAG_I = 0x00;
}
void CPU::op_SEI() {
   FLAG_I = 0x01;
}

void CPU::op_ADC_imm() {
   uint32_t value = fetchAccumulator();
   ADC(value);
}
void CPU::op_SBC_imm() {
   uint32_t value = fetchAccumulator();
   SBC(value);
}
void CPU::op_AND_imm() {
   uint32_t value = fetchAccumulator();
   AND(value);
}
void CPU::op_ORA_imm() {
   uint32_t value = fetchAccumulator();
   ORA(value);
}
void CPU::op_EOR_imm() {
   uint32_t value = fetchAccumulator();
   EOR(value);
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

