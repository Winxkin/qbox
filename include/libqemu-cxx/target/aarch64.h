/*
 *  This file is part of libqemu-cxx
 *  Copyright (C) 2015-2019  GreenSocs
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "libqemu-cxx/libqemu-cxx.h"

namespace qemu {

class CpuArm : public Cpu
{
public:
    static constexpr const char* const TYPE = "arm-cpu";

    CpuArm() = default;
    CpuArm(const CpuArm&) = default;
    CpuArm(const Object& o): Cpu(o) {}

    void set_cp15_cbar(uint64_t cbar);
    void add_nvic_link();

    uint64_t get_exclusive_addr() const;
    uint64_t get_exclusive_val() const;
    void set_exclusive_val(uint64_t val);
};

class CpuAarch64 : public CpuArm
{
public:
    static constexpr const char* const TYPE = "arm-cpu";

    CpuAarch64() = default;
    CpuAarch64(const CpuAarch64&) = default;
    CpuAarch64(const Object& o): CpuArm(o) {}

    void set_aarch64_mode(bool aarch64_mode);
};

class ArmNvic : public Device
{
public:
    static constexpr const char* const TYPE = "armv7m_nvic";

    ArmNvic() = default;
    ArmNvic(const ArmNvic&) = default;
    ArmNvic(const Object& o): Device(o) {}

    void add_cpu_link();
};

} // namespace qemu
