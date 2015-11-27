#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdio.h>
#include <assert.h>
#include <string.h>

/// More comfortable dump
#define ListElement_dump(This) list_element_dump_(This, #This)
/// To be stylish
#define list_element_dump(This) ListElement_dump(This)
/// To be stylish
#define ListElement_OK(This) list_element_OK(This)

/**
@brief Element of visa-versa linked list of strings.
The element containes word and provides information about its neighbors.
*/
typedef struct ListElement ListElement;

/**
*@brief Standard list element constructor writes word to the element.
*
*Constructs list element and puts the given word in it.
*@param This Pointer to the element to be constructed.
*@param word String with word to be put in the element
*@return 1 (true) if success, 0 (false) otherwise.
*/
bool list_element_construct (ListElement* This, const char word[]);
/**
*@brief Standard list element destructor.
*
*Destructs the given element.
*@param This Pointer to the element to be destructed
*/
void list_element_destruct (ListElement* This);
/**
*@brief Checks if the element is in a list already
*
*Checks the links of the element.
*@param This Pointer to the element to be checked
*/
bool list_element_linked (const ListElement* This);
/**
*@brief Standard list element varificator.
*
*Checks if the given element is valid
*@param This Pointer to the element to be checked
*/
bool list_element_OK (const ListElement* This);
/**
*@brief Prints element's dump.
*
*Outputs the current status of element.
*@param This Pointer to the element to be dumped.
*/
void list_element_dump_ (const ListElement* This, const char name[]);
/**
*@brief Inserts the given element before the other.
*Inserts the given element before the other. Links are changed.
*
*@param This Pointer to the element to be inserted.
*@param after Pointer to the element This will be inserted after.
*@return true if success, false otherwise
*/
bool list_element_insert_after (ListElement* This, ListElement* after);
/**
*@brief Inserts the given element before the other.
*Inserts the given element before the other. Links are changed.
*
*@param This Pointer to the element to be inserted.
*@param before Pointer to the element This will be inserted before.
*@return true if success, false otherwise
*/
bool list_element_insert_before (ListElement* This, ListElement* before);
/**
*@brief Deletes the given element
*Deletes the given element. Links are changed.
*
*@param This Pointer to the element to be deleted
*@return true if success, false otherwise
*/
void list_element_remove (ListElement* This);

/// More comfortable dump
#define ListHead_dump(This) list_head_dump_(This, #This)
/// To be stylish
#define list_head_dump(This) ListHead_dump(This)
/// To be stylish
#define ListHead_OK(This) list_head_OK(This)
/// Maximum amount of items to be printed
#define LIST_PRINT 10
typedef struct
{
    size_t amount;/**< Amount of elements in the list */

    ListElement* first;/** First element in the list. */
    bool is_valid;
} ListHead;
/**
*@brief Standard list head constructor. Sets the first element to the
*
*Standard list head constructor. Sets the first element to the given.
*@param This Pointer to the head to be constructed.
*@param first Pointer to the element to be put first
*@return 1 (true) if success, 0 (false) otherwise.
*/
bool list_head_construct (ListHead* This);
/**
*@brief Standard list head destructor.
*
*Destructs the given head.
*@param This Pointer to the head to be destructed
*@warning The whole list will be destructed!
*/
void list_head_destruct (ListHead* This);
/**
*@brief Standard list head varificator.
*
*Checks if the given head is valid. First LIST_PRINT elements are also checked.
*@param This Pointer to the head to be checked
*/
bool list_head_OK (const ListHead* This);
/**
*@brief Prints head's dump.
*
*Outputs the current status of head and printf first LIST_PRINT elements.
*@param This Pointer to the head to be dumped.
*/
void list_head_dump_ (const ListHead* This, const char name[]);
/**
*@brief Adds one more element to the list.
*The given element is added to the list. Elements with the same
*words are being treated as equal.
*@param This Pointer to the head to add element to.
*@param element List element to be added
*/
bool list_head_add (ListHead* This, const char word[]);

bool list_head_construct (ListHead* This)
{
    assert (This);
    This->amount = 0;
    This->is_valid = true;
    This->first = NULL;

    return true;
}

void list_head_destruct (ListHead* This)
{
    assert (This);
    if (This->first)
        list_element_destruct(This->first);
    This->amount = 0;
    This->is_valid = false;
}

typedef struct ListElement
{
    char* word;/**< String that stores word. */
    size_t amount;/**< Amount of times the word has been inserted */

    ListElement* next;/**< Pointer to the next element. */
    ListElement* prev;/**< Pointer to the previous element. */
    ListHead* head;/**< Pointer to the head of the list */

    bool is_valid;/**< State of the element. true if valid, false otherwise. */
} ListElement;

