/** @file */

#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include <math.h>
#include <assert.h>
#include <errno.h>
#include "stack.h"

#define DEFINES_ONLY
#include "reg_address.h"
#undef DEFINES_ONLY

/// More comfortable dump
#define CPU_dump(This) cpu_dump_(This, #This)
/// To be stylish
#define cpu_dump(This) CPU_dump(This)
/// To be stylish
#define cpu_OK(This) CPU_OK(This)

/**
@brief Simple stack processor.

Processor is a structure that containes a stack and a register to operate with.
*/
typedef struct
{
    char registers[REG_SIZE * REG_NUMBER];/**< Registers of the processor*/
    Stack stack;/**< Operating stack. */
    Stack fu_stack; /**< Stack that contains functions return positions */
    int position;
    Buffer program;

    bool state;/**< State of the CPU. true if ON, false if OFF. */
} CPU;

/**
*@brief Standard CPU constructor.
*
*Constructs CPU with empty stack and register.
*@param This Pointer to the CPU to be constructed.
*@return 1 (true) if success, 0 (false) otherwise.
*@todo Dynamic memory management, so its OK that only true is returned by now.
*/
bool cpu_construct (CPU* This);

/**
*@brief Copy CPU constructor.
*
*Constructs CPU as copy of other. Writes to errno.
*@param This Pointer to the CPU to be constructed.
*@param other The CPU to copy from.
*@return 1 (true) if success, 0 (false) otherwise.
*/
bool cpu_construct_copy (CPU* This, const CPU* other);

/**
*@brief Destructs the CPU.
*
*Destructs the CPU, setting its values to poison.
*@param This Pointer to the CPU to be destructed.
*/
void cpu_destruct (CPU* This);

/**
*@brief Validates the CPU.
*
*Checks if the CPU is correct according to its values.
*@param This Pointer to the CPU to be checked.
*@return true if the CPU is valid, false otherwise.
*/
bool cpu_OK (const CPU* This);

/**
*@brief Prints CPU's dump.
*
*Outputs the current state of CPU.
*@param This Pointer to the CPU to be dumped.
*/
void cpu_dump_ (const CPU* This, const char name[]);

/**
*@brief Pushes value to CPU stack.
*
*Puts the given value at the top of CPU stack.
*@param This Pointer to the CPU to perform operation on.
*@param value The value to be put is the stack.
*@return true if success, false otherwise.
*/
bool cpu_push (CPU* This);

/**
*@brief Pushes variable to CPU stack.
*
*Puts the value from the given register at the top of CPU stack.
*@param This Pointer to the CPU to perform operation on.
*@param registerN Register to read the value from.
*@return true if success, false otherwise.
*/
bool cpu_push_reg (CPU* This);

/**
*@brief Pops value from the stack.
*
*Pops the top element from the CPU and writes it to the given register.
*@param This Pointer to the CPU to perform operation on.
*@param registerN Register to write the value to.
*@return true if success, false otherwise. In case it wasn't successful, invalidates CPU.
*@warning Stack must contain at least one element.
*/
bool cpu_pop_reg (CPU* This);

/**
*@brief Summs the top two elements of the stack.
*
*Add the top element of stack to the previous and stroes the result in stack.
*Both top two elements are removed.
*@param This Pointer to the CPU to perform operation on.
*@return true if success, false otherwise. In case of fail invalidates CPU.
*@warning Stack must contain at least two elements.
*/
bool cpu_add (CPU* This);

/**
*@brief Subtracts the penult stack element from the top one.
*
*Gets the top element of stack and subtracts the penult stack element from it.
*Both top two elements are removed and the result is written to stack.
*@param This Pointer to the CPU to perform operation on.
*@return true if success, false otherwise. In case of fail invalidates CPU.
*@warning Stack must contain at least two elements.
*/
bool cpu_sub (CPU* This);

