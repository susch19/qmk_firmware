# MCU name
MCU = SN32F248BF
BOOTLOADER = sn32-dfu

LTO_ENABLE = no
KEYBOARD_SHARED_EP = no
BACKLIGHT_ENABLE = no
MAGIC_ENABLE = yes
MAGIC_KEYCODE_ENABLE = yes
BOOTMAGIC_ENABLE = yes # Virtual DIP switch configuration
MOUSEKEY_ENABLE = no    # Mouse keys
EXTRAKEY_ENABLE = no    # Audio control and System control
RAW_ENABLE = yes
CONSOLE_ENABLE = no     # Console for debug
COMMAND_ENABLE = no     # Commands for debug and configuration
SLEEP_LED_ENABLE = no   # Breathing sleep LED during USB suspend
NKRO_ENABLE = yes        # USB Nkey Rollover
AUDIO_ENABLE = no
RGBLIGHT_ENABLE = no
UNICODE_ENABLE = no
SERIAL_LINK_ENABLE = no
WAIT_FOR_USB = no
CUSTOM_MATRIX = yes
DIP_SWITCH_ENABLE = yes # Bluetooth and Windows/Mac Switch
PROGRAMMABLE_BUTTON_ENABLE = yes
TAP_DANCE_ENABLE = no
RGB_MATRIX_CUSTOM_USER = yes
KEY_OVERRIDE_ENABLE = yes
SPACE_CADET_ENABLE = no

BLUETOOTH_ENABLE 		= yes
BLUETOOTH_DRIVER 		= ItonBT
DIP_SWITCH_ENABLE 		= yes

# Custom RGB matrix handling
 RGB_MATRIX_ENABLE = yes
 RGB_MATRIX_DRIVER = SN32F24xB

EEPROM_DRIVER = wear_leveling
WEAR_LEVELING_DRIVER = sn32_flash

# # project specific files
# Build ansi by default
# DEFAULT_FOLDER = keychron/k14/rgb/ansi
