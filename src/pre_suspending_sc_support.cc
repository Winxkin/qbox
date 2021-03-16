/*
 * Copyright (C) 2020 GreenSocs
 */

#ifndef SC_INCLUDE_DYNAMIC_PROCESSES
#define SC_INCLUDE_DYNAMIC_PROCESSES
#endif
#include <systemc>

#include "greensocs/libgssync/pre_suspending_sc_support.h"
#include <greensocs/libgsutils.h>

#ifndef SC_HAS_SUSPENDING

namespace gs {

    global_pause gp;
    unsigned int gp_ref_count = 0;

    void global_pause::sleeper() {
        if (m_has_suspending_channels == 0 && m_suspend == 0) {
            GS_LOG("no suspending");
            return;
        }

        if (m_suspend > 0 && m_unsuspendable > 0 && sc_core::sc_pending_activity()) {
            // wait till we can suspend.
            GS_LOG("unsuspendable %d", m_unsuspendable);
            return sleeper_event.notify(sc_core::sc_time_to_pending_activity());
        }

        if (m_suspend == 0 && m_has_suspending_channels > 0 && sc_core::sc_pending_activity()) {
            GS_LOG("suspending channels wait for idle");
            return sleeper_event.notify(sc_core::sc_time_to_pending_activity());
        }

        // wait till there are no other pending events in this delta,
        // then suspend
        if (sc_core::sc_pending_activity_at_current_time()) {
            GS_LOG("waiting for idle");
            sleeper_event.notify(sc_core::SC_ZERO_TIME);
        } else {
            GS_LOG("%s Suspended", sc_core::sc_time_stamp().to_string().c_str());
            std::unique_lock<std::mutex> lock(mutex);
            cond.wait(lock, [this] {
                return wakeups > 0
                       || sc_core::sc_time_to_pending_activity() == sc_core::SC_ZERO_TIME
                       || m_suspend == 0
                       || m_unsuspendable > 0;
            });
            if (wakeups) {
                wakeups--;
            }
            GS_LOG("%s Wake", sc_core::sc_time_stamp().to_string().c_str());
            if (m_suspend > 0 || m_has_suspending_channels > 0) {
                GS_LOG("Loop again");
                // could optimise for the suspending channels
                // e.g. sleeper_event.notify(sc_core::sc_time_to_pending_activity());
                sleeper_event.notify(sc_core::SC_ZERO_TIME);
            }
        }
    }

    void global_pause::suspendable() {
        sc_core::sc_process_b *process_p = (sc_core::sc_process_b *) sc_core::sc_get_current_process_handle();
        assert(process_p);

        if (m_unsuspendable_map[process_p]) {
            m_unsuspendable_map[process_p] = false;

            assert(m_unsuspendable);
            m_unsuspendable--;

            if(log_enabled) {
                if (process_p && process_p->get_parent_object())
                    GS_LOG("suspendable() %d %s", m_unsuspendable, process_p->get_parent_object()->name());
                else
                    GS_LOG("suspendable() %d none", m_unsuspendable);
            }
        }
        sleeper_event.notify(sc_core::SC_ZERO_TIME);
    }

    void global_pause::unsuspendable() {
        sc_core::sc_process_b *process_p = (sc_core::sc_process_b *) sc_core::sc_get_current_process_handle();
        assert(process_p);

        if (!m_unsuspendable_map[process_p]) {
            m_unsuspendable_map[process_p] = true;

            m_unsuspendable++;

            if (log_enabled) {
                if (process_p && process_p->get_parent_object())
                    GS_LOG("unsuspendable() %d %s", m_unsuspendable, process_p->get_parent_object()->name());
                else
                    GS_LOG("unsuspendable() %d none", m_unsuspendable);
            }
        }
    }

