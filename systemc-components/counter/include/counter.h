/*
 * Copyright (c) 2022 GreenSocs
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _Counter_H
#define _Counter_H


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
#include "RegisterIF.h"

#define START_COUNTER 0x0
#define COUNT_VAL   0x04

class Counter : public sc_core::sc_module, RegisterInterface
{

    SCP_LOGGER();
public:
    tlm_utils::simple_target_socket<Counter, DEFAULT_TLM_BUSWIDTH> socket;

    Counter(sc_core::sc_module_name name)
        : socket("target_socket")
    {
        std::cout << "[Counter] constructor\n";
        socket.register_b_transport(this, &Counter::b_transport);
        SC_METHOD(update);
        sensitive << clk.pos();
        init_register();
        is_running = false;
    }

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
            update(addr, ptr);
            check_regs_status();
            break;
        }
        case tlm::TLM_READ_COMMAND:
        {
            break;
        }
        default:
            break;
        }
    };

    void init_register()
    {
        this->add_register("START_COUNTER", START_COUNTER , 0);
        this->add_register("COUNT_VAL", COUNT_VAL , 0);
        std::cout << "[Counter] Initialize registers done.\n";
    };

private:
    void update(uint64_t address, unsigned char* ptr)
    {
        uint32_t reg_temp =  (ptr[3] << 24) | (ptr[2] << 16) | (ptr[1] << 8) | ptr[0];
        update_register(address, reg_temp);
    };


    void update() {
        if(is_running)
        {
            if (reset.read()) {
                count = 0;
            } else {
                count++;
            }
            out.write(count);
            registers["COUNT_VAL"].set_value(count);
            std::cout << "Counter value at " << sc_core::sc_time_stamp() << " : " << count << std::endl;
        }
    };

    void check_regs_status()
    {
        if(registers["COUNT_VAL"].get_value() == 0x01)
        {
            is_running = true;
        }
        else
        {
            is_running = false;
        }
    };

public:
    sc_core::sc_in<bool> clk;      // Clock input
    sc_core::sc_in<bool> reset;    // Reset input
    sc_core::sc_out<int> out;      // Counter output
    int count;
    bool is_running;
};

#endif
