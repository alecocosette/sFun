//
// Created by yeims on 12/23/2025.
//


#pragma once
#include <cstdint>
#include <vector>
#include "cart.h"

struct Memory {
  //wram
  Cart* cart = nullptr;
  std::vector<uint8_t> wram;
  std::vector<uint8_t> rom;

  Memory();
  void loadROM(const std::vector<uint8_t>& data);

  uint8_t read8(uint32_t address);
  void write8(uint32_t address, uint8_t value);

};




