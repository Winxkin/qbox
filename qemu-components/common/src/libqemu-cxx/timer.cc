/*
 * This file is part of libqemu-cxx
 * Copyright (c) 2022-2023 Qualcomm Innovation Center, Inc. All Rights Reserved.
 * Author: GreenSocs 2015-2019
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <libqemu/libqemu.h>

#include <libqemu-cxx/libqemu-cxx.h>
#include <internals.h>

namespace qemu {

Timer::Timer(std::shared_ptr<LibQemuInternals> internals): m_int(internals) {}

Timer::~Timer()
{
    del();

    if (m_timer != nullptr) {
        m_int->exports().timer_free(m_timer);
    }
}

static void timer_generic_callback(void* opaque)
{
    Timer::TimerCallbackFn* cb = reinterpret_cast<Timer::TimerCallbackFn*>(opaque);

    (*cb)();
}

void Timer::set_callback(TimerCallbackFn cb)
{
    m_cb = cb;
    m_timer = m_int->exports().timer_new_virtual_ns(timer_generic_callback, reinterpret_cast<void*>(&m_cb));
}

void Timer::mod(int64_t deadline)
{
    if (m_timer != nullptr) {
        m_int->exports().timer_mod_ns(m_timer, deadline);
    }
}

void Timer::del()
{
    if (m_timer != nullptr) {
        m_int->exports().timer_del(m_timer);
    }
}

} // namespace qemu
