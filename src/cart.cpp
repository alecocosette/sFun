#include "../include/cart.h"
#include <fstream>
#include <iostream>
Cart::Cart() : rom_size(0), sram_size(0), lorom(false), hirom(false),
               rom_type(0), country(0), license(0), version(0),
               reset_vector(0), nmi_vector(0) {
}
uint8_t Cart::read8(uint32_t addr) {
    // LoROM mapping: $00:8000-$FF:FFFF maps to ROM
    // Address space is 24-bit
    
    if (lorom) {
        // LoROM: lower 15 bits of address within a bank
        uint32_t offset = addr & 0x7FFF;
        uint8_t bank = (addr >> 16) & 0x7F;
        uint32_t rom_addr = (bank * 0x8000) + offset;
        
        if (rom_addr < rom.size()) {
            return rom[rom_addr];
        }
    } else if (hirom) {
        // HiROM: banks $C0-$FF map to ROM
        uint8_t bank = (addr >> 16) & 0xFF;
        uint16_t offset = addr & 0xFFFF;
        
        if (bank >= 0xC0) {
            uint32_t rom_addr = ((bank - 0xC0) * 0x10000) + offset;
            if (rom_addr < rom.size()) {
                return rom[rom_addr];
            }
        }
    }
    
    return 0xFF;  // Open bus
}

void Cart::write8(uint32_t addr, uint8_t value) {
    // SRAM write (if present)
    if (sram_size > 0) {
        // SRAM is typically at $70:0000-$7F:FFFF (LoROM) or $A0:0000-$BF:FFFF (HiROM)
        uint8_t bank = (addr >> 16) & 0xFF;
        uint16_t offset = addr & 0xFFFF;
        
        if (lorom && (bank == 0x70 || bank == 0x71)) {
            uint32_t sram_addr = offset;
            if (sram_addr < sram.size()) {
                sram[sram_addr] = value;
            }
        } else if (hirom && bank >= 0xA0 && bank <= 0xBF) {
            uint32_t sram_addr = ((bank - 0xA0) * 0x10000) + offset;
            if (sram_addr < sram.size()) {
                sram[sram_addr] = value;
            }
        }
    }
}