    void global_pause::unsuspend_all() {
        sc_core::sc_process_b *process_p = (sc_core::sc_process_b *) sc_core::sc_get_current_process_handle();
        if (process_p) {
            if (m_suspend_all_req_map[process_p]) {
                m_suspend_all_req_map[process_p] = false;
            } else {
                return;
            }
        }
        // update global counters
        assert (m_suspend > 0);
        m_suspend--;

        if (log_enabled) {
            if (process_p && process_p->get_parent_object())
                GS_LOG("unsuspend_all() %d %s", m_suspend, process_p->get_parent_object()->name());
            else
                GS_LOG("unsuspend_all() %d none", m_suspend);
        }
    }

    void global_pause::suspend_all() {
        sc_core::sc_process_b *process_p = (sc_core::sc_process_b *) sc_core::sc_get_current_process_handle();
        if (process_p) {
            if (!m_suspend_all_req_map[process_p]) {
                m_suspend_all_req_map[process_p] = true;
            } else {
                return;
            }
        }
        // update global counters
        m_suspend++;
        sleeper_event.notify(sc_core::SC_ZERO_TIME);

        if (log_enabled) {
            if (process_p && process_p->get_parent_object())
                GS_LOG("suspend_all() %d %s", m_suspend, process_p->get_parent_object()->name());
            else
                GS_LOG("suspend_all() %d none", m_suspend);
        }
    }

    bool global_pause::attach_suspending(sc_core::sc_prim_channel *p) {
        GS_LOG("attach_suspending %d", m_suspending_channels.size());
        assert(p);
        std::vector<const sc_core::sc_prim_channel *>::iterator it =
                std::find(m_suspending_channels.begin(), m_suspending_channels.end(), p);
        if (it == m_suspending_channels.end()) {
            m_suspending_channels.push_back(p);
            m_has_suspending_channels = true;
            return true;
        }
        return false;
    }

    bool global_pause::detach_suspending(sc_core::sc_prim_channel *p) {
        GS_LOG("detach_suspending() %d", m_suspending_channels.size());
        assert(p);
        std::vector<const sc_core::sc_prim_channel *>::iterator it =
                std::find(m_suspending_channels.begin(), m_suspending_channels.end(), p);
        if (it != m_suspending_channels.end()) {
            *it = m_suspending_channels.back();
            m_suspending_channels.pop_back();
            m_has_suspending_channels = (m_suspending_channels.size() > 0);
            return true;
        }
        return false;
    }

    void global_pause::async_wakeup() {
        std::lock_guard<std::mutex> lock(mutex);
        GS_LOG("async_wakeup()");
        wakeups++;
        cond.notify_all();
    }

    global_pause& global_pause::get() {
        return gp;
    }

    global_pause::global_pause() {
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (gp_ref_count > 0) {
                std::cerr << "global_pause is a singleton, should only be instantiated once." << std::endl;
                std::abort();
            }
            gp_ref_count++;
        }
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&sleeper_event);
        opt.dont_initialize();
        // nb sc_bind used to be only defined in the global namespace!*/
        sc_core::sc_spawn(sc_bind(&global_pause::sleeper, this), "global_pause_sleeper", &opt);
    }
}

namespace sc_core {
    void sc_suspend_all() {
         gs::global_pause::get().suspend_all();
    }

    void sc_unsuspend_all() {
        gs::global_pause::get().unsuspend_all();
    }

    void sc_suspendable() {
        gs::global_pause::get().suspendable();
    }

    void sc_unsuspendable() {
        gs::global_pause::get().unsuspendable();
    }

    void sc_internal_async_wakeup() {
        gs::global_pause::get().async_wakeup();
    }

    #ifndef SC_HAS_ASYNC_ATTACH_SUSPENDING
        /* Provide a 'global' attach/detach mechanism */
        bool async_attach_suspending(sc_core::sc_prim_channel *p) {
            gs::global_pause::get().attach_suspending(p);
        }
        bool async_detach_suspending(sc_core::sc_prim_channel *p) {
            gs::global_pause::get().detach_suspending(p);
        }
    #endif
}

#endif
