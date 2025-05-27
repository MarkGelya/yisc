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
    uint64_t addr = trans.get_address();
    unsigned char *ptr = trans.get_data_ptr();
    unsigned int len = trans.get_data_length();

    if (addr >= (uint64_t)MEM_SIZE) {
        trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
        return;
    }

    if (cmd == tlm::TLM_READ_COMMAND) {
        std::copy_n(mem + addr, len, ptr);
        std::cout << "TLM_READ_COMMAND " << addr << std::endl;
    } else if (cmd == tlm::TLM_WRITE_COMMAND) {
        std::copy_n(ptr, len, mem + addr);
        std::cout << "TLM_WRITE_COMMAND" << addr << " " << (int)mem[addr] << std::endl;
    }

    delay = sc_core::SC_ZERO_TIME;

    trans.set_response_status(tlm::TLM_OK_RESPONSE);
}

void Memory::readFile(std::string const &filename) {
    std::ifstream f;
    f.open(filename, std::ios::binary);
    if (f.is_open()) {
        int i = 0;
        while (f.read((char*)(mem+i), 1)) {
            i++;
            if (i > MEM_SIZE)
                break;
        }
        f.close();
    } else {
        std::cerr << "MEMORY: not f.is_open()";
    }
}

Memory::~Memory() {
    std::ofstream f;
    f.open("dump.bin");
    for (uint8_t *ptr = mem; ptr < mem + MEM_SIZE; ptr++) {
        f.write((char*)ptr, 1);
    }
    f.close();
}

