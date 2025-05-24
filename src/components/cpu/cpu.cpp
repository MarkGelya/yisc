#include "cpu.h"

SC_HAS_PROCESS(CPU);

CPU::CPU(const sc_core::sc_module_name &name) : sc_module(name) {
    instr_decoder.instr(instr_bus);
    instr_decoder.opcode(instrOp_bus);
    instr_decoder.funct3(instrF3_bus);
    instr_decoder.funct7(instrF7_bus);
    instr_decoder.rs1(regFileA1);
    instr_decoder.rs2(regFileA2);
    instr_decoder.rd(regFileAD);
    instr_decoder.immI(instrImmI_bus);
    instr_decoder.immU(instrImmU_bus);
    instr_decoder.immB(instrImmB_bus);

    eu.clk(clk);
    eu.opcode(instrOp_bus);
    eu.funct3(instrF3_bus);
    eu.funct7(instrF7_bus);
    eu.aluSrc(aluSrc_bus);
    eu.aluCmd(aluCmd_bus);
    eu.regWriteEn(regFileWriteEn);
    eu.regWriteDataSel(regWriteDataSel);

    reg_file.clk(clk);
    reg_file.reset(rst);
    reg_file.a1(regFileA1);
    reg_file.a2(regFileA2);
    reg_file.ad(regFileAD);
    reg_file.data(regFileWriteData);
    reg_file.writeEnable(regFileWriteEn);
    reg_file.rd1(regFileRD1);
    reg_file.rd2(regFileRD2);

    alu.aluCmd(aluCmd_bus);
    alu.srcA(regFileRD1);
    alu.srcB(aluB_bus);
    alu.result(aluResult_bus);

    muxWD.in0(aluResult_bus);
    muxWD.in1(instrImmUExtTo32_bus);
    muxWD.sel(regWriteDataSel);
    muxWD.out(regFileWriteData);

    muxALU.in0(regFileRD2);
    muxALU.in1(instrImmIExtTo32_bus);
    muxALU.sel(aluSrc_bus);
    muxALU.out(aluB_bus);

    prog_counter.clk(clk);
    prog_counter.rst(rst);
    prog_counter.pcSrc(progCounterSrc);
    prog_counter.immB(instrImmB_bus);
    prog_counter.addr(memAddr);

    SC_METHOD(extend_instrImmI_bus);
    sensitive << instrImmI_bus;
    dont_initialize();

    SC_METHOD(extend_instrImmU_bus)
    sensitive << instrImmU_bus;
    dont_initialize();

    SC_THREAD(main)
    sensitive << clk;
}

void CPU::main() {
    while (true) {
        wait(clk->posedge_event());
        tlm::tlm_generic_payload trans;
        trans.set_command(tlm::TLM_WRITE_COMMAND);
        trans.set_address(0x1234);
        trans.set_data_ptr((unsigned char*)0xC0FFEE);
        trans.set_data_length(1);
        sc_core::sc_time latency = sc_core::SC_ZERO_TIME;
        memory->b_transport(trans, latency);
        wait(latency);
        wait(1, SC_SEC);
        if (trans.get_response_status() != tlm::TLM_OK_RESPONSE)
            SC_REPORT_FATAL("Write", "Bad response\n");
    }

}

void CPU::extend_instrImmI_bus() {
    sc_bv<32> bv32_1;
    bv32_1.range(11, 0) = instrImmI_bus.read();
    bv32_1.range(31, 12) = 0;
    instrImmIExtTo32_bus.write(bv32_1);
}

void CPU::extend_instrImmU_bus() {
    sc_bv<32> bv32_2;
    bv32_2.range(19, 0) = instrImmU_bus.read();
    bv32_2.range(31, 20) = 0;
    instrImmUExtTo32_bus.write(bv32_2);
}


