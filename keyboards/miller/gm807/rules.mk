# MCU name
MCU        = SN32F248BF
BOOTLOADER = sn32-dfu

# Build Options
#   comment out to disable the options.
#
LTO_ENABLE		  = no
BOOTMAGIC_ENABLE  = yes      # Enable Bootmagic Lite
MOUSEKEY_ENABLE   = no       # Mouse keys
EXTRAKEY_ENABLE   = yes      # Audio control and System control
CONSOLE_ENABLE    = no       # Console for debug
COMMAND_ENABLE    = no       # Commands for debug and configuration
NKRO_ENABLE       = no       # Enable N-Key Rollover
BACKLIGHT_ENABLE  = no       # Enable keyboard backlight functionality
AUDIO_ENABLE      = no       # Audio output

WAIT_FOR_USB      = no
ENCODER_ENABLE    = yes

BLUETOOTH_ENABLE  = yes
BLUETOOTH_DRIVER  = ItonBT

# Custom RGB matrix handling
RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = SN32F24xB
