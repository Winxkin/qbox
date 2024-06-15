/*
 *  GreenSocs base virtual platform
 *  Copyright (c) 2021 Greensocs
 *
 *  Author: Luc Michel
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <chrono>

#include <systemc>
#include <cci/utils/broker.h>
#include <tlm>
#include <cci_configuration>
#include <libgsutils.h>
#include <limits>
#include <qemu-instance.h>
#include <module_factory_registery.h>

#include <argparser.h>
#include <cciutils.h>

#include <cpu_riscv/cpu_riscv64/include/riscv64.h>
#include <irq-ctrl/plic_sifive/include/plic-sifive.h>
#include <irq-ctrl/riscv_aclint_swi/include/riscv-aclint-swi.h>
#include <timer/riscv_aclint_mtimer/include/riscv-aclint-mtimer.h>
#include <uart/16550/include/16550.h>

#include <router/include/router.h>
#include <gs_memory/include/gs_memory.h>
#include <loader.h>

#include <stdio.h>




class RiscvDemoPlatform : public sc_core::sc_module {

private:
    
    cci::cci_broker_handle m_broker;
    cci::cci_param<int> m_gdb_port;
    gs::gs_memory<> m_memory;
    gs::router<> m_router;
    QemuInstance& m_qemu_inst;
    cpu_riscv64 m_core_riscv64;

    void do_bus_binding()
    {

        m_core_riscv64.socket.bind(m_router.target_socket);
        m_router.initiator_socket.bind(m_memory.socket);

    }

    void setup_irq_mapping() {
        
    }

public:
    RiscvDemoPlatform(const sc_core::sc_module_name &n)
        : sc_core::sc_module(n)
        , m_broker(cci::cci_get_broker())
        , m_gdb_port("gdb_port", 0, "GDB port")
        , m_qemu_inst("qemu_inst", qemu::get_default_lib_loader(), )
        , m_router("router")
        , m_memory("memory",1024)
        , m_core_riscv64("core_riscv64", m_qemu_inst, 12)
        
    {
       
        
        do_bus_binding();
        setup_irq_mapping();
    }
};

int sc_main(int argc, char *argv[])
{
    gs::ConfigurableBroker m_broker{};
    cci::cci_originator orig{ "sc_main" };
    auto broker_h = m_broker.create_broker_handle(orig);
    ArgParser ap{ broker_h, argc, argv };

  
    RiscvDemoPlatform platform("platform");

    auto start = std::chrono::system_clock::now();
    try {
        SCP_INFO() << "SC_START";
        sc_core::sc_start();
    } catch (std::runtime_error const& e) {
        std::cerr << argv[0] << "Error: '" << e.what() << std::endl;
        exit(1);
    } catch (const std::exception& exc) {
        std::cerr << argv[0] << " Error: '" << exc.what() << std::endl;
        exit(2);
    } catch (...) {
        SCP_ERR() << "Local platform Unknown error (main.cc)!";
        exit(3);
    }
    auto end = std::chrono::system_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Simulation Time: " << sc_core::sc_time_stamp().to_seconds() << "SC_SEC" << std::endl;
    std::cout << "Simulation Duration: " << elapsed.count() << "s (Wall Clock)" << std::endl;

    return 0;
}
