#include "memory.h"

SC_HAS_PROCESS(Memory);

Memory::Memory(sc_core::sc_module_name const &name, std::string const &filename) : sc_module(name), socket("socket"), LATENCY(sc_core::SC_ZERO_TIME) {
    socket.register_b_transport(this, &Memory::b_transport);
    readFile(filename);
}

Memory::Memory(sc_core::sc_module_name const &name) : sc_module(name), socket("socket"), LATENCY(sc_core::SC_ZERO_TIME) {
    socket.register_b_transport(this, &Memory::b_transport);
}

void Memory::b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
    tlm::tlm_command cmd = trans.get_command();
    uint64_t adr = trans.get_address();
    unsigned char *ptr = trans.get_data_ptr();
    unsigned int len = trans.get_data_length();

    if (adr >= (uint64_t)MEM_SIZE) {
        trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
        return;
    }

    if (cmd == tlm::TLM_READ_COMMAND) {
        // std::copy_n(mem + adr, len, ptr);
        std::cout << "TLM_READ_COMMAND" << std::endl;
    } else if (cmd == tlm::TLM_WRITE_COMMAND) {
        // std::copy_n(ptr, len, mem + adr);
        std::cout << "TLM_WRITE_COMMAND" << std::endl;
    }

    delay = sc_core::SC_ZERO_TIME;

    trans.set_response_status(tlm::TLM_OK_RESPONSE);
}

void Memory::readFile(std::string const &filename) {
    std::ifstream f;
    f.open(filename);
    if (f.is_open()) {
        f.close();
    } else {
        std::cerr << "MEMORY: not f.is_open()";
    }
}