/**
*@brief Multiplies the top two elements of the stack.
*
*Multiplies the top element of stack to the previous and stroes the result in stack.
*Both top two elements are removed.
*@param This Pointer to the CPU to perform operation on.
*@return true if success, false otherwise. In case of fail invalidates CPU.
*@warning Stack must contain at least two elements.
*/
bool cpu_mul (CPU* This);

/**
*@brief Divides the top stack element by the previous.
*
*Gets the top element of stack and divides it by the penult stack element.
*Both top two elements are removed and the result is written to stack.
*@param This Pointer to the CPU to perform operation on.
*@return true if success, false otherwise. In case of fail invalidates CPU.
*@warning Stack must contain at least two elements.
*/
bool cpu_div (CPU* This);

/**
*@brief Raise the top element in the power of the penult one.
*
*Gets the top element of stack and raises it to power equal to the penult stack element.
*Both top two elements are removed and the result is written to stack.
*@param This Pointer to the CPU to perform operation on.
*@return true if success, false otherwise. In case of fail invalidates CPU.
*@warning Stack must contain at least two elements.
*/
bool cpu_pow (CPU* This);

/**
*@brief Executes the given program.
*
*Treats the given sequence of numbers as instructions and executes them.
*@param This Pointer to the CPU to perform operation on.
*@param program Array of instruction codes.
*@return true if no error has occured, false otherwise. In case of fail invalidates CPU.
*@warning End of the program must be marked with 'end' code. Undefined behaviour otherwise.
*/
bool cpu_execute (CPU* This);

/**
*@brief Prints the top stack element.
*
*@param This Pointer to the CPU to perform operation on.
*@return true if no error has occured, false otherwise. In case of fail invalidates CPU.
*/
bool cpu_out (CPU* This);

/**
*@brief Scans value and puts it to the top of the stack
*
*@param This Pointer to the CPU to perform operation on.
*@return true if no error has occured, false otherwise. In case of fail invalidates CPU.
*/
bool cpu_in (CPU* This);

bool cpu_jmp (CPU* This)
{
    ASSERT_OK(CPU, This);
    This->position++;
    int new_position = *(int*)(This->program.chars + This->position);
    This->position = new_position;
    //printf ("jmp %d\n", new_position);

    return true;
}

#define IF_JUMP(name, operation) \
bool cpu_ ## name (CPU* This)\
{\
    ASSERT_OK(CPU, This);\
    TYPE top = 0, next = 0;\
    This->position++;\
    int new_position = *(int*)(This->program.chars + This->position);\
    \
    if (!stack_pop(&This->stack, &top) || !stack_pop(&This->stack, &next))\
    {\
        cpu_destruct(This);\
        ASSERT_OK(CPU, This);\
        return false;\
    }\
    else\
    {\
        /*printf ("CHECK: %f " #operation " %f =>", top, next);*/\
        if (top operation next)\
        {\
            This->position = new_position;\
            /*printf ("jmp %d\n", new_position);*/\
        }\
        else\
        {\
            This->position += sizeof(int);\
            /*printf ("jmp ++\n");*/\
        }\
        ASSERT_OK(CPU, This);\
        return true;\
    }\
}

IF_JUMP (ja,  > )
IF_JUMP (jae, >=)
IF_JUMP (jb,  < )
IF_JUMP (jbe, <=)
IF_JUMP (je,  ==)
IF_JUMP (jne, !=)

// Dummy functions
bool cpu_call(CPU* This)
{
    ASSERT_OK(CPU, This);
    This->position++;
    int new_position = *(int*)(This->program.chars + This->position);
    This->position += sizeof (int);
    if (!stack_push (&This->fu_stack, (float)This->position))
        return false;
    This->position = new_position;
    //printf ("call %d\n", new_position);

    return true;
}
bool cpu_err(CPU* This)
{
    printf ("*BEEP*\n");
    return false;
}
bool cpu_ret(CPU* This)
{
    ASSERT_OK(CPU, This);
    float ret_position = 0;
    if (!stack_pop (&This->fu_stack, &ret_position))
        return false;
    This->position = ret_position;
    //printf ("ret %d\n", (int)ret_position);

    return true;
}
bool cpu_end(CPU* This)
{
    return true;
}
bool cpu_pop(CPU* This)
{
    return false;
}
bool cpu_pop_mem(CPU* This)
{
    return false;
}
bool cpu_push_mem(CPU* This)
{
    return false;
}

