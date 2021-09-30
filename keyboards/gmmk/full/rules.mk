# project specific files

## chip/board settings
# - the next two should match the directories in
#   <chibios>/os/hal/ports/$(MCU_FAMILY)/$(MCU_SERIES)
MCU_FAMILY = SN32
MCU_SERIES = SN32F260

# Linker script to use
# - it should exist either in <chibios>/os/common/ports/ARMCMx/compilers/GCC/ld/
#   or <this_dir>/ld/
MCU_LDSCRIPT = SN32F260

# Startup code to use
#  - it should exist in <chibios>/os/common/startup/ARMCMx/compilers/GCC/mk/
MCU_STARTUP = sn32f26x

# Board: it should exist either in <chibios>/os/hal/boards/
#  or <this_dir>/boards
BOARD = SN_SN32F260

# Cortex version
MCU  = cortex-m0

# ARM version, CORTEX-M0/M1 are 6, CORTEX-M3/M4/M7 are 7
ARMV = 6

# BOOTLOADER = flash
SN32_BOOTLOADER_ADDRESS = 0x1FFF0009

OPT_DEFS = -Os

# Options to pass to dfu-util when flashing
# DFU_ARGS = -d 0483:df11 -a 0 -s 0x08000000:leave
# DFU_SUFFIX_ARGS = -p DF11 -v 0483

# Build Options
#   comment out to disable the options.
#
BACKLIGHT_ENABLE = no
MAGIC_ENABLE = no
BOOTMAGIC_ENABLE = yes # Virtual DIP switch configuration
MOUSEKEY_ENABLE = no    # Mouse keys
EXTRAKEY_ENABLE = no    # Audio control and System control
SLEEP_LED_ENABLE = no   # Breathing sleep LED during USB suspend
NKRO_ENABLE = no
CONSOLE_ENABLE = no
SERIAL_LINK_ENABLE = no
WAIT_FOR_USB = no
CUSTOM_MATRIX = no
LED_MATRIX_ENABLE = no
LED_MATRIX_DRIVER = custom
RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = custom
RAW_ENABLE = yes

OPENRGB_ENABLE = yes

# some options to reduce ram usage
LDFLAGS += --specs=nano.specs
OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE
USE_LINK_GC = yes
LTO_ENABLE = yes

USE_EXCEPTIONS_STACKSIZE = 0xD0
USE_PROCESS_STACKSIZE = 0x1E0

DEFAULT_FOLDER = gmmk/full/rev3

# process stack size of 0x1c0 crashes during SEND_STRING
USE_EXCEPTIONS_STACKSIZE = 0xD0
# 0x180 140 f0 d0    80gg a0gg b0gg b8gg c0 ok
USE_PROCESS_STACKSIZE = 0x1E0
# 0x210
