#ifndef __RISCV64_CPU__
#define __RISCV64_CPU__

#include <systemc>
#include <tlm>
#include <cci_configuration>
#include <libgsutils.h>
#include <limits>
#include <cpu_arm/cpu_arm_cortex_m55/include/cortex-m55.h>
#include <cpu_riscv/cpu_riscv64/include/riscv64.h>
#include <cpu_riscv/cpu_vt_riscv/include/vt_riscv.h>
#include <qemu-instance.h>
#include "gs_memory/include/gs_memory.h"
#include "router/include/router.h"
#include "remote.h"
#include "pass/include/pass.h"
#include <module_factory_registery.h>
#include "RTL_Models/dummy/include/dummy.h"

class Riscv64CPU : public sc_core::sc_module
{
    SCP_LOGGER();

public:
    Riscv64CPU(const sc_core::sc_module_name& n, sc_core::sc_object* obj)
        : Riscv64CPU(n, *(dynamic_cast<QemuInstance*>(obj)))
    {
    }
    Riscv64CPU(const sc_core::sc_module_name& n, QemuInstance& qemu_inst)
        : sc_core::sc_module(n)
        // , sysclk("systemclk, ")
        , m_broker(cci::cci_get_broker())
        , m_gdb_port("gdb_port", 0, "GDB port")
        , m_qemu_inst(qemu_inst)
        , m_router("router")
        , m_cpu("cpu", m_qemu_inst, 64)
        , m_memory("memory", 0x10000)
    {
        unsigned int m_irq_num = m_broker.get_param_handle(std::string(this->name()) + ".cpu.nvic.num_irq")
                                     .get_cci_value()
                                     .get_uint();

        if (!m_gdb_port.is_default_value()) m_cpu.p_gdb_port = m_gdb_port;

        SCP_INFO(()) << "number of irqs  = " << m_irq_num;

        do_bus_binding();
    }

    void do_bus_binding()
    {
        m_router.initiator_socket.bind(m_cpu.m_plic.socket);
        m_cpu.socket.bind(m_router.target_socket);
        m_memory.socket.bind(m_router.target_socket);
    }

public:
    sc_core::sc_clock sysclk;

private:
    cci::cci_broker_handle m_broker;
    cci::cci_param<int> m_gdb_port;
    QemuInstance& m_qemu_inst;
    gs::router<> m_router;
    vt_cpu_riscv64 m_cpu;
    gs::gs_memory<> m_memory;
    
};
GSC_MODULE_REGISTER(Riscv64CPU, sc_core::sc_object*);
#endif