bool cpu_set_program (CPU* This, const Buffer* program)
{
    buffer_destruct(&This->program);
    if (!buffer_construct_copy(&This->program, program))
        return false;
    return true;
}

bool cpu_construct (CPU* This)
{
    assert (This);
    if (!buffer_construct_empty(&This->program, 1))
        return false;
    // The program begins with 'end'
    This->program.chars[0] = 0;
    This->position = 0;
    memset (This->registers, 0, REG_SIZE * REG_NUMBER);
    if (!stack_construct(&This->stack))
    {
        cpu_destruct(This);
        return false;
    }

    This->state = true;
    return true;
}

bool cpu_construct_copy(CPU* This, const CPU* other)
{
    assert (This);
    ASSERT_OK(CPU, other);
    buffer_construct_copy(&This->program, &other->program);
    This->position = other->position;
    memcpy (This->registers, other->registers, REG_SIZE * REG_NUMBER);
    if (!stack_construct_copy(&This->stack, &other->stack))
    {
        cpu_destruct(This);
        return false;
    }

    This->state = true;
    return true;
}

void cpu_destruct (CPU* This)
{
    assert (This);
    This->position = 0;
    buffer_destruct(&This->program);
    memset (This->registers, 0, REG_SIZE * REG_NUMBER);
    stack_destruct(&This->stack);
    This->state = false;
}

bool cpu_OK (const CPU* This)
{
    assert (This);
    return This->state && stack_OK(&This->stack) && buffer_OK (&This->program) && This->registers;
}

void cpu_dump_ (const CPU* This, const char name[])
{
    assert (This);
    printf ("%s = CPU (", name);
    if (cpu_OK(This))
        printf ("ok)\n");
    else
        printf ("ERROR)\n");
    printf ("{\n");
    printf ("    state = %d\n", This->state);
    printf ("    registers:\n");
    for (size_t i = 0; i < REG_NUMBER; i++)
        printf ("      [%lu] %d\n", i, This->registers[i*REG_SIZE]);
    printf ("    program:\n");
    for (int i = 0; i < This->program.length; i ++)
        printf ("%d ", (int)This->program.chars[i]);
    printf ("\n    stack:\n");
    stack_dump(&This->stack);
    printf ("}\n");
}

bool cpu_push (CPU* This)
{
    ASSERT_OK(CPU, This);
    This->position ++;
    TYPE value = *(TYPE*)(This->program.chars + This->position);
    This->position += sizeof(TYPE);

    if (!stack_push(&This->stack, value))
    {
        cpu_destruct(This);
        return false;
    }
    ASSERT_OK(CPU, This);
    return true;
}

bool cpu_dup (CPU* This)
{
    ASSERT_OK(CPU, This);
    TYPE top = 0;
    if (!stack_pop(&This->stack, &top))
    {
        cpu_destruct(This);
        return false;
    }
    if (!stack_push(&This->stack, top) || !stack_push(&This->stack, top))
    {
        cpu_destruct(This);
        return false;
    }
    return true;
}

bool cpu_push_reg (CPU* This)
{
    ASSERT_OK(CPU, This);
    This->position ++;
    char address = This->program.chars[This->position];
    This->position ++;

    TYPE value = *(TYPE*)(This->registers + address);
    if (!stack_push(&This->stack, value))
    {
        cpu_destruct(This);
        return false;
    }
    ASSERT_OK(CPU, This);
    return true;
}

bool cpu_pop_reg (CPU* This)
{
    ASSERT_OK(CPU, This);
    This->position ++;
    char address = This->program.chars[This->position];
    This->position ++;

    if (!stack_pop(&This->stack, (TYPE*)(This->registers + address)))
    {
        cpu_destruct(This);
        ASSERT_OK(CPU, This);
        return false;
    }
    ASSERT_OK(CPU, This);
    return true;
}

