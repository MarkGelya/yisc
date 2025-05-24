#include "instr_decoder.h"

void INSTR_DECODER::main() {
    opcode = instr.read().range(6, 2);
    rd = instr.read().range(11, 7);
    rs1 = instr.read().range(19, 15);
    rs2 = instr.read().range(25, 20);
    funct3 = instr.read().range(14, 12);
    funct7 = instr.read().range(31, 25);
    immI = instr.read().range(31, 20);
    immU = instr.read().range(31, 12);
    immB = instr.read().range(31, 12);
}
