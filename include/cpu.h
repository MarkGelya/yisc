#pragma once

#include "systemc.h"

#include "execution_unit.h"
#include "registers.h"
#include "tlm_utils/simple_initiator_socket.h"

class CPU : sc_core::sc_module {
public:
    enum State {
        InstrFetch = 0,
        Execute,
        HLT,
    };

    sc_port<sc_signal_in_if<bool>> run;
    sc_port<sc_signal_in_if<bool>> rst;
    sc_port<sc_signal_in_if<bool>> clk;

    sc_port<sc_signal_out_if<bool>> hlt;

    tlm_utils::simple_initiator_socket<CPU> memory;

    // mem/io
    sc_signal<bool> busEnable;
    sc_signal<bool> busReady;
    sc_signal<sc_uint<1>> busRdWr;
    sc_signal<sc_uint<32>> busAddr;
    sc_signal<sc_uint<32>> busDataIn;
    sc_signal<sc_uint<32>> busDataOut;

    CPU(const sc_core::sc_module_name &name);

private:
    RV32Reg regs;
    uint32_t instrReg;
    uint32_t PC;
    State state;

    void main();
    void bus_thread();
    void extend_instrImmI_bus();
    void extend_instrImmU_bus();

    void state_instrFetch();
    void state_execute();

    void instr_ld();
    void instr_sd();
    void instr_add();
    void instr_hlt();
};
