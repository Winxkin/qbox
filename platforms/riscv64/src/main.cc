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

protected:
    // cci::cci_param<unsigned> p_riscv_num_cpus;
    // cci::cci_param<int> p_log_level;

    // gs::ConfigurableBroker m_broker;

    // cci::cci_param<int> m_quantum_ns;
    // cci::cci_param<int> m_gdb_port;

    // QemuInstanceManager m_inst_mgr;
    // // QemuInstance &m_qemu_inst;
    // QemuCpuRiscv64 m_cpus;

    // gs::router<> m_router;

    /*
     * QEMU RISC-V cpu boots at 0x1000. We put a small rom here to jump into
     * the bootrom.
    //  */
    // gs::gs_memory<> m_resetvec_rom;

    // gs::gs_memory<> m_rom;
    // gs::gs_memory<> m_sram;
    // gs::gs_memory<> m_dram;

    // /* Modeled as memory */
    // gs::gs_memory<> m_qspi;
    // gs::gs_memory<> m_boot_gpio;

    // uart_16550 m_uart;

    // gs::gs_memory<> m_fallback_mem;

    // gs::loader<> m_loader;

    void do_bus_binding()
    {

        // m_cpus.socket.bind(m_router.target_socket);


        // m_router.initiator_socket.bind(m_resetvec_rom.socket);
        // m_router.initiator_socket.bind(m_rom.socket);
        // m_router.initiator_socket.bind(m_sram.socket);
        // m_router.initiator_socket.bind(m_dram.socket);
        // m_router.initiator_socket.bind(m_qspi.socket);
        // m_router.initiator_socket.bind(m_boot_gpio.socket);
        // m_router.initiator_socket.bind(m_uart.socket);
        

        // General loader
        // m_loader.initiator_socket.bind(m_router.target_socket);

        // MUST be added last
        // m_router.initiator_socket.bind(m_fallback_mem.socket);
    }

    void setup_irq_mapping() {
        // if (p_riscv_num_cpus)
        // {
        //     int irq = gs::cci_get<int>(std::string(m_uart.name()) + ".irq");
        // }
    }

public:
    RiscvDemoPlatform(const sc_core::sc_module_name &n)
        : sc_core::sc_module(n)
        // , p_riscv_num_cpus("riscv_num_cpus", 1, "Number of RISCV cores")
        // , p_log_level("log_level", 2, "Default log level")
        // , m_broker({
        //            {"plic.num_sources", cci::cci_value(280)},
        //            {"plic.num_priorities", cci::cci_value(7)},
        //            {"plic.priority_base", cci::cci_value(0x04)},
        //            {"plic.pending_base", cci::cci_value(0x1000)},
        //            {"plic.enable_base", cci::cci_value(0x2000)},
        //            {"plic.enable_stride", cci::cci_value(0x80)},
        //            {"plic.context_base", cci::cci_value(0x200000)},
        //            {"plic.context_stride", cci::cci_value(0x1000)},
        //            {"plic.aperture_size", cci::cci_value(0x4000000)},
        //            {"plic.hart_config", cci::cci_value("MS,MS,MS,MS,MS")},

        //            {"uart.regshift", cci::cci_value(2)},
        //            {"uart.baudbase", cci::cci_value(38400000)},

        //            {"swi.num_harts", cci::cci_value(5)},

        //            {"mtimer.timecmp_base", cci::cci_value(0x0)},
        //            {"mtimer.time_base", cci::cci_value(0xbff8)},
        //            {"mtimer.provide_rdtime", cci::cci_value(true)},
        //            {"mtimer.aperture_size", cci::cci_value(0x10000)},
        //            {"mtimer.num_harts", cci::cci_value(5)},
        //            {"mtimer.timebase_freq", cci::cci_value{1000000}},
        // })
        // , m_quantum_ns("quantum_ns", 1000000, "TLM-2.0 global quantum in ns")
        // , m_gdb_port("gdb_port", 0, "GDB port")
        // , m_loader("load")
        // , m_qemu_inst("qemu_inst", nullptr, qemu::Target::RISCV64)
        // , m_cpus("cpu", m_qemu_inst , "64" , 64)
        // , m_router("router")
        // , m_resetvec_rom("resetvec_rom", 1024)
        // , m_rom("rom", 1024)
        // , m_sram("sram", 1024)
        // , m_dram("dram", 1024)
        // , m_qspi("qspi", 1024)
        // , m_boot_gpio("boot_gpio", 1024)
        // , m_uart("uart", m_qemu_inst)
        // , m_fallback_mem("fallback_memory")
        
    {
        // using tlm_utils::tlm_quantumkeeper;

        // int level = p_log_level.get_value();
        // scp::set_logging_level(scp::as_log(level));

        // sc_core::sc_time global_quantum(m_quantum_ns, sc_core::SC_NS);
        // tlm_quantumkeeper::set_global_quantum(global_quantum);

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
