//
// Created by yeims on 6/28/2026.
//
#include "../include/CPU.h"
void CPU::op_JMP() {
    uint16_t adress = fetch16();
    PC = adress;
}
void CPU::op_JSR_abs() {
    uint16_t adress = fetch16();
    uint16_t returnAdd = PC - 1;
    push16(returnAdd);
    PC = adress;
}
void CPU::op_JSL_abs() {
    uint32_t adress = fetch16();
    uint8_t bank = fetch8();
    push8(PB);
    push16(PC-1);
    PB = bank;
    PC = adress;
}
void CPU::op_RTS() {
    uint8_t low = pop8();
    uint8_t high = pop8();
    uint16_t returnAdd = high << 8 | low;
    PC = returnAdd + 1;

}
void CPU::op_RTL() {
    uint8_t low = pop8();
    uint8_t high = pop8();
    uint16_t returnAdd = high << 8 | low;
    PC = returnAdd + 1;
    PB = pop8();

}