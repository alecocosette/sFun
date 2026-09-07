#ifndef BUS_H
#define BUS_H
#include <string>
#include <stdint.h>
#include <vector>
#include "Memory.h"
#include "Cart.h"

class Bus {
public:
    Bus();

    ~Bus();

    // Read/write operations
    uint8_t read8(uint32_t address);
    uint16_t read16(uint32_t address);
    void write8(uint32_t address, uint8_t value);
    void write16(uint32_t address, uint16_t value);
    
    // Load cartridge
    void loadCartridge(const std::string& path);
    
private:
    uint8_t* wram;  // 128KB WRAM
    class Cart* cartridge;
};

#endif