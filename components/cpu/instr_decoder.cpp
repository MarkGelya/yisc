#include "systemc.h"

SC_MODULE(INSTR_DECODER) {
public:
    sc_in<sc_bv<32>> instr;
    sc_in<bool> clk;

    sc_out<sc_bv<5>> opcode;
    sc_out<sc_bv<5>> rd;
    sc_out<sc_bv<5>> rs1;
    sc_out<sc_bv<5>> rs2;
    sc_out<sc_bv<3>> funct3;
    sc_out<sc_bv<7>> funct7;

    SC_CTOR(INSTR_DECODER) {
        SC_METHOD(main);
        sensitive << instr << clk;
        dont_initialize();
    }

private:
    void main();
};

void INSTR_DECODER::main() {
    opcode = instr.read().range(6, 2);
    rd = instr.read().range(11, 7);
    rs1 = instr.read().range(19, 15);
    rs2 = instr.read().range(25, 20);
    funct3 = instr.read().range(14, 12);
    funct7 = instr.read().range(31, 25);
}
