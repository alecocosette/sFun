//
// Created by yeims on 1/13/2026.
//

#ifndef CART_H
#define CART_H
#include "stdio.h"
#include <stdint.h>
#include <vector>
#pragma once

// switch to vectors and enums
class Cart{
    public:
    std::vector<uint8_t> rom;
    std::vector<uint8_t> sram;

    uint32_t rom_size;
    uint32_t sram_size;

    bool lorom;
    bool hirom;

    uint8_t rom_type;
    uint8_t country;
    uint8_t license;
    uint8_t version;

    uint16_t reset_vector;
    uint16_t nmi_vector;

    uint8_t read8(uint32_t addr);
    void write8(uint32_t addr, uint8_t value);

};




#endif //CART_H
