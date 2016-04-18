
#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <assert.h>
#include <errno.h>
#include <string.h>

/// Type of stack's values.
#define TYPE float
/// Maximum number of elements in stack.
#define STACK_SIZE 32
#define MAX_PRINTED 8

/// More comfortable dump
#define Stack_dump(This) stack_dump_(This, #This)
/// To be stylish
#define stack_dump(This) Stack_dump(This)
/// To be stylish
#define Stack_OK(This) stack_OK(This)

/**
@brief Simple stack of integer
Stack contains integer numbers and provides some operations with them
*/
typedef struct
{
    TYPE values [STACK_SIZE];/**< Array of values. */
    TYPE* top;/**< Pointer to the top element of stack. */
    size_t size;/**< Number of elements in the stack. */

    bool state;/**< State of the stack. true if valid, false otherwise. */
} Stack;

/**
*@brief Standard stack constructor.
*
*Constructs stack with the top pointer at NULL. Writes to errno.
*@param This Pointer to the stack to be constructed.
*@return 1 (true) if success, 0 (false) otherwise.
*@todo Dynamic memory management, so its OK that only true is returned by now
*/
bool stack_construct (Stack* This);

/**
*@brief Copy stack constructor.
*
*Constructs stack as copy of other. Writes to errno.
*@param This Pointer to the stack to be constructed.
*@param other The stack to copy from.
*@return 1 (true) if success, 0 (false) otherwise.
*/
bool stack_construct_copy (Stack* This, const Stack* other);

/**
*@brief Destructs the stack.
*
*Destructs the stack, setting its values to poison.
*@param This Pointer to the stack to be destructed.
*/
void stack_destruct (Stack* This);

/**
*@brief Validates the stack.
*
*Checks if the stack is correct according to its values.
*@param This Pointer to the stack to be checked.
*@return true if the stack is valid, false otherwise.
*/
bool Stack_OK (const Stack* This);

/**
*@brief Prints stack's dump.
*
*Outputs the current state of stack.
*@param This Pointer to the stack to be dumped.
*/
void stack_dump_ (const Stack* This, const char name[]);

/**
*@brief Pushes value to stack.
*
*Put the given value to stack as top element.
*@param This Pointer to the stack to perform operation on.
*@param value Value to be pushed.
*@return true if success, false otherwise
*/
bool stack_push (Stack* This, TYPE value);

/**
*@brief Pops value from the stack.
*
*Pops the top element from the stack and returnes it.
*@param This Pointer to the stack to perform operation on.
*@param value Value that was popped. In case it wasn't successful, invalidates stack.
*/
bool stack_pop (Stack* This, TYPE* value);

bool stack_construct (Stack *This)
{
    assert (This);
    This->top = NULL;
    This->size = 0;
    This->state = true;

    return true;
}

bool stack_construct_copy (Stack* This, const Stack* other)
{
    assert (This);
    ASSERT_OK(Stack, other);
    if (other->size == 0)
    {
        This->top = NULL;
        This->state = true;
        This->size = 0;

        return true;
    }
    else
    {
        memcpy (This->values, other->values, sizeof(TYPE)*other->size);
        if (!This->values)
        {
            errno = EFAULT;
            stack_destruct(This);
            return false;
        }
        This->size = other->size;
        This->top = This->values + (This->size - 1);
        This->state = true;

        return true;
    }
    ASSERT_OK(Stack, This);
    ASSERT_OK(Stack, other);
}

void stack_destruct (Stack* This)
{
    assert (This);
    This->top = NULL;
    This->size = 0;
    // Destruction must work always
    This->state = false;
}

bool Stack_OK (const Stack* This)
{
    assert (This);
    return This->state && (!This->top || (This->top == (This->values + (This->size - 1))));
}

void stack_dump_ (const Stack* This, const char name[])
{
    assert (This);
    printf ("%s = Stack (", name);
    if (Stack_OK(This))
        printf ("ok)\n");
    else
        printf ("ERROR)\n");
    printf ("{\n");
    printf ("    state = %d\n", This->state);
    printf ("    top = %p\n", This->top);
    printf ("    size = %lu\n", This->size);
    printf ("    values:\n");

    if (This->top != NULL)
    {
        int i = 0;
        for (; (This->values + i)!= This->top && i < MAX_PRINTED; i ++)
            printf ("        ->[%d] %g\n", i, This->values[i]);
        // Printing top
        printf ("        -->[%d] %g\n", i, This->values[i]);
        i++;
        for (; i < MAX_PRINTED; i ++)
            printf ("          [%d] %g\n", i, This->values[i]);
    }
    else
        for (int i = 0; i < MAX_PRINTED; i ++)
            printf ("          [%d] %g\n", i, This->values[i]);
    printf ("}\n");
}

bool stack_push (Stack* This, TYPE value)
{
    ASSERT_OK(Stack, This);
    if (This->top == NULL)
    {
        This->top = This->values;
        *This->top = value;
        This->size = 1;

        ASSERT_OK(Stack, This);
        return true;
    }
    else if (This->size == STACK_SIZE)
    {
        stack_destruct(This);
        return false;
    }
    else
    {
        *(++ This->top) = value;
        This->size ++;
    }
    ASSERT_OK(Stack, This);

    return true;
}

bool stack_pop (Stack* This, TYPE* value)
{
    assert (value);
    ASSERT_OK(Stack, This);
    if (This->top == NULL)
    {
        stack_destruct(This);
        return false;
    }
    else if (This->top == This->values)
    {
        This->top = NULL;
        This->size = 0;
        *value = This->values[0];
        ASSERT_OK(Stack, This);
        return true;
    }
    else
    {
        This->size --;
        *value = (*(This->top --));
        ASSERT_OK(Stack, This);
        return true;
    }
}

#endif // STACK_H_INCLUDED
