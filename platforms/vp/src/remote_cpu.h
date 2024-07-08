/*
 * Quic Module Cortex-M55
 *
 * Copyright (c) 2023 Qualcomm Innovation Center, Inc. All Rights Reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __REMOTE_CORTEX_M55__
#define __REMOTE_CORTEX_M55__

#include <systemc>
#include <vector>
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
#include "dummy/include/dummy.h"
#include "counter/include/counter.h"

class RemoteCPU : public sc_core::sc_module
{
    SCP_LOGGER();

public:
    RemoteCPU(const sc_core::sc_module_name& n, sc_core::sc_object* obj)
        : RemoteCPU(n, *(dynamic_cast<QemuInstance*>(obj)))
    {
        if (obj == nullptr) {
            SCP_ERR(()) << "Received null object pointer!";
        } else {
            SCP_INFO(()) << "Object pointer is valid!";
        }
    }
    RemoteCPU(const sc_core::sc_module_name& n, QemuInstance& qemu_inst)
        : sc_core::sc_module(n)
        , m_broker(cci::cci_get_broker())
        , sysclk("systemclk",10, sc_core::SC_US)
        , m_gdb_port("gdb_port", 0, "GDB port")
        , m_qemu_inst(qemu_inst)
        , m_router("router")
        , m_cpu("cpu", m_qemu_inst, 0x0)
        , m_dummy("dummy")
        , m_counter("counter")
    {
        SCP_INFO(()) << "RemoteCPU constructor";

        // binding sockets
        
        m_cpu.socket.bind(m_router.target_socket);
        m_router.initiator_socket.bind(m_dummy.socket);
        m_router.initiator_socket.bind(m_counter.socket);

        // binding signal 
        m_counter.clk(sysclk);
        m_counter.reset(sig_sysrst);
        m_counter.out(sig_counter_out);
    }

private:
    // define signal here.
    sc_core::sc_signal<bool> sig_sysrst;
    sc_core::sc_signal<int> sig_counter_out;  

private:
    // define models here.
    cci::cci_broker_handle m_broker;
    cci::cci_param<int> m_gdb_port;
    QemuInstance& m_qemu_inst;
    gs::router<> m_router;
    cpu_riscv64 m_cpu;
    dummy m_dummy;
    sc_core::sc_clock sysclk;
    Counter m_counter;
};
GSC_MODULE_REGISTER(RemoteCPU, sc_core::sc_object*);
#endif