bool list_head_add (ListHead* This, const char word[])
{
    assert (This);
    assert (word);
    ListElement* current = This->first;
    ListElement* new_element = (ListElement*)calloc (1, sizeof(*new_element));
    list_element_construct(new_element, word);
    if (current)
    {
        while (current->next)
        {
            if (!strcmp (current->word, word))
            {
                current->amount++;
                This->amount ++;
                return true;
            }
            current = current->next;
        }
        return (list_element_insert_after(new_element, current));
    }
    else
    {
        This->first = new_element;
        new_element->head = This;
        This->amount ++;
        return true;
    }
}

void list_head_dump_ (const ListHead* This, const char name[])
{
    assert (This);
    printf ("%s = ListHead (", name);
    if (list_head_OK(This))
        printf ("ok)\n");
    else
        printf ("ERROR)\n");
    printf ("{\n");
    printf ("\tfirst = %p\n", This->first);
    printf ("\tamount = %lu\n", This->amount);
    printf ("\tList:\n");

    ListElement* current = This->first;
    for (int i =0; i < LIST_PRINT && current; i++)
    {
        printf ("\t[%d]", i);
        list_element_dump(current);
        current = current->next;
    }
    printf ("}\n");
}

bool list_head_OK (const ListHead* This)
{
    assert (This);
    ListElement* current = This->first;
    bool is_valid = true;

    for (int i =0; i < LIST_PRINT && is_valid && current != NULL; i++)
    {
        if ((current->head != This) ||
            (current->next && (current->next->prev != current)) ||
            (current->prev && (current->prev->next != current))) is_valid = false;
    }

    return is_valid;
}

bool list_element_construct (ListElement* This, const char word[])
{
    assert (This);
    assert (word);
    This->amount = 1;
    This->next = NULL;
    This->prev = NULL;
    This->head = NULL;
    This->word = strdup (word);
    if (!This->word)
    {
        list_element_destruct(This);
        return false;
    }
    This->is_valid = true;

    return true;
}

void list_element_destruct (ListElement* This)
{
    assert (This);
    This->is_valid = false;
    This->head = NULL;
    // Destructs everything after.
    if (This->next)
        list_element_destruct (This->next);
    This->next = NULL;
    if (This->prev)
        This->prev->next = NULL;
    This->prev = NULL;
    This->amount = 0;
    if (This->word)
        free (This->word);
    This->word = NULL;
    free(This);
}

bool list_element_OK (const ListElement* This)
{
    assert (This);
    return This->word && This->is_valid;
}

void list_element_dump_ (const ListElement* This, const char name[])
{
    assert (This);
    printf ("%s = ListElement (", name);
    if (list_element_OK(This))
        printf ("ok)\n");
    else
        printf ("ERROR)\n");
    printf ("{\n");
    printf ("    next = %p\n", This->next);
    printf ("    prev = %p\n", This->prev);
    printf ("    head = %p\n", This->head);
    printf ("    word = [%s]\n", This->word);
    printf ("    amount = %lu\n", This->amount);
    printf ("}\n");
}

bool list_element_insert_after (ListElement* This, ListElement* after)
{
    assert (This);
    assert (after);
    if (!list_element_linked(after))
    {
        list_element_destruct(This);
        return false;
    }

    if (list_element_linked(This))
    {
        list_element_destruct(This);
        return false;
    }
    list_element_OK(This);
    list_element_OK(after);

    This->head = after->head;

    This->next = after->next;
    This->prev = after;

    if (after->next)
        after->next->prev = This;
    after->next = This;
    This->head->amount++;

    return true;
}

bool list_element_insert_before (ListElement* This, ListElement* before)
{
    assert (This);
    assert (before);
    if (!list_element_linked(before))
    {
        list_element_destruct(This);
        return false;
    }
    if (list_element_linked(This))
    {
        list_element_destruct(This);
        return false;
    }
    list_element_OK(This);
    list_element_OK(before);

    This->head = before->head;

    This->next = before;
    This->prev = before->prev;

    if (before->prev)
        before->prev->next = This;
    else
        before->head->first = This;
    before->prev = This;
    This->head->amount++;

    return true;
}

void list_element_remove (ListElement* This)
{
    list_element_OK(This);

    if (This->prev)
        This->prev->next = This->next;
    if (This->next)
        This->next->prev = This->prev;
    This->head = NULL;
    This->next = NULL;
    This->prev = NULL;
}

bool list_element_linked (const ListElement* This)
{
    return This->head;
}

#endif // LIST_H_INCLUDED
