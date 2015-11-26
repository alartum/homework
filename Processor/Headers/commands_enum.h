#ifndef COMMANDS_ENUM_H_INCLUDED
#define COMMANDS_ENUM_H_INCLUDED

enum COMMANDS
{
    #define CMD(name, key_value, shift_to_the_right, arguments_type) \
    cmd_ ## name = key_value,
    #include "commands.h"
    #undef CMD
};



#endif // COMMANDS_ENUM_H_INCLUDED
