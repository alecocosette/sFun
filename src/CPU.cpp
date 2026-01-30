//
// Created by yeims on 12/23/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../include/CPU.h"

#include <iostream>
#include <ostream>

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

void CPU::fetchAccumulator() {
   if (accumulatoris8()) {
    fetch8();
   }
   else {
      fetch16();
   }
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
void CPU::step() {
   uint8_t opcode = fetch8();

   // TODO: there is apparently a better way which i will look into it later
   // rn just stick to some opcodes
   //also add cycles later
   switch (opcode) {
      case 0xEA: //NOP (no op)
         break;
      case 0xA9://LDA imm


         if (getFlag(FLAG_M)) {
            uint8_t value = fetch8();
            A = (A & 0xFF00) | value;
            setFlag(FLAG_Z, value == 0);
            setFlag(FLAG_N, value & 0x80);
         } else {
            uint16_t value = fetch16();
            A = value;
            setFlag(FLAG_Z, value == 0);
            setFlag(FLAG_N, value & 0x8000);
         }
         break;
      case 0xA5: // LDA direct page
         if (getFlag(FLAG_M)) {
            uint8_t value = fetch8();

         }
         break;
      case 0xAD: // LDA absolute
         break;
      case 0x85: //STA dp
         break;
      case 0x8D: //STA abs
         break;
      case 0x4C: //JUMP ABS
         break;
      case 0x00: //BREAK
         break;
      case 0xC2: //REP
         uint8_t mask = fetch8();
         uint16_t oldP = P;
         P = P & ~mask;
            if (emulation){
               P |= FLAG_M;
               P |= FLAG_X;
            }
         applyWidthSideEffects(oldP);
         break;
      case 0xE2: //SEP
         P = P | FLAG_M;
      //TODO
         //this one can change depending on what i find online
      break;
      default:
         std::cout <<"what are we doing bro "<<opcode<<std::endl;
   }
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
}