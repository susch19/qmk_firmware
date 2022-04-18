#pragma once

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__AVR__)
#    include <util/delay.h>
#    define wait_ms(ms) _delay_ms(ms)
#    define wait_us(us) _delay_us(us)
#elif defined PROTOCOL_CHIBIOS
#    include <ch.h>
#define wait_ms(ms)                                                         \
    do {                                                                    \
        if (ms != 0) {                                                      \
            if(CH_CFG_ST_RESOLUTION < 32) {                                 \
                uint32_t limit = (UINT16_MAX / CH_CFG_ST_FREQUENCY) * 1000; \
                if(ms > limit) {                                            \
                    chThdSleepMilliseconds(limit);                          \
                }                                                           \
                else {                                                      \
                    chThdSleepMilliseconds(ms);                             \
                }                                                           \
            }                                                               \
            else {                                                          \
                chThdSleepMilliseconds(ms);                                 \
            }                                                               \
        } else {                                                            \
            chThdSleepMicroseconds(1);                                      \
        }                                                                   \
    } while (0)

#elif defined PROTOCOL_ARM_ATSAM
#    include "clks.h"
#    define wait_ms(ms) CLK_delay_ms(ms)
#    define wait_us(us) CLK_delay_us(us)
#else  // Unit tests
void wait_ms(uint32_t ms);
#    define wait_us(us) wait_ms(us / 1000)
#endif

#ifdef __cplusplus
}
#endif
