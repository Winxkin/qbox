/*
 * Copyright (c) 2022 GreenSocs
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _DUMMY_H
#define _DUMMY_H


#include <mutex>
#include <queue>

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



class dummy : public sc_core::sc_module
{
    SCP_LOGGER();
public:
    tlm_utils::simple_target_socket<dummy, DEFAULT_TLM_BUSWIDTH> socket;

    dummy(sc_core::sc_module_name name)
        : socket("target_socket")
    {
        SCP_TRACE(()) << "dummy constructor";
        socket.register_b_transport(this, &dummy::b_transport);
    }

    dummy() = delete;
    dummy(const dummy&) = delete;

    ~dummy() {}

    void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay)
    {
        unsigned char* ptr = trans.get_data_ptr();
        uint64_t addr = trans.get_address();
        unsigned int len = trans.get_data_length();

        trans.set_dmi_allowed(false);
        trans.set_response_status(tlm::TLM_OK_RESPONSE);

        switch (trans.get_command()) {
        case tlm::TLM_WRITE_COMMAND:
            SCP_TRACE(()) << "dummy TLM_WRITE_COMMAND";
            SCP_TRACE(()) << "-----------------------";
            SCP_TRACE(()) << "      TM is Pass       ";
            SCP_TRACE(()) << "-----------------------";
            exit(0);
            break;
        case tlm::TLM_READ_COMMAND:
            SCP_TRACE(()) << "dummy TLM_READ_COMMAND";
            SCP_TRACE(()) << "-----------------------";
            SCP_TRACE(()) << "      TM is Fail       ";
            SCP_TRACE(()) << "-----------------------";
            exit(0);
            break;
        default:
            break;
        }
    };

};


extern "C" void module_register();
#endif