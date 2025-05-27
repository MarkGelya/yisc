#include "cpu.h"

SC_HAS_PROCESS(CPU);

CPU::CPU(const sc_core::sc_module_name &name) : sc_module(name) {
    busReady.write(false);
    PC = 0;

    SC_THREAD(main)
    sensitive << clk << busReady;

    SC_THREAD(bus_thread)
    sensitive << clk << busEnable;
}

void CPU::instr_ld() {
    std::cout << "LD" << std::endl;
    busRdWr.write(0);
    busAddr.write(PC + (int32_t)regs.regs[GET_RS1(instrReg)] + GET_IMM(instrReg));
    wait(clk->posedge_event());
    busEnable.write(true);
    wait(clk->posedge_event());
    busEnable.write(false);
    if (!busReady.read())
        wait(busReady.posedge_event());
    regs.regs[GET_RD(instrReg)] = busDataIn.read();
    state = State::InstrFetch;
}


void CPU::instr_sd() {
    std::cout << "SD" << std::endl;
    busRdWr.write(1);
    busAddr.write(PC + (int32_t)regs.regs[GET_RS1(instrReg)] + GET_IMM(instrReg));
    busDataOut.write(regs.regs[GET_RS2(instrReg)]);
    wait(clk->posedge_event());
    busEnable.write(true);
    wait(clk->posedge_event());
    busEnable.write(false);
    if (!busReady.read())
        wait(busReady.posedge_event());
    state = State::InstrFetch;
}

void CPU::instr_add() {
    uint32_t a = regs.regs[GET_RS1(instrReg)];
    uint32_t b = regs.regs[GET_RS2(instrReg)];
    wait(clk->posedge_event());
    regs.regs[GET_RD(instrReg)] = a + b;
    state = State::InstrFetch;
}

void CPU::instr_hlt() {
    std::cout << "HLT" << std::endl;
    hlt->write(true);
    state = State::HLT;
}

void CPU::state_instrFetch() {
    busRdWr.write(0);
    busAddr.write(PC);
    wait(clk->posedge_event());
    busEnable.write(true);
    wait(clk->posedge_event());
    busEnable.write(false);
    if (!busReady.read())
        wait(busReady.posedge_event());
    instrReg = busDataIn.read();
    state = State::Execute;
    PC += 4;
}

void CPU::state_execute() {
    switch (instrReg & INSTR_OPCODE_MASK) {
        case INSTR_LD:
            instr_ld();
            break;
        case INSTR_SD:
            instr_sd();
            break;
        case INSTR_ADD:
            instr_add();
            break;
        case INSTR_HLT:
            instr_hlt();
            break;
    }
}

void CPU::main() {
    while (true) {
        wait(clk->posedge_event());
        switch (state) {
            case State::InstrFetch:
                std::cout << "State::InstrFetch" << endl;
                state_instrFetch();
                break;
            case State::Execute:
                std::cout << "State::Execute" << endl;
                state_execute();
                break;
            case State::HLT:
                std::cout << "State::HLT" << endl;
                if (run->read())
                    state = State::InstrFetch;
                break;
        }
    }
}

void CPU::bus_thread() {
    while (true) {
        wait(busEnable.posedge_event());
        tlm::tlm_generic_payload trans;
        uint32_t buff;
        if (busRdWr.read() == 0) {
            trans.set_command(tlm::TLM_READ_COMMAND);
        } else {
            trans.set_command(tlm::TLM_WRITE_COMMAND);
            buff = busDataOut.read();
        }
        trans.set_address(busAddr.read());
        trans.set_data_ptr((unsigned char*)&buff);
        trans.set_data_length(sizeof(buff));
        sc_core::sc_time latency = sc_core::SC_ZERO_TIME;
        memory->b_transport(trans, latency);
        wait(latency);
        busDataIn.write(buff);
        if (trans.get_response_status() != tlm::TLM_OK_RESPONSE)
            SC_REPORT_FATAL("Write", "Bad response\n");
        busReady.write(true);
        wait(clk->posedge_event());
        busReady.write(false);
    }
}

