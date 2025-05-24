#include "clock_generator.h"

SC_HAS_PROCESS(CLOCK_GENERATOR);

CLOCK_GENERATOR::CLOCK_GENERATOR(const sc_core::sc_module_name &name) : sc_module(name) {
    SC_METHOD(main);
    sensitive << main_clk;
    dont_initialize();
}

void CLOCK_GENERATOR::main() {
    if (main_clk->read()) {
        std::cout << sc_time_stamp() << ": " << "CLOCK" << std::endl;
    }
    if (!hlt->read() || run->read()) {
        clk->write(main_clk->read());
    }
}
