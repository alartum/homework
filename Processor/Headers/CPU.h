/** @file */

#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "CPU.h"
#include <math.h>
#include <assert.h>
#include <errno.h>
#include "stack.h"
#include "codes.h"

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
    TYPE registers[REGISTERS_NUMBER];/**< Registers of the processor (64-bit).*/
    Stack stack;/**< Operating stack. */

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
bool cpu_push (CPU* This, TYPE value);

/**
*@brief Pushes variable to CPU stack.
*
*Puts the value from the given register at the top of CPU stack.
*@param This Pointer to the CPU to perform operation on.
*@param registerN Register to read the value from.
*@return true if success, false otherwise.
*/
bool cpu_push_rx (CPU* This, enum REGISTERS registerN);

/**
*@brief Pops value from the stack.
*
*Pops the top element from the CPU and writes it to the given register.
*@param This Pointer to the CPU to perform operation on.
*@param registerN Register to write the value to.
*@return true if success, false otherwise. In case it wasn't successful, invalidates CPU.
*@warning Stack must contain at least one element.
*/
bool cpu_pop_rx (CPU* This, enum REGISTERS registerN);

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
bool cpu_execute (CPU* This, const int program[]);

/**
*@brief Prints the top stack element.
*
*@param This Pointer to the CPU to perform operation on.
*@return true if no error has occured, false otherwise. In case of fail invalidates CPU.
*/
bool cpu_out (CPU* This);

/**
*@brief Builds commands map by the given program.
*
*@param program Array of instruction codes.
*@param map The map to be build.
*/
void build_map (const int program[], int map[]);

bool cpu_construct (CPU* This)
{
    assert (This);
    memset (This->registers, 0, sizeof (TYPE) * REGISTERS_NUMBER);
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
    memcpy (This->registers, other->registers, sizeof (TYPE) * REGISTERS_NUMBER);
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
    memset (This->registers, 0, sizeof (TYPE) * REGISTERS_NUMBER);
    stack_destruct(&This->stack);
    This->state = false;
}

bool cpu_OK (const CPU* This)
{
    assert (This);
    return This->state && stack_OK(&This->stack);
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
    for (size_t i = 0; i < REGISTERS_NUMBER; i++)
        printf ("      [%lu] %d\n", i, This->registers[i]);
    stack_dump(&This->stack);
    printf ("}\n");
}

bool cpu_push (CPU* This, TYPE value)
{
    ASSERT_OK(CPU, This);

    if (!stack_push(&This->stack, value))
    {
        cpu_destruct(This);
        return false;
    }
    ASSERT_OK(CPU, This);
    return true;
}

bool cpu_push_rx (CPU* This, enum REGISTERS registerN)
{
    ASSERT_OK(CPU, This);
    return cpu_push(This, This->registers[registerN]);
}

bool cpu_pop_rx (CPU* This, enum REGISTERS registerN)
{
    ASSERT_OK(CPU, This);

    if (!stack_pop(&This->stack, &This->registers[registerN]))
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
    printf ("%d\n", top);
    ASSERT_OK(CPU, This);
    return true;
}

bool cpu_execute (CPU* This, const int program[])
{
    size_t current = 0;
    int positions[MAX_PROGRAM_LENGTH] = {};
    build_map((const int*)program, positions);

    while (program[current] != END)
    {
        switch (program[current])
        {
        case PUSH:
            DPRINT_int(PUSH);
            if (!cpu_push(This, program[++current]))
                return false;
            current++;
            break;
        case PUSH_RX:
            DPRINT_int(PUSH_RX);
            if (!cpu_push_rx(This, (enum CODE)program[++current]))
                return false;
            current++;
            break;
        case POP:
            DPRINT_int(POP);
            if (!cpu_pop_rx(This, (enum CODE)program[++current]))
                return false;
            current++;
            break;
        case ADD:
            DPRINT_int(ADD);
            if (!cpu_add(This))
                return false;
            current++;
            break;
        case SUB:
            DPRINT_int(SUB);
            if (!cpu_div(This))
                return false;
            current++;
            break;
        case MUL:
            DPRINT_int(MUL);
            if (!cpu_mul(This))
                return false;
            current++;
            break;
        case DIV:
            DPRINT_int(DIV);
            if (!cpu_div(This))
                return false;
            current++;
            break;
        case POW:
            DPRINT_int(POW);
            if (!cpu_pow(This))
                return false;
            current++;
            break;
        case JMP:
            DPRINT_int(JMP);
            current ++;
            current = positions[program[current]];
            break;
        case JA:
            ;
            DPRINT_int(JA);
            TYPE top = 0, previous = 0;
            if (!stack_pop(&This->stack, &top) || !stack_pop(&This->stack, &previous))
                return false;
            current++;
            if (top > previous)
                current = positions[program[current]];
            else
                current++;
            break;
        case OUT:
            DPRINT_int(OUT);
            if (!cpu_out(This))
                return false;
            current++;
            break;
        default:
            return false;
        }
    }
    return true;
}

void build_map (const int program[], int map[])
{
    size_t current = 0, current_map = 0;
    while (program[current] != END)
    {
        map[current_map] = current;
        current_map++;
        switch (program[current])
        {
        case PUSH:
            current += 2;
            break;
        case PUSH_RX:
            current += 2;
            break;
        case POP:
            current += 2;
            break;
        case ADD:
            current++;
            break;
        case SUB:
            current++;
            break;
        case MUL:
            current++;
            break;
        case DIV:
            current++;
            break;
        case POW:
            current++;
            break;
        case JMP:
            current += 2;
            break;
        case JA:
            ;
            current += 2;
            break;
        case OUT:
            current++;
            break;
        }
    }
    map[current_map] = current;
}
#endif // CPU_H_INCLUDED
