#include "cpu.h"
#include "memory.h"
#include "clock_generator.h"

int sc_main(int argc, char *argv[]) {
    // period, high %, start at, start value.
    sc_clock main_clk("clk", 2, SC_SEC, 0.5, 0.0, SC_SEC, false);

    sc_signal<bool> clk;
    sc_signal<bool> hlt;
    sc_signal<bool> run;
    sc_signal<bool> rst;

    run.write(true);
    rst.write(false);

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

    sc_start(100, SC_SEC);

    return 0;
}
