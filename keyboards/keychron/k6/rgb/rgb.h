#pragma once

#include "quantum.h"

#if defined(KEYBOARD_keychron_k6_rgb_ansi)
    #include "ansi.h"
#elif defined(KEYBOARD_keychron_k6_rgb_iso)
    #include "iso.h"
#elif defined(KEYBOARD_keychron_k6_rgb_optical_ansi)
    #include "optical_ansi.h"
#elif defined(KEYBOARD_keychron_k6_rgb_optical_iso)
    #include "optical_iso.h"
#endif
