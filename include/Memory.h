//
// Created by yeims on 12/23/2025.
//

#ifndef MEMORY_H
#define MEMORY_H
#pragma once
#include <cstdint>
#include <vector>


struct Memory {
  //wram
  std::vector<uint8_t> wram;
  Memory();
  uint8_t read8(uint32_t address);
  void write8(uint32_t address, uint8_t value);
};



#endif //MEMORY_H
