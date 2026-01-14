//
// Created by yeims on 1/13/2026.
//

#ifndef CART_H
#define CART_H
#include "stdio.h"
#include <stdint.h>
#pragma once

// switch to vectors and enums
struct Cart{
  uint8_t type;
    uint8_t *rom;
    uint8_t * ram;
    uint32_t rom_size;
    uint32_t ram_size;
};




#endif //CART_H
