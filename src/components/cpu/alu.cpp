#include "alu.h"

void ALU::main() {
    std::cout << "ALU" << endl;
    switch(aluCmd.read()) {
        case ALU_CMD::OR:
            result.write(srcA.read() || srcB.read());
            break;
        case ALU_CMD::AND:
            result.write(srcA.read() && srcB.read());
            break;
        case ALU_CMD::XOR:
            result.write(srcA.read() ^ srcB.read());
            break;
        case ALU_CMD::ADD:
            result.write(srcA.read() + srcB.read());
            break;
        case ALU_CMD::SUB:
            result.write(srcA.read() - srcB.read());
            break;
    }
}

