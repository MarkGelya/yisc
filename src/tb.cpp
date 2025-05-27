#include "cpu.h"
#include "memory.h"
#include "clock_generator.h"

#define PERIOD 2

SC_MODULE(INIT) {
    sc_out<bool> run;
    sc_out<bool> rst;

    sc_in<bool> hlt;

    void main() {
        run.write(false);
        rst.write(true);
        next_trigger(PERIOD, SC_SEC);

        run.write(true);
        rst.write(false);
        next_trigger(PERIOD, SC_SEC);

        run.write(false);
        next_trigger(PERIOD, SC_SEC);
    }

    void stop() {
        exit(0);
    }

    SC_CTOR(INIT) {
        SC_METHOD(main);

        SC_METHOD(stop);
        sensitive << hlt.pos();
        dont_initialize();
    }
};



int sc_main(int argc, char *argv[]) {
    // period, high %, start at, start value.
    sc_clock main_clk("clk", PERIOD, SC_SEC, 0.5, 0.0, SC_SEC, false);

    sc_signal<bool> clk;
    sc_signal<bool> hlt;
    sc_signal<bool> run;
    sc_signal<bool> rst;

    CLOCK_GENERATOR clock("CLOCK_GENERATOR");
    clock.main_clk(main_clk);
    clock.clk(clk);
    clock.hlt(hlt);
    clock.run(run);

    Memory memory("MEMORY", "code.bin");

    CPU cpu("CPU");
    cpu.run(run);
    cpu.rst(rst);
    cpu.clk(clk);
    cpu.hlt(hlt);
    cpu.memory(memory.socket);

    INIT init("INIT");
    init.rst(rst);
    init.run(run);
    init.hlt(hlt);

    sc_start(100, SC_SEC);

    return 0;
}
