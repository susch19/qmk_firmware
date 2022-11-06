# project specific files
# SRC = ../../../drivers/led/sn32/rgb_matrix_sn32f24xb.c
SRC += config_led.c

# Custom RGB matrix handling
RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = SN32F24xB

VIA_ENABLE = yes
OPENRGB_ENABLE = yes
CUSTOM_MATRIX = yes
