#include "systemc.h"

#include "registers.h"

SC_MODULE(REGISTER_FILE) {
public:
    sc_in<sc_uint<5>> a1;
    sc_in<sc_uint<5>> a2;
    sc_in<sc_uint<5>> a3;
    sc_in<bool> writeEnable;
    sc_in<bool> reset;
    sc_in<sc_uint<32>> data;

    SC_CTOR(REGISTER_FILE) {
        SC_METHOD(resetMethod);
        sensitive << reset.pos();
        dont_initialize();

        SC_METHOD(writeMethod);
        sensitive << writeEnable.pos();
        dont_initialize();
    }

private:
    RV32Reg registers;
    void resetMethod();
    void writeMethod();
};

void REGISTER_FILE::resetMethod() {
    std::memset(&registers, 0, sizeof(registers));
}

void REGISTER_FILE::writeMethod() {
    if (!reset.read()) {
        size_t reg_addr = a1.read();
        registers.regs[reg_addr] = 0;
    }
}
