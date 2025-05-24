#include "reg_file.h"

void REGISTER_FILE::resetMethod() {
    std::memset(&registers.regs, 0, sizeof(registers.regs));
}

void REGISTER_FILE::writeMethod() {
    if (!reset.read()) {
        size_t reg_addr = ad.read().to_uint();
        registers.regs[reg_addr] = 0;
    }
}

void REGISTER_FILE::readDataMethod() {
    rd1.write(registers.regs[a1.read().to_uint()]);
    rd2.write(registers.regs[a2.read().to_uint()]);
}
