/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_SYS_TIMER_H
#define MBED_SYS_TIMER_H

#include "platform/NonCopyable.h"
#include "platform/mbed_atomic.h"
#include "drivers/TimerEvent.h"
#include "cmsis.h"

extern "C" {
#if defined(TARGET_CORTEX_A)
#include "irq_ctrl.h"
#endif
}

namespace mbed {
namespace internal {

/**
 * @cond MBED_INTERNAL
 *
 * @addtogroup mbed
 * @{
 *
 * @defgroup mbed_SysTimer SysTimer class
 * @{
 */

/**
 * The SysTimer class is used to provide timing for system suspension, and
 * the idle loop in TICKLESS mode.
 *
 * Template for speed for testing - only one instance will be used normally.
 *
 * @note SysTimer is not the part of Mbed API.
 */
template <uint32_t US_IN_TICK, bool IRQ = true>
class SysTimer: private mbed::TimerEvent, private mbed::NonCopyable<SysTimer<US_IN_TICK, IRQ> > {
public:

    /**
     * Default constructor uses LPTICKER if available (so the timer will
     * continue to run in deep sleep), else USTICKER.
     */
    SysTimer();

    SysTimer(const ticker_data_t *data);

    ~SysTimer();

    /**
     * Get the interrupt number for the tick
     *
     * @return interrupt number
     */
#if TARGET_CORTEX_A
    static IRQn_ID_t get_irq_number();
#elif TARGET_CORTEX_M
    static IRQn_Type get_irq_number();
#endif

    /**
     * Set the wake time
     *
     * Schedules an interrupt to cause wake-up in time for the event. Interrupt
     * may be arranged early to account for latency. If the time has already
     * passed, no interrupt will be scheduled.
     *
     * This is called from outside a critical section, as it is known to be
     * a slow operation.
     *
     * If the wake time is already set, this is a no-op. But that check is racy,
     * which means wake_time_set() should be rechecked after taking a critical
     * section.
     *
     * As a side-effect, this clears the unacknowledged tick count - the caller
     * is expected to use update_and_get_tick() after the suspend operation.
     *
     * @param at Wake up tick
     * @warning If the ticker tick is already scheduled it needs to be cancelled first!
     */
    void set_wake_time(uint64_t at);

    /**
     * Check whether the wake time has passed
     *
     * This is a fast operation, based on checking whether the wake interrupt
     * has run.
     *
     * @return true if the specified wake tick has passed
     */
    bool wake_time_passed() const
    {
        return core_util_atomic_load_bool(&_wake_time_passed);
    }

    /**
     * Check whether wake timer is active
     *
     * @return true if the wake timer is active.
     */
    bool wake_time_set() const
    {
        return core_util_atomic_load_bool(&_wake_time_set);
    }

    /**
     * Cancel any pending wake
     */
    void cancel_wake();

    /**
     * Schedule an os tick to fire
     *
     * Ticks will be rescheduled automatically every tick until cancel_tick is called.
     *
     * A tick will be fired immediately if there are any unacknowledged ticks.
     *
     * @warning If a tick is already scheduled it needs to be cancelled first!
     */
    void start_tick();

    /**
     * Acknowledge an os tick
     *
     * This will queue another os tick immediately if the os is running slow
     */
    void acknowledge_tick();

    /**
     * Prevent any more scheduled ticks from triggering
     *
     * If called from OS tick context, there may be remaining unacknowledged ticks.
     */
    void cancel_tick();

    /**
     * Check whether ticker is active
     *
     * Each time the tick interrupt fires, it is automatically rescheduled,
     * so this will remain true once the tick is started, except during
     * processing.
     *
     * @return true if the ticker is active.
     */
    bool ticking() const
    {
        return core_util_atomic_load_bool(&_ticking);
    }

    /**
     * Check unacknowledged ticks
     *
     * Returns the count of how many times the OS timer has been queued minus
     * the number of times is has been acknowledged.
     *
     * get_tick() - unacknowledged_ticks() should equal the OS's tick count,
     * although such a calculation is not atomic if the ticker is currently running.
     *
     * @return number of unacknowledged ticks
     */
    int unacknowledged_ticks() const
    {
        return core_util_atomic_load_u8(&_unacknowledged_ticks);
    }

    /** Get the current tick count
     *
     * This count is updated by the ticker interrupt, if the ticker interrupt
     * is running. It the ticker interrupt is not running, update_and_get_tick()
     * should be used instead.
     *
     * This indicates how many ticks have been generated by the tick interrupt.
     * The os_timer should equal this number minus the number of unacknowledged ticks.
     *
     * @return The number of ticks since timer creation.
     */
    uint64_t get_tick() const;

    /** Update and get the current tick count
     *
     * This is a slow operation that reads the timer and adjusts for elapsed time.
     * Can only be used when the ticker is not running, as there is no IRQ
     * synchronization.
     *
     * This clears the unacknowledged tick counter - the caller is assumed to update
     * their timer based on this return.
     *
     * @return The number of ticks since timer creation.
     */
    uint64_t update_and_get_tick();

    /**
     * Returns time since last tick
     *
     * @return Relative time in microseconds
     */
    us_timestamp_t get_time_since_tick() const;

    /**
     * Get the time
     *
     * Returns the instantaneous precision time from underlying timer.
     * This is a slow operation so should not be called from critical sections.
     *
     * @return Current time in microseconds
     */
    us_timestamp_t get_time() const;

protected:
    virtual void handler();
    void _increment_tick();
    void _schedule_tick();
    uint64_t _elapsed_ticks() const;
    static void _set_irq_pending();
    static void _clear_irq_pending();
    us_timestamp_t _time_us;
    uint64_t _tick;
    uint8_t _unacknowledged_ticks;
    bool _wake_time_set;
    bool _wake_time_passed;
    bool _wake_early;
    bool _ticking;
    bool _deep_sleep_locked;
};

/**
 * @}
 * @}
 * @endcond
 */

}
}

#endif
