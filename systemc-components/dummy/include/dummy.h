/*
 * Copyright (c) 2022 GreenSocs
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _DUMMY_H
#define _DUMMY_H


#include <mutex>
#include <queue>
#include <iterator>
#include <map>
#include <vector>
#include <iostream>
#include <cstdint>

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
#include "dummy_regadd.h"
#include "RegisterIF.h"



class dummy : public sc_core::sc_module
{
    SCP_LOGGER();
private:
    std::string m_name;
    RegisterInterface regs;

private:
    void InitializeRegister()
    {
        regs.add_register("dummy_RESULT", 0x00, 0);
        regs.set_register_callback("dummy_RESULT", std::bind(&dummy::cb_dummy_RESULT, this, std::placeholders::_1, std::placeholders::_2));
    }

    void cb_dummy_RESULT(const std::string& name, uint32_t value)
    {
        if (name == "dummy_RESULT")
        {
            if (value == 0x0)
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
        }
    }


public:
    tlm_utils::simple_target_socket<dummy, DEFAULT_TLM_BUSWIDTH> socket;

    dummy(sc_core::sc_module_name name) :
        sc_core::sc_module(name),
        m_name(name),
        socket("target_socket")
    {
        socket.register_b_transport(this, &dummy::b_transport);
        InitializeRegister();
    }

    void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
        // Handle transaction

        switch (trans.get_command()) {
        case tlm::TLM_WRITE_COMMAND:
        {   
            unsigned int wdata = 0;
            std::memcpy(&wdata, trans.get_data_ptr(), sizeof(wdata));
            //regs[trans.get_address()] = wdata;
            regs.update_register(trans.get_address(), wdata);
            std::cout << m_name << ": Received transaction with address 0x" << std::hex << trans.get_address() << " data: 0x" << std::hex << wdata << std::dec << std::endl;
            trans.set_response_status(tlm::TLM_OK_RESPONSE);
        }
        case tlm::TLM_READ_COMMAND:
        {
            unsigned int rdata = 0;
            rdata =(unsigned int)regs[trans.get_address()].get_value();
            std::cout << m_name << ": Received transaction with address 0x" << std::hex << trans.get_address() << " data: 0x" << std::hex << rdata << std::dec << std::endl;
            std::memcpy(trans.get_data_ptr(), &rdata, trans.get_data_length());
            trans.set_response_status(tlm::TLM_OK_RESPONSE);
            break;
        }
        default:
            break;
        }
    }
};


extern "C" void module_register();
#endif