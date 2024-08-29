#pragma once

enum main_modes {
    RGB_COMMAND,
    LAYER_COMMAND,
    MACRO_COMMAND,
    OTHER_COMMAND
};

enum rgb_modes{

    IndexItereationRGBZero,
    Reserved,
    PerKeyRGB,
    IndexItereationRGB,
};
enum layer_modes{
    SwitchToLayer,
    ResetLayer,
    ChangeLanguage
};
enum macro_modes{
    PCMacro,
    PCMacroUnicode,
    PCMacro16Bit,
    PCMacroString,
    PCMacroUnicodeString,
    TypeClipboard
};

enum other_modes{
    Ack,
    CustomKeyCode,
    CustomCommands = 255
};

enum custom_commands{
    MULTI = 253,
    RESET_KEYBOARD = 254,
    GO_INTO_BOOTLOADER = 255
};
