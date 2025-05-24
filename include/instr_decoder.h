#pragma once

#include "systemc.h"

SC_MODULE(INSTR_DECODER) {
public:
    sc_in<sc_bv<32>> instr;

    sc_out<sc_bv<5>> opcode;
    sc_out<sc_bv<5>> rd;
    sc_out<sc_bv<5>> rs1;
    sc_out<sc_bv<5>> rs2;
    sc_out<sc_bv<3>> funct3;
    sc_out<sc_bv<7>> funct7;
    sc_out<sc_bv<12>> immI;
    sc_out<sc_bv<20>> immU;
    sc_out<sc_bv<20>> immB;

    SC_CTOR(INSTR_DECODER) {
        SC_METHOD(main);
        sensitive << instr;
        dont_initialize();
    }

private:
    void main();
};
