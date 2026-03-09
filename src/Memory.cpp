//
// Created by yeims on 12/23/2025.
//

#include "../include/Memory.h"
#include <stdio.h>
Memory::Memory() : wram(128*1024, 0), rom(512 * 1024, 0)
{
  // Fake program at $00:8000
  rom[0x0000] = 0xA9; // LDA #$01
  rom[0x0001] = 0x01;
  rom[0x0002] = 0xEA; // NOP
  rom[0x0003] = 0xEA; // NOP

  // Reset vector
  rom[0x7FFC] = 0x00; // low
  rom[0x7FFD] = 0x80; // high

}
void Memory::loadROM(const std::vector<uint8_t>& data) {
  rom = data;
}
uint8_t Memory::read8(uint32_t address) {

  uint32_t index = address & 0x7FFF;
  if (index < rom.size()) {
    return rom[index];
  } else {
    return 0xFF;
  }
  //come bacl to it later
  // if ((0xFF0000 & address) == 0x7E0000 || (0xFF0000 & address) == 0x7F0000) {
  //   return wram[address & 0x01FFFF];
  // }
  // if ((address & 0x8000) && (address <= 0x00FFFF)) {
  //   return rom[address & 0x7FFF];
  // }
  // return 0xFF;
}
void Memory::write8(uint32_t address, uint8_t value) {
  if ((address & 0xFF0000) == 0x7E0000 || (address & 0xFF0000) == 0x7F0000) {
    wram[address & 0x1FFFF] = value;
  }
}
