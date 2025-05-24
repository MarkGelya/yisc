#pragma once

#include "systemc.h"

class CLOCK_GENERATOR : sc_core::sc_module {
public:
    sc_port<sc_signal_in_if<bool>> main_clk;
    sc_port<sc_signal_in_if<bool>> run;
    sc_port<sc_signal_in_if<bool>> hlt;
    sc_port<sc_signal_out_if<bool>> clk;

    CLOCK_GENERATOR(const sc_core::sc_module_name &name);

private:
    void main();
};
