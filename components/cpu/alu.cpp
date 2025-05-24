#include "systemc.h"

#include "alu.h"
#include "registers.h"

SC_MODULE(ALU) {
public:
    sc_in<sc_uint<32>> srcA;
    sc_in<sc_uint<32>> srcB;
    sc_in<sc_uint<3>> aluCmd;

    sc_out<sc_uint<32>> srcRes;

    SC_CTOR(ALU) {
        SC_METHOD(main);
        sensitive << srcA << srcB << aluCmd;
        dont_initialize();
    }

private:
    RV32Reg registers;
    void main();
};

void ALU::main() {
    std::cout << "ALU" << endl;
    switch(aluCmd.read()) {
        case ALU_CMD::OR:
            srcRes.write(srcA.read() || srcB.read());
            break;
        case ALU_CMD::AND:
            srcRes.write(srcA.read() && srcB.read());
            break;
        case ALU_CMD::XOR:
            srcRes.write(srcA.read() ^ srcB.read());
            break;
        case ALU_CMD::ADD:
            srcRes.write(srcA.read() + srcB.read());
            break;
        case ALU_CMD::SUB:
            srcRes.write(srcA.read() - srcB.read());
            break;
    }
}

