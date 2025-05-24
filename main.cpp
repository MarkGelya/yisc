#include "systemc.h"

#include "components/clock_gen/clock_generator.cpp"
#include "components/cpu/instr_decoder.cpp"
#include "components/cpu/execution_unit.cpp"
#include "components/cpu/alu.cpp"

#include "components/cpu/execution_unit.h"


int sc_main(int, char*[]) {
  // period, high %, start at, start value.
  sc_clock main_clk("clk", 2, SC_SEC, 0.5, 0.0, SC_SEC, false);

  sc_signal<bool> clk;
  sc_signal<bool> hlt;
  sc_signal<bool> run;

  sc_signal<sc_bv<32>> instr_bus;
  sc_signal<sc_bv<5>> instrOp_bus;
  sc_signal<sc_bv<3>> instrF3_bus;
  sc_signal<sc_bv<7>> instrF7_bus;
  sc_signal<sc_bv<5>> instrRd_bus;
  sc_signal<sc_bv<5>> instrRs1_bus;
  sc_signal<sc_bv<5>> instrRs2_bus;

  sc_signal<sc_uint<1>> aluSrc_bus;
  sc_signal<sc_uint<3>> aluCmd_bus;
  sc_signal<sc_uint<32>> aluSrcA_bus;
  sc_signal<sc_uint<32>> aluSrcB_bus;
  sc_signal<sc_uint<32>> aluSrcRes_bus;

  sc_signal<bool> regWriteEn;

  hlt.write(0);
  run.write(1);

  CLOCK_GENERATOR clock("CLOCK_GENERATOR");
  clock.main_clk(main_clk);
  clock.clk(clk);
  clock.hlt(hlt);
  clock.run(run);

  INSTR_DECODER instr_decoder("INSTR_DECODER");
  instr_decoder.clk(clk);
  instr_decoder.instr(instr_bus);
  instr_decoder.opcode(instrOp_bus);
  instr_decoder.funct3(instrF3_bus);
  instr_decoder.funct7(instrF7_bus);
  instr_decoder.rd(instrRd_bus);
  instr_decoder.rs1(instrRs1_bus);
  instr_decoder.rs2(instrRs2_bus);

  EXECUTION_UNIT eu("EXECUTION_UNIT");
  eu.clk(clk);
  eu.opcode(instrOp_bus);
  eu.funct3(instrF3_bus);
  eu.funct7(instrF7_bus);
  eu.aluSrc(aluSrc_bus);
  eu.aluCmd(aluCmd_bus);
  eu.regWriteEn(regWriteEn);

  ALU alu("ALU");
  alu.aluCmd(aluCmd_bus);
  alu.srcA(aluSrcA_bus);
  alu.srcB(aluSrcB_bus);
  alu.srcRes(aluSrcRes_bus);

  uint32_t instr = RV32_FT7_FT3_OP_ADD;
  instr_bus.write(instr);
  aluSrcA_bus.write(3);
  aluSrcB_bus.write(4);

  sc_start(10, SC_SEC);

  std::cout << aluSrcRes_bus.read().to_uint() << std::endl;

  return 0;
}

