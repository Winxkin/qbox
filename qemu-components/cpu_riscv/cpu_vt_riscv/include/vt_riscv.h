/*
 *  This file is part of libqbox
 *  Copyright (c) 2021 Greensocs
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <string>
#include <functional>

#include <libqemu-cxx/target/riscv.h>

#include <libgssync.h>
#include <module_factory_registery.h>
#include <irq-ctrl/plic_sifive/include/plic_sifive.h>

#include <cpu.h>

class vt_riscv64 : public QemuCpu
{
protected:
    uint64_t m_hartid;
    gs::async_event m_irq_ev;
    plic_sifive m_plic;

    void mip_update_cb(uint32_t value)
    {
        if (value) {
            m_irq_ev.notify();
        }
    }

public:

    static constexpr qemu::Target ARCH = qemu::Target::RISCV64;

    vt_riscv64(const sc_core::sc_module_name& name, QemuInstance& inst, const char* model, uint64_t hartid)
        : QemuCpu(name, inst, std::string(model) + "-riscv")
        , m_hartid(hartid)
        , m_plic("plic", inst)
        , m_irq_ev(true)
    {
        m_external_ev |= m_irq_ev;
    }

    void before_end_of_elaboration()
    {
        QemuCpu::before_end_of_elaboration();
        m_plic.before_end_of_elaboration();

        /* ensure the plic is also created */
        qemu::CpuRiscv64 cpu(get_qemu_dev());

        cpu.set_prop_int("hartid", m_hartid);
        cpu.set_mip_update_callback(std::bind(&vt_riscv64::mip_update_cb, this, std::placeholders::_1));
        
        /* setup cpu&plic links so that we can realize both objects */
        qemu::Device plic = m_plic.get_qemu_dev();
        cpu.set_prop_link("plic", plic);
        plic.set_prop_link("cpu", cpu);

    }
};

class vt_cpu_riscv64 : public vt_riscv64
{
public:
    vt_cpu_riscv64(const sc_core::sc_module_name& name, sc_core::sc_object* o, uint64_t hartid)
        : vt_cpu_riscv64(name, *(dynamic_cast<QemuInstance*>(o)), hartid)
    {
    }
    vt_cpu_riscv64(const sc_core::sc_module_name& n, QemuInstance& inst, uint64_t hartid)
        : vt_riscv64(n, inst, "rv64", hartid)
    {
    }
};

class vt_riscv32 : public QemuCpu
{
protected:
    uint64_t m_hartid;
    gs::async_event m_irq_ev;
    plic_sifive m_plic;

    void mip_update_cb(uint32_t value)
    {
        if (value) {
            m_irq_ev.notify();
        }
    }

public:

    static constexpr qemu::Target ARCH = qemu::Target::RISCV32;

    vt_riscv32(const sc_core::sc_module_name& name, QemuInstance& inst, const char* model, uint64_t hartid)
        : QemuCpu(name, inst, std::string(model) + "-riscv")
        , m_hartid(hartid)
        , m_plic("plic", inst)
        , m_irq_ev(true)
    {
        m_external_ev |= m_irq_ev;
    }

    void before_end_of_elaboration()
    {
        QemuCpu::before_end_of_elaboration();
        m_plic.before_end_of_elaboration();

        /* ensure the plic is also created */
        qemu::CpuRiscv64 cpu(get_qemu_dev());

        cpu.set_prop_int("hartid", m_hartid);
        cpu.set_mip_update_callback(std::bind(&vt_riscv32::mip_update_cb, this, std::placeholders::_1));
        
        /* setup cpu&plic links so that we can realize both objects */
        qemu::Device plic = m_plic.get_qemu_dev();
        cpu.set_prop_link("plic", plic);
        plic.set_prop_link("cpu", cpu);

    }
};

class vt_cpu_riscv32 : public vt_riscv32
{
public:
    vt_cpu_riscv32(const sc_core::sc_module_name& name, sc_core::sc_object* o, uint64_t hartid)
        : vt_cpu_riscv32(name, *(dynamic_cast<QemuInstance*>(o)), hartid)
    {
    }
    vt_cpu_riscv32(const sc_core::sc_module_name& n, QemuInstance& inst, uint64_t hartid)
        : vt_riscv32(n, inst, "rv32", hartid)
    {
    }
};

extern "C" void module_register();
