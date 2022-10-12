/* Copyright 2020 QMK
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * This file was auto-generated by:
 *    `qmk chibios-confmigrate -i platforms/chibios/boards/SN_SN32F260/configs/chconf.h -r platforms/chibios/boards/common/configs/chconf.h`
 */

#pragma once

#define CH_CFG_ST_FREQUENCY 10000

#define CH_CFG_ST_TIMEDELTA 0

#define CH_CFG_OPTIMIZE_SPEED FALSE

#define CH_CFG_USE_TIMESTAMP FALSE

#define CH_CFG_USE_SEMAPHORES FALSE

#define CH_CFG_USE_MUTEXES FALSE

#define CH_CFG_USE_CONDVARS_TIMEOUT FALSE

#define CH_CFG_USE_MESSAGES TRUE

#define CH_CFG_USE_MAILBOXES TRUE

#define CH_CFG_USE_MEMCORE FALSE

#define CH_CFG_IDLE_ENTER_HOOK() {                                          \
    SN_PMU->CTRL = 4;                                                       \
}

#define CH_CFG_IDLE_LEAVE_HOOK() {                                          \
    SN_PMU->CTRL = 0;                                                       \
}
#define PORT_IDLE_THREAD_STACK_SIZE     0
#define PORT_INT_REQUIRED_STACK         0
#define RAW_IN_CAPACITY                 1
#define RAW_OUT_CAPACITY                1

/* can't call sleep without the idle thread, must override related functions */
#if CH_CFG_NO_IDLE_THREAD == TRUE
        #pragma weak chThdSleep
        #pragma weak chThdSuspendTimeoutS
#endif

#include_next <chconf.h>

