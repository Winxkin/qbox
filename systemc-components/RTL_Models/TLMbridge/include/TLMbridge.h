/*
 * Copyright (c) 2022 GreenSocs
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _TLMBRIDGE_H
#define _TLMBRIDGE_H


#include <mutex>
#include <queue>
#include <iterator>
#include <map>

#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include <systemc>
#include <cci_configuration>

#include <ports/initiator-signal-socket.h>
#include <async_event.h>
#include <ports/biflow-socket.h>
#include <module_factory_registery.h>
#include <scp/report.h>
#include <scp/helpers.h>
#include <tlm_sockets_buswidth.h>





class TLMbridge : public sc_core::sc_module
{
    SCP_LOGGER();
public:
    tlm_utils::simple_target_socket<TLMbridge, DEFAULT_TLM_BUSWIDTH> socket;

    TLMbridge(sc_core::sc_module_name name)
        : socket("target_socket")
    {
        std::cout << "[TLMbridge] constructor\n";
        socket.register_b_transport(this, &TLMbridge::b_transport);
    }

    TLMbridge() = delete;
    TLMbridge(const TLMbridge&) = delete;

    ~TLMbridge() {}

    void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay)
    {
        unsigned char* ptr = trans.get_data_ptr();
        uint64_t addr = trans.get_address();
        unsigned int len = trans.get_data_length();

        trans.set_dmi_allowed(false);
        trans.set_response_status(tlm::TLM_OK_RESPONSE);

        switch (trans.get_command()) {
        case tlm::TLM_WRITE_COMMAND:
        {
            std::cout << "[TLMbridge] TLM_WRITE_COMMAND     addr: " << addr << "    len: " << len << "\n";
            
            uint64_t data = *((uint64_t*)ptr);

            /*
                To convert data from transaction to wires binding directly to the input of RTL models
            */
            break;
        }
        case tlm::TLM_READ_COMMAND:
        {
            std::cout << "[TLMbridge] TLM_READ_COMMAND     addr: " << addr << "    len: " << len << "\n";

            /*
                To convert data from transaction to wires binding directly to the output of RTL models
            */
            break;
        }
        default:
            break;
        }
        
    };

public:

/*
    To declare in / out interfaces that are respective with RTL model in here 
*/

};


extern "C" void module_register();
#endif