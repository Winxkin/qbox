/*
 * Copyright (c) 2022 GreenSocs
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _DUMMY_H
#define _DUMMY_H


#include <mutex>
#include <queue>
#include <iostream>

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
        std::cout << "[dummy] constructor\n";
        socket.register_b_transport(this, &dummy::b_transport);
        this->dummy_reg = 0;
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
        {
            std::cout << "[dummy] TLM_WRITE_COMMAND\n";
            uint32_t reg_temp =  (ptr[3] << 24) | (ptr[2] << 16) | (ptr[1] << 8) | ptr[0];
            this->update_reg(addr, reg_temp);
            this->check_dummy_reg();
            break;
        }
        case tlm::TLM_READ_COMMAND:
        {
            std::cout << "[dummy] TLM_READ_COMMAND\n";
            break;
        }
        default:
            break;
        }
    };
private:
    void update_reg(uint64_t addr, uint32_t value)
    {
        this->dummy_reg = value;
        std::cout << "[dummy] [update_reg] dummy_reg: " << dummy_reg << "\n";
    };

    void check_dummy_reg()
    {
        if(dummy_reg == 0)
        {
            std::cout << "-----------------------\n";
            std::cout << "      TM is Pass       \n";
            std::cout << "-----------------------\n";
            exit(0);
        }
        else
        {
            std::cout << "-----------------------\n";
            std::cout << "      TM is Fail       \n";
            std::cout << "-----------------------\n";
            exit(0);
        }
    };

    uint32_t dummy_reg;

};


extern "C" void module_register();
#endif