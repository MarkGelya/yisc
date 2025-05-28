#include "bus.h"


Bus::Bus(sc_core::sc_module_name const &name):
        sc_module(name),
        init_memory_socket("init_memory_socket"),
        cpu_memory_socket("cpu_memory_socket")
{
    init_memory_socket.register_b_transport(this, &Bus::b_transport);
    cpu_memory_socket.register_b_transport(this, &Bus::b_transport);
}

void Bus::b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
    memory_socket->b_transport(trans, delay);
}

