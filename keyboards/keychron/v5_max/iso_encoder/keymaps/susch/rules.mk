
ifeq ($(strip $(TAP_DANCE_ENABLE)), yes)
    SRC += tap_dance_custom.c
endif


RAW_ENABLE_SUSCH = yes

ifeq ($(strip $(RAW_ENABLE_SUSCH)), yes)
    SRC += raw_hid_custom.c
endif


SRC += custom_space_cadet.c

RAW_ENABLE = yes
AUTO_SHIFT_ENABLE = yes
VIA_ENABLE = yes
RGB_MATRIX_CUSTOM_USER = yes
UNICODE_COMMON = yes