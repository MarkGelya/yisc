#pragma once

#include <iostream>
#include <fstream>

#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"

#define MEM_SIZE 1024*1024

class Memory : sc_core::sc_module {
public:
    tlm_utils::simple_target_socket<Memory> socket;
    const sc_core::sc_time LATENCY;

    Memory(const sc_core::sc_module_name &name);
    Memory(sc_core::sc_module_name const &name, std::string const &filename);

    virtual void b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay);

    ~Memory();

private:
    uint8_t mem[MEM_SIZE];
    void readFile(const std::string &filename);
};
