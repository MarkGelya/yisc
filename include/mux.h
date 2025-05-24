#pragma once

#include "systemc.h"

template <class T>
class MUX: public sc_module {
public:
    sc_in<T> in0;
    sc_in<T> in1;
    sc_in<sc_uint<1>> sel;
    sc_out<T> out;

    SC_CTOR(MUX) {
        SC_METHOD(main);
        sensitive << in0 << in1 << sel;
    }

    void main() {
        if (sel.read())
            out.write(in1.read());
        else
            out.write(in0.read());
    }
};



