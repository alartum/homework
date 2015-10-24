/** @file */

#ifndef CODES_H_INCLUDED
#define CODES_H_INCLUDED
/// Maximum number of instructions in the program.
#define MAX_PROGRAM_LENGTH 1024
/// Codes of CPU instructions.
enum CODE
{
    PUSH = 1, /**< Pushes argument to the stack. Usage: PUSH <number>*/
    PUSH_RX, /**< Pushes argument to the stack. Usage: PUSH <register>*/
    POP, /**< Pops argument from the stack to register. Usage: POP <register>*/
    END, /**< Ends the program. Usage: END*/
    ADD, /**< Summs top two elements of stack. Usage: ADD*/
    SUB, /**< Subtracts the previous element from the top. Usage: DIV*/
    MUL, /**< Multiplies top two elements of stack. Usage: MUL*/
    DIV, /**< Divides the top element by the previous. Usage: DIV*/
    POW, /**< Raise the top element in the power of previous. Usage: POW*/
    JA, /**< Jumps to the given line if the top element is above the previous. Usage: JA <number>*/
    JMP, /**< Jumps to the given line. Usage: JMP <number>*/
    OUT, /**< Outputs the top stack element. Usage: OUT */
    ERROR /**< Error code */
};

/// Number of processor registers.
#define REGISTERS_NUMBER 4
/// Names of registers.
enum REGISTERS
{
    RAX,
    RBX,
    RCX,
    RDX
};

#endif // CODES_H_INCLUDED
