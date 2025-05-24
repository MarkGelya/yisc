#pragma once

#include "systemc.h"

#include "alu.h"
#include "registers.h"

enum ALU_CMD {
    OR = 0,
    AND,
    XOR,
    ADD,
    SUB,
    ADDI,
};

SC_MODULE(ALU) {
public:
    sc_in<sc_uint<32>> srcA;
    sc_in<sc_uint<32>> srcB;
    sc_in<sc_uint<3>> aluCmd;

    sc_out<sc_uint<32>> result;

    SC_CTOR(ALU) {
        SC_METHOD(main);
        sensitive << srcA << srcB << aluCmd;
        dont_initialize();
    }

private:
    RV32Reg registers;
    void main();
};
