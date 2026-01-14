//
// Created by yeims on 12/23/2025.
//

#include "../include/Memory.h"
#include <stdio.h>
Memory::Memory() : wram(128*1024, 0){

}

uint8_t Memory::read8(uint32_t address) {
  if ((0xFF0000 & address) == 0x7E0000 || (0xFF0000 & address) == 0x7F0000) {
    return wram[address & 0x01FFFF];
  }
  return 0xFF;
}
void Memory::write8(uint32_t address, uint8_t value) {
  if ((address & 0xFF0000) == 0x7E0000 || (address & 0xFF0000) == 0x7F0000) {
    wram[address & 0x1FFFF] = value;
  }
}
