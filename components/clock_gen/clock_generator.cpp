#include "systemc.h"

SC_MODULE(CLOCK_GENERATOR) {
public:
    sc_port<sc_signal_in_if<bool>> main_clk;
    sc_port<sc_signal_in_if<bool>> run;
    sc_port<sc_signal_in_if<bool>> hlt;
    sc_port<sc_signal_out_if<bool>> clk;


    SC_CTOR(CLOCK_GENERATOR) {
        SC_METHOD(main);
        sensitive << main_clk;
        dont_initialize();
    }

private:
    void main();
};

void CLOCK_GENERATOR::main() {
    if (main_clk->read()) {
        std::cout << sc_time_stamp() << ": " << "CLOCK" << std::endl;
    }
    if (!hlt->read() || run->read()) {
        clk->write(main_clk->read());
    }
}
