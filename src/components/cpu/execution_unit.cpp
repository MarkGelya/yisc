#include "systemc.h"

#include "alu.h"
#include "execution_unit.h"

void EXECUTION_UNIT::main() {
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
