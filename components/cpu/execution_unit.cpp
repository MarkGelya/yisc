#include "systemc.h"

#include "alu.h"
#include "registers.h"
#include "execution_unit.h"

SC_MODULE(EXECUTION_UNIT) {
public:
    sc_in<bool> clk;
    sc_in<sc_bv<5>> opcode;
    sc_in<sc_bv<3>> funct3;
    sc_in<sc_bv<7>> funct7;

    sc_out<bool> regWriteEn;
    sc_out<sc_uint<1>> aluSrc;
    sc_out<sc_uint<3>> aluCmd;

    SC_CTOR(EXECUTION_UNIT) {
        SC_METHOD(main);
        sensitive << clk.pos() << opcode << funct3 << funct7;
        dont_initialize();
    }

private:
    RV32Reg registers;
    void main();
};

void EXECUTION_UNIT::main() {
    std::cout << "EXECUTION_UNIT" << endl;

    uint32_t op = opcode.read().to_uint();
    uint32_t f3 = funct3.read().to_uint();
    uint32_t f7 = funct7.read().to_uint();

    uint32_t instr = op << RV32_OP_OFFSET | f3 << RV32_FT3_OFFSET | f7 << RV32_FT7_OFFSET;
    aluSrc.write(0);
    switch(instr) {
        case RV32_FT7_FT3_OP_OR:
            aluCmd.write(ALU_CMD::OR);
            return;
        case RV32_FT7_FT3_OP_AND:
            aluCmd.write(ALU_CMD::AND);
            return;
        case RV32_FT7_FT3_OP_XOR:
            aluCmd.write(ALU_CMD::XOR);
            return;
        case RV32_FT7_FT3_OP_ADD:
            aluCmd.write(ALU_CMD::ADD);
            return;
        case RV32_FT7_FT3_OP_SUB:
            aluCmd.write(ALU_CMD::SUB);
            return;
    }

    instr = instr & RV32_FT3_MASK & RV32_OP_MASK;
    switch(instr) {
        case RV32_FT3_OP_ADDI:
            aluCmd.write(ALU_CMD::ADD);
            aluSrc.write(1);
            return;
    }

}
