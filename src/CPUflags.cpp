#include "../include/CPU.h"

void CPU::setFlag(uint8_t flag, bool value) {
    if (value) P |= flag;
    else P &= ~flag;
}

bool CPU::getFlag(uint8_t flag) const {
    return P & flag;
}
void CPU::op_REP() {
    uint8_t mask = fetch8();
    uint16_t oldP = P;
    P = P & ~mask;
    if (emulation){
        P |= FLAG_M;
        P |= FLAG_X;
    }
    applyWidthSideEffects(oldP);
}
void CPU::op_SEP() {
    uint8_t mask2 = fetch8();
    uint16_t oldPSEP = P;
    P |= mask2;
    if (emulation) { P |= FLAG_M | FLAG_X; }
    applyWidthSideEffects(oldPSEP);
}
void CPU::setZN(uint32_t value, int width) {
    if (width == 8) {
        setFlag(FLAG_Z, (value & 0xFF) == 0);
        setFlag(FLAG_N, value & 0x80);
    } else { // 16-bit
        setFlag(FLAG_Z, (value & 0xFFFF) == 0);
        setFlag(FLAG_N, value & 0x8000);
    }
}