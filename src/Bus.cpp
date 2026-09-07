#include "../include/Bus.h"

#include <fstream>
#include <string>
#include <iostream>
Bus::Bus() : cartridge(nullptr) {
    wram = new uint8_t[0x20000];  // Allocate on heap
    for (int i = 0; i < 0x20000; i++) {
        wram[i] = 0;
    }
}

Bus::~Bus() {
    delete[] wram;
    if (cartridge != nullptr) {
        delete cartridge;
    }
}

uint8_t Bus::read8(uint32_t address) {
    uint8_t bank = (address >> 16) & 0xFF;
    uint16_t addr = address & 0xFFFF;

    // WRAM: $7E:0000-$7E:FFFF and $7F:0000-$7F:FFFF
    if (bank == 0x7E || bank == 0x7F) {
        return wram[address & 0x1FFFF];
    }

    // Cartridge ROM
    if (cartridge != nullptr) {
        return cartridge->read8(address);
    }

    return 0xFF;
}

uint16_t Bus::read16(uint32_t address) {
    uint8_t low = read8(address);
    uint8_t high = read8(address + 1);
    return low | (high << 8);
}

void Bus::write8(uint32_t address, uint8_t value) {
    uint8_t bank = (address >> 16) & 0xFF;

    if (bank == 0x7E || bank == 0x7F) {
        wram[address & 0x1FFFF] = value;
    } else if (cartridge != nullptr) {
        cartridge->write8(address, value);
    }
}

void Bus::write16(uint32_t address, uint16_t value) {
    write8(address, value & 0xFF);
    write8(address + 1, (value >> 8) & 0xFF);
}

void Bus::loadCartridge(const std::string& path) {
    cartridge = new Cart();

    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "ERROR: Could not open ROM: " << path << std::endl;
        return;
    }

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    cartridge->rom.resize(size);
    file.read((char*)cartridge->rom.data(), size);
    file.close();

    cartridge->rom_size = size;
    cartridge->lorom = true;
    cartridge->hirom = false;

    std::cout << "Loaded ROM: " << path << " (" << size << " bytes)" << std::endl;
}