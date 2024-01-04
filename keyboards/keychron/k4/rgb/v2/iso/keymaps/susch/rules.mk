
ifeq ($(strip $(TAP_DANCE_ENABLE)), yes)
    SRC += tap_dance_custom.c
endif


ifeq ($(strip $(RAW_ENABLE_SUSCH)), yes)
    SRC += raw_hid_custom.c
endif

SRC += custom_space_cadet.c
# SRC += calc.c

RAW_ENABLE = yes
VIA_ENABLE = yes
AUTO_SHIFT_ENABLE = yes
RGB_MATRIX_CUSTOM_USER = yes

BLUETOOTH_ENABLE 		= yes
BLUETOOTH_DRIVER 		= ItonBT
