
ifeq ($(strip $(TAP_DANCE_ENABLE)), yes)
    SRC += tap_dance_custom.c
endif


ifeq ($(strip $(RAW_ENABLE_SUSCH)), yes)
    SRC += raw_hid_custom.c
endif

SRC += custom_space_cadet.c
# SRC += calc.c

VIA_ENABLE = yes


