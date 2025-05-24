#pragma once

#include "systemc.h"

SC_MODULE(PROG_COUNTER) {
public:
    sc_in<bool> clk;
    sc_in<bool> rst;
    sc_in<sc_uint<1>> pcSrc;
    sc_in<sc_bv<20>> immB;

    sc_out<sc_uint<32>> addr;

    SC_CTOR(PROG_COUNTER) {
        SC_METHOD(main);
        sensitive << clk.pos() << rst.pos();
        dont_initialize();
    }

private:
    uint32_t reg;

    void main();
};
