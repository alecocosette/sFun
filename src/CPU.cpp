//
// Created by yeims on 12/23/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../include/CPU.h"

void CPU::setFlag(uint8_t flag, bool value) {
   if (value) P |= flag;
   else P &= ~flag;
}
bool CPU::getFlag(uint8_t flag) const {
   return P & flag;
}
uint8_t CPU::fetch8(){
   uint32_t addr = (PB << 16) | PC;
   uint8_t data = memory->read8(addr);
   PC++;
   return data;
}
void CPU::step() {
   uint8_t opcode = fetch8();
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