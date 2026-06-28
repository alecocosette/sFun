//
// Created by yeims on 6/28/2026.
//
#include "../include/CPU.h"
void CPU::op_TAX() {
    if (indexis8()) {
        X = (X & 0xFF00) | (A & 0x00FF);
        setZN(X & 0xFF, 8);
    } else {
        X = A;
        setZN(X, 16);
    }
}
void CPU::op_TAY() {
    if (indexis8()) {
        Y = (Y & 0xFF00) | (A & 0x00FF);
        setZN(Y & 0xFF, 8);
    }
    else {
        Y = A;
        setZN(Y, 16);
    }
}
void CPU::op_TXA() {
    if (accumulatoris8()) {
        A = (A & 0xFF00) | (X & 0x00FF);
        setZN(A & 0xFF, 8);
    }
    else {
        A = X;
        setZN(A, 16);
    }
}
void CPU::op_TYA() {
    if (accumulatoris8()) {
        A = (A & 0xFF00) | (Y & 0x00FF);
        setZN(A & 0xFF, 8);
    }
    else {
        A = Y;
        setZN(A, 16);
    }
}
void CPU::op_TXY() {
    if (indexis8()) {
        Y = (Y & 0xFF00) | (X & 0x00FF);
        setZN(Y & 0xFF, 8);
    }
    else {
        Y = X;
        setZN(Y, 16);
    }
}
void CPU::op_TYX() {
    if (indexis8()) {
        X = (X & 0xFF00) | (Y & 0x00FF);
        setZN(X & 0xFF, 8);
    }
    else {
        X = Y;
        setZN(X, 16);
    }
}
void CPU::op_TCS() {
    if (accumulatoris8()) {
        A = A << 8;
    }
    SP = A;
    setZN(SP, 16);
}
void CPU::op_TCD() {
    if (accumulatoris8()) {
        A = A << 8;
    }
    DP = A;
    setZN(DP, 16);
}
void CPU::op_TSC() {

    A = SP;
    setZN(A, 16);
}
void CPU::op_TDC() {
    A = DP;
    setZN(A, 16);
}
void CPU::op_TSX() {
    if (indexis8()) {
        X = (X & 0xFF00) | (SP & 0x00FF);
        setZN(X & 0xFF, 8);
    }
    else {
        X = SP;
        setZN(X, 16);
    }
}
void CPU::op_TXS() {
    if (indexis8()) {
        SP = (SP & 0xFF00) | (X & 0x00FF);
        setZN(SP, 16);
    }
    else {
        SP = X;
        setZN(SP, 16);
    }
}