bool cpu_add (CPU* This)
{
    ASSERT_OK(CPU, This);
    TYPE a = 0, b = 0, sum = 0;

    if (!stack_pop(&This->stack, &a) || !stack_pop(&This->stack, &b))
    {
        cpu_destruct(This);
        ASSERT_OK(CPU, This);
        return false;
    }
    else
    {
        sum = a + b;
        ASSERT_OK(CPU, This);
        return stack_push(&This->stack, sum);
    }
}

bool cpu_sub (CPU* This)
{
    ASSERT_OK(CPU, This);
    TYPE a = 0, b = 0, sub = 0;

    if (!stack_pop(&This->stack, &a) || !stack_pop(&This->stack, &b))
    {
        cpu_destruct(This);
        ASSERT_OK(CPU, This);
        return false;
    }
    else
    {
        sub = a - b;
        ASSERT_OK(CPU, This);
        return stack_push(&This->stack, sub);
    }
}

bool cpu_mul (CPU* This)
{
    ASSERT_OK(CPU, This);
    TYPE a = 0, b = 0, mul = 0;

    if (!stack_pop(&This->stack, &a) || !stack_pop(&This->stack, &b))
    {
        cpu_destruct(This);
        ASSERT_OK(CPU, This);
        return false;
    }
    else
    {
        mul = a * b;
        ASSERT_OK(CPU, This);
        return stack_push(&This->stack, mul);
    }
}

bool cpu_div (CPU* This)
{
    ASSERT_OK(CPU, This);
    TYPE a = 0, b = 0, div = 0;

    if (!stack_pop(&This->stack, &a) || !stack_pop(&This->stack, &b))
    {
        cpu_destruct(This);
        ASSERT_OK(CPU, This);
        return false;
    }
    else
    {
        div = a / b;
        ASSERT_OK(CPU, This);
        return stack_push(&This->stack, div);
    }
}

bool cpu_pow (CPU* This)
{
    ASSERT_OK(CPU, This);
    TYPE a = 0, b = 0, a_pow_b = 0;

    if (!stack_pop(&This->stack, &a) || !stack_pop(&This->stack, &b))
    {
        cpu_destruct(This);
        ASSERT_OK(CPU, This);
        return false;
    }
    else
    {
        a_pow_b = (TYPE) pow ((double) a, (double) b);
        ASSERT_OK(CPU, This);
        return stack_push(&This->stack, a_pow_b);
    }
}

bool cpu_out (CPU* This)
{
    ASSERT_OK(CPU, This);
    TYPE top = 0;
    if (!stack_pop(&This->stack, &top))
    {
        cpu_destruct(This);
        ASSERT_OK(CPU, This);
        return false;
    }
    printf ("%g\n", top);
    ASSERT_OK(CPU, This);
    return true;
}

bool cpu_in (CPU* This)
{
    ASSERT_OK(CPU, This);
    TYPE input = 0;

    scanf ("%f", &input);
    if (!stack_push(&This->stack, input))
    {
        cpu_destruct(This);
        ASSERT_OK(CPU, This);
        return false;
    }
    ASSERT_OK(CPU, This);
    return true;
}
// TODO: all functions are written in processor, we are only calling them
//from define of each CMD_INFO with right argument
bool cpu_execute (CPU* This)
{
    while (This->program.chars[This->position] != 0)
    {
        bool is_done = false;
#define CMD(name, key, shift, arguments) \
        if (!is_done && This->program.chars[This->position] == key)\
        {\
            /*printf ("EXE: " #name "\n");*/\
            if (!cpu_ ## name (This))\
                return false;\
            /*stack_dump (&This->stack);*/\
            This->position += shift;\
            is_done = true;\
        }

#include "commands.h"
#undef CMD
    }

    return true;
}


#endif // CPU_H_INCLUDED
