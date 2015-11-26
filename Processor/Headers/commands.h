#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED
/** @file */
/// Maximum number of instructions in the program.
#define MAX_PROGRAM_LENGTH 1024

//Position argument (unsigned int)
#define ARG_POS 1
//Number argument (int or float)
#define ARG_NUM 2
//address of memory
#define ARG_MEM 2*2
//register
#define ARG_REG 2*2*2
//label
#define ARG_LBL 2*2*2*2
//overloaded
#define ARG_OVL 2*2*2*2*2
//no arguments
#define ARG_NO  2*2*2*2*2*2

#endif // COMMANDS_H_INCLUDED
//! FLAG TO INT AND FLOAT OPERATIONS
/// Codes of CPU instructions
#ifndef DEFINES_ONLY
// CMD(name, key_value, shift_to_the_right, arguments_type)

//^^^^^^^^^^^^^^^^^^^^^^^^
// 0~19  NO ARGUM/home/alartum/Study/Proga/Processor/Assembler/main.c|289|ошибка: ENT commands
//^^^^^^^^^^^^^^^^^^^^^^^^

CMD (end, 0, 0, ARG_NO)
CMD (err, 1, 0, ARG_NO)
CMD (out, 2, 1, ARG_NO)
CMD (add, 3, 1, ARG_NO)
CMD (sub, 4, 1, ARG_NO)
CMD (mul, 5, 1, ARG_NO)
CMD (div, 6, 1, ARG_NO)
CMD (pow, 7, 1, ARG_NO)
CMD (ret, 8, 0, ARG_NO)
CMD (dup, 9, 1, ARG_NO)

//^^^^^^^^^^^^^^^^^^^^^^^^
// 20~39 OVERLOADED commands
//^^^^^^^^^^^^^^^^^^^^^^^^
CMD (push,     20, 0, (ARG_NUM | ARG_REG | ARG_MEM))
CMD (push_reg, 21, 0, ARG_OVL)
CMD (push_mem, 22, 0, ARG_OVL)

// Dummy command
CMD  (pop,      23, 0, (ARG_MEM | ARG_REG))
CMD  (pop_reg,  24, 0, ARG_OVL)
CMD  (pop_mem,  25, 0, ARG_OVL)

//^^^^^^^^^^^^^^
// 40~60 LABEL commands
//^^^^^^^^^^^^^^
CMD   (ja,   41, 0, (ARG_POS | ARG_LBL))
CMD   (jae,  42, 0, (ARG_POS | ARG_LBL))
CMD   (jb,   43, 0, (ARG_POS | ARG_LBL))
CMD   (jbe,  44, 0, (ARG_POS | ARG_LBL))
CMD   (je,   45, 0, (ARG_POS | ARG_LBL))
CMD   (jne,  46, 0, (ARG_POS | ARG_LBL))
CMD   (jmp,  47, 0, (ARG_POS | ARG_LBL))
CMD   (call, 48, 0, ARG_LBL)
#endif
