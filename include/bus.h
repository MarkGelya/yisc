#pragma once

#include <iostream>
#include <fstream>

#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/simple_initiator_socket.h"

class Bus : sc_core::sc_module {
public:
    tlm_utils::simple_target_socket<Bus> init_memory_socket;
    tlm_utils::simple_target_socket<Bus> cpu_memory_socket;

    tlm_utils::simple_initiator_socket<Bus> memory_socket;

    Bus(sc_core::sc_module_name const &name);
    void b_transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay);
};

