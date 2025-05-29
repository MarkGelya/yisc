#include "cpu.h"
#include "memory.h"
#include "clock_generator.h"
#include "bus.h"

#define PERIOD 2

#define FILENAME "code.bin"

SC_MODULE(INIT) {
    SC_HAS_PROCESS(INIT);

    uint32_t programCounter = 0;

    sc_out<bool> run;
    sc_out<bool> rst;
    sc_out<sc_uint<32>> data;

    sc_in<bool> hlt;

    tlm_utils::simple_initiator_socket<INIT> bus;

    void main() {
        run.write(false);
        rst.write(true);
        wait(PERIOD, SC_SEC);

        run.write(true);
        data.write(programCounter);
        rst.write(false);
        wait(PERIOD, SC_SEC);

        run.write(false);
        wait(PERIOD, SC_SEC);
    }

    void stop() {
        sc_core::sc_stop();
    }

    void load() {
        char buff;
        std::ifstream f;
        f.open(FILENAME, std::ios::binary);
        int i = 0;
        if (f.is_open()) {
            while (f.read((char*)(&buff), 1)) {
                if (i > MEM_SIZE)
                    break;
                tlm::tlm_generic_payload trans;
                trans.set_command(tlm::TLM_WRITE_COMMAND);
                trans.set_address(i);
                trans.set_data_ptr((unsigned char*)&buff);
                trans.set_data_length(sizeof(buff));
                sc_core::sc_time latency = sc_core::SC_ZERO_TIME;
                bus->b_transport(trans, latency);
                wait(latency);
                i++;
            }
            f.close();
        } else {
            std::cerr << "MEMORY: not f.is_open()";
        }
        main();
    }


    INIT(const sc_core::sc_module_name &name, uint32_t programCounter) : sc_module(name),  programCounter(programCounter) {
        // SC_METHOD(main);
        // sensitive << ready;
        // dont_initialize();

        SC_METHOD(stop);
        sensitive << hlt.pos();
        dont_initialize();

        SC_THREAD(load);
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

    Memory memory("MEMORY");

    Bus bus("Bus");
    bus.memory_socket(memory.socket);

    CPU cpu("CPU");
    cpu.run(run);
    cpu.rst(rst);
    cpu.clk(clk);
    cpu.hlt(hlt);
    cpu.bus(bus.cpu_memory_socket);

    INIT init("INIT", 16);
    init.rst(rst);
    init.run(run);
    init.hlt(hlt);
    init.bus(bus.init_memory_socket);
    init.data(cpu.busDataInPC);
    sc_start(100, SC_SEC);

    return 0;
}
