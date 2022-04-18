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
    ResetKeyboard = 254,
    GoIntoBootloader = 255
};
