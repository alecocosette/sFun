//
// Created by yeims on 6/28/2026.
//
#include "../include/CPU.h"
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
//HELPERS / ALU STUFF

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
