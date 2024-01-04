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
    PCMacro16Bit,
    PCMacroString
};

enum other_modes{
    CustomKeyCode,
    CustomCommands = 255
};

enum custom_commands{
    MULTI = 253,
    RESET_KEYBOARD = 254,
    GO_INTO_BOOTLOADER = 255
};
