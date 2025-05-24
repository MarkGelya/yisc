#pragma once

#include "systemc.h"

#include "instr_decoder.h"
#include "execution_unit.h"
#include "reg_file.h"
#include "alu.h"
#include "prog_counter.h"
#include "mux.h"

#include "execution_unit.h"
#include "tlm_utils/simple_initiator_socket.h"

class CPU : sc_core::sc_module {
public:
    sc_port<sc_signal_in_if<bool>> run;
    sc_port<sc_signal_in_if<bool>> rst;
    sc_port<sc_signal_in_if<bool>> clk;

    sc_port<sc_signal_out_if<bool>> hlt;

    tlm_utils::simple_initiator_socket<CPU> memory;

    sc_signal<sc_bv<32>> instr_bus;
    sc_signal<sc_bv<5>> instrOp_bus;
    sc_signal<sc_bv<3>> instrF3_bus;
    sc_signal<sc_bv<7>> instrF7_bus;
    sc_signal<sc_bv<12>> instrImmI_bus;
    sc_signal<sc_uint<32>> instrImmIExtTo32_bus;
    sc_signal<sc_bv<20>> instrImmU_bus;
    sc_signal<sc_uint<32>> instrImmUExtTo32_bus;
    sc_signal<sc_bv<20>> instrImmB_bus;

    sc_signal<sc_bv<5>> regFileA1;
    sc_signal<sc_bv<5>> regFileA2;
    sc_signal<sc_bv<5>> regFileAD;
    sc_signal<bool> regFileWriteEn;
    sc_signal<sc_uint<32>> regFileWriteData;
    sc_signal<sc_uint<1>> regWriteDataSel;
    sc_signal<sc_uint<32>> regFileRD1;
    sc_signal<sc_uint<32>> regFileRD2;

    sc_signal<sc_uint<1>> aluSrc_bus;
    sc_signal<sc_uint<3>> aluCmd_bus;
    sc_signal<sc_uint<32>> aluB_bus;
    sc_signal<sc_uint<32>> aluResult_bus;

    sc_signal<sc_uint<1>> progCounterSrc;

    sc_signal<sc_uint<32>> memAddr;

    CPU(const sc_core::sc_module_name &name);

private:
    INSTR_DECODER instr_decoder{"INSTR_DECODER"};
    EXECUTION_UNIT eu{"EXECUTION_UNIT"};
    REGISTER_FILE reg_file{"REGISTER_FILE"};
    ALU alu{"ALU"};
    MUX<sc_uint<32>> muxWD{"MUX_Write_data"};
    MUX<sc_uint<32>> muxALU{"MUX_ALU"};
    PROG_COUNTER prog_counter{"PROG_COUNTER"};

    void main();
    void extend_instrImmI_bus();
    void extend_instrImmU_bus();
};
