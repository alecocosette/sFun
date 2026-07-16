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
      setFlag(FLAG_C, mem >= (val & 0xFFFF));
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
      setFlag(FLAG_C, mem >= (val & 0xFFFF));
      setZN(result, 16);
   }
}
void CPU::ADC(uint32_t val) {
   if (accumulatoris8()) {
      uint8_t a = A & 0xFF;
      uint8_t v = val & 0xFF;
      uint8_t c = getFlag(FLAG_C) ? 1 : 0;
      uint16_t result = (uint16_t)a + (uint16_t)v + c;
      setFlag(FLAG_C, result > 0xFF);

      uint8_t result8 = result & 0xFF;
      bool overflow = ((a ^ result8) & (v ^ result8) & 0x80) != 0;
      setFlag(FLAG_V, overflow);

      A = (A & 0xFF00) | result8;
      setZN(result8, 8);
   } else {
      uint16_t a = A & 0xFFFF;
      uint16_t v = val & 0xFFFF;
      uint16_t c = getFlag(FLAG_C) ? 1 : 0;
      uint32_t result = (uint32_t)a + (uint32_t)v + c;

      setFlag(FLAG_C, result > 0xFFFF);

      uint16_t result16 = result & 0xFFFF;
      bool overflow = ((a ^ result16) & (v ^ result16) & 0x8000) != 0;
      setFlag(FLAG_V, overflow);

      A = result16;
      setZN(result16, 16);
   }
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
   setFlag(FLAG_V, false);
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
   setFlag(FLAG_V, false);
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
   setFlag(FLAG_V, false);
}
