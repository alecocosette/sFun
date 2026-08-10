//
// Created by yeims on 6/28/2026.
//
#include "../include/CPU.h"
void CPU::LDA(uint32_t value) {
    if (accumulatoris8()) {
        A = (A & 0xFF00) | (value & 0xFF);
        setZN(A & 0xFF, 8);
    }
    else {
        A = value & 0xFFFF;
        setZN(A, 16);
    }
}

void CPU::op_LDA_imm() {
    uint32_t value = fetchAccumulator();
    LDA(value);
}
void CPU::op_LDA_dp() {
    uint16_t addr = DP + fetch8();

    uint32_t value = accumulatoris8()
        ? memory->read8(addr)
        : memory->read16(addr);
    LDA(value);
}
void CPU::op_LDA_long() {
    uint32_t addr = addr_long();
    uint32_t value = fetchAccumulator();
    LDA(value);
}
void CPU::op_LDA_ind_x() {
    uint32_t addr = addr_ind_x();
    uint32_t value = memory->read8(addr);
    if (!accumulatoris8()) {
        value |= (memory->read8(addr + 1) << 8);
    }
    LDA(value);
}
void CPU::op_LDA_ind_y() {
    uint32_t addr = addr_ind_y();
    uint32_t value = memory->read8(addr);
    if (!accumulatoris8()) {
        value |= (memory->read8(addr + 1) << 8);
    }
    LDA(value);
}
void CPU::op_LDX_imm() {
    if (indexis8()) {
        uint8_t value = fetch8();
        X = (X & 0xFF00) | value;
        setZN(value, 8);
    } else {
        uint16_t value = fetch16();
        X = value;
        setZN(value, 16);
    }
}
void CPU::LDY(uint32_t value) {
    if (indexis8()) {
        Y = (Y & 0xFF00) | (value & 0xFF);
        setZN(Y & 0xFF, 8);
    }
    else {
        Y = value & 0xFFFF;
        setZN(Y, 16);
    }
}








void CPU::op_STA_dp() {
    uint32_t addr = addr_dp();

    if (accumulatoris8()) {
        memory->write8(addr, A & 0xFF);
    } else {
        memory->write16(addr, A & 0xFFFF);
    }
}
void CPU::op_STX_dp() {
    uint32_t addr = addr_dp();
    if (indexis8()) {
        memory->write8(addr, X & 0xFF);
    }
    else {
        memory->write16(addr, X & 0xFFFF);
    }
}
void CPU::op_STY_dp() {
    uint32_t addr = addr_dp();
    if (indexis8()) {
        memory->write8(addr, Y & 0xFF);
    }
    else {
        memory->write16(addr, Y & 0xFFFF);
    }
}


void CPU::op_ADC_long() {
    uint32_t addr = addr_long();
    uint32_t value = fetchAccumulator();
    ADC(value);
}

void CPU::op_SBC_long() {
    uint32_t addr = addr_long();
    uint32_t value = fetchAccumulator();
    SBC(value);
}



void CPU::op_ADC_ind_x() {
    uint32_t addr = addr_ind_x();
    uint32_t value = memory->read8(addr);
    if (!accumulatoris8()) {
        value |= (memory->read8(addr + 1) << 8);
    }
    ADC(value);
}

void CPU::op_SBC_ind_x() {
    uint32_t addr = addr_ind_x();
    uint32_t value = memory->read8(addr);
    if (!accumulatoris8()) {
        value |= (memory->read8(addr + 1) << 8);
    }
    SBC(value);
}



void CPU::op_ADC_ind_y() {
    uint32_t addr = addr_ind_y();
    uint32_t value = memory->read8(addr);
    if (!accumulatoris8()) {
        value |= (memory->read8(addr + 1) << 8);
    }
    ADC(value);
}

void CPU::op_SBC_ind_y() {
    uint32_t addr = addr_ind_y();
    uint32_t value = memory->read8(addr);
    if (!accumulatoris8()) {
        value |= (memory->read8(addr + 1) << 8);
    }
    SBC(value);
}

// Stack relative
void CPU::op_ADC_stack() {
    uint32_t addr = addr_stack_rel();
    uint32_t value = memory->read8(addr);
    if (!accumulatoris8()) {
        value |= (memory->read8(addr + 1) << 8);
    }
    ADC(value);
}

void CPU::op_SBC_stack() {
    uint32_t addr = addr_stack_rel();
    uint32_t value = memory->read8(addr);
    if (!accumulatoris8()) {
        value |= (memory->read8(addr + 1) << 8);
    }
    SBC(value);
}