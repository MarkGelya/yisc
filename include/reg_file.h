#pragma once

#include "systemc.h"

#include "registers.h"

SC_MODULE(REGISTER_FILE) {
public:
    sc_in<sc_bv<5>> a1;
    sc_in<sc_bv<5>> a2;
    sc_in<sc_bv<5>> ad;
    sc_in<bool> writeEnable;
    sc_in<bool> reset;
    sc_in<sc_uint<32>> data;
    sc_in<bool> clk;

    sc_out<sc_uint<32>> rd1;
    sc_out<sc_uint<32>> rd2;

    SC_CTOR(REGISTER_FILE) {
        SC_METHOD(resetMethod);
        sensitive << reset.pos();
        dont_initialize();

        SC_METHOD(writeMethod);
        sensitive << clk.neg();
        dont_initialize();

        SC_METHOD(readDataMethod);
        sensitive << a1 << a2;
        dont_initialize();
    }

private:
    RV32Reg registers;
    void resetMethod();
    void writeMethod();
    void readDataMethod();
};
