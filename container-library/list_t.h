#ifndef LIST_T_H_INCLUDED
#define LIST_T_H_INCLUDED

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

#define LIST_MIN_SIZE 16
/// More comfortable dump
#define list_node_t_dump(this) list_node_t_dump_(this, #this)
/// More comfortable dump
#define list_t_dump(this) list_t_dump_(this, #this)
/// Maximum amount of items to be printed
#define LIST_PRINT 50
/// Reallocation multiplier: during the allocation the size will
/// be increased by REALLOC_MULT times
#define REALLOC_MULT 2

/**
@brief Element of visa-versa linked list of strings.
The element containes word and provides information about its neighbors.
*/
typedef struct list_node_t list_node_t;
/**
@brief Visa-versa linked list structure storing elements of type list_node_t.
*/
typedef struct list_t list_t;

struct list_t
{
    size_t size;/**< Amount of elements in the list */
    size_t max_size;/**< Maximum amount of elements can be stored*/

    list_node_t* storage;/**< Pointer to the local storage*/

    size_t first;/**< First element in the list. */
    size_t last;/**< Last element in the list. */

    size_t free;/**< First free element. */
    bool is_sorted;/**< Is the storage sorted. */
    bool is_valid;/**< Validity of the list*/
};

struct list_node_t
{
    char* word;/**< String that stores word. */
    size_t amount;/**< Amount of times the word has been inserted */

    size_t next;/**< Pointer to the next element. */
    size_t prev;/**< Pointer to the previous element. */
    size_t pos;/**< This node number in list */
    list_t* head;/**< Pointer to the head of the list */

    bool is_valid;/**< State of the element. 1 if valid, 0 otherwise. */
    bool is_allocated;/**< Was the memory for the string allocated */
};

/**
*@brief Standard list element constructor writes word to the element.
*
*Constructs list element and puts the given word in it.
*@param this Pointer to the element to be constructed.
*@param word String with word to be put in the element
*@return 1 (1) if success, 0 (0) otherwise.
*/
void list_node_t_construct (list_node_t* this, char word[]);
/**
*@brief Standard list element destructor.
*
*Destructs the given element.
*@param this Pointer to the element to be destructed
*/
void list_node_t_destruct (list_node_t* this);
/**
*@brief Checks if the element is in a list already
*
*Checks the links of the element.
*@param this Pointer to the element to be checked
*/
bool list_node_t_linked (const list_node_t* this);
/**
*@brief Standard list element varificator.
*
*Checks if the given element is valid
*@param this Pointer to the element to be checked
*/
bool list_node_t_OK (const list_node_t* this);
/**
*@brief Prints element's dump.
*
*Outputs the current status of element.
*@param this Pointer to the element to be dumped.
*/
void list_node_t_dump_ (const list_node_t* this, const char name[]);
/**
*@brief Inserts the given element before the other.
*Inserts the given element before the other. Links are changed.
*
*@param this Pointer to the element to be inserted.
*@param after Pointer to the element this will be inserted after.
*@return 1 if success, 0 otherwise
*/
bool list_node_t_insert_after (list_node_t* this, list_node_t* after);
/**
*@brief Inserts the given element before the other.
*Inserts the given element before the other. Links are changed.
*
*@param this Pointer to the element to be inserted.
*@param before Pointer to the element this will be inserted before.
*@return 1 if success, 0 otherwise
*/
bool list_node_t_insert_before (list_node_t* this, list_node_t* before);
/**
*@brief Deletes the given element
*Deletes the given element. Links are changed.
*
*@param this Pointer to the element to be deleted
*@return 1 if success, 0 otherwise
*/
void list_node_t_remove (list_node_t* this);
/**
*@brief Standard list head constructor.
*
*Standard list-_t constructor.
*@param this Pointer to the head to be constructed.
*@param size Starting size of list. If 0 is passed, default value is used
*@return 1 (1) if success, 0 (0) otherwise.
*/
bool list_t_construct (list_t* this, size_t size);
/**
*@brief Standard list head destructor.
*
*Destructs the given head.
*@param this Pointer to the head to be destructed
*@warning The whole list will be destructed!
*/
void list_t_destruct (list_t* this);
/**
*@brief Standard list head varificator.
*
*Checks if the given head is valid. First LIST_PRINT elements are also checked.
*@param this Pointer to the head to be checked
*/
bool list_t_OK (const list_t* this);
/**
*@brief Prints head's dump.
*
*Outputs the current status of head and printf first LIST_PRINT elements.
*@param this Pointer to the head to be dumped.
*/
void list_t_dump_ (const list_t* this, const char name[]);
/**
*@brief Adds one more element to the list.
*The given element is added to the list. Elements with the same
*words are being treated as equal.
*@param this Pointer to the head to add element to.
*@param element List element to be added
*/
void list_t_add (list_t* this, char word[]);

inline bool list_t_is_empty (list_t* this)
{
    return (this->first < SIZE_MAX)? false : true;
}

inline bool list_t_is_full (list_t* this)
{
    return (this->free == SIZE_MAX)? true : false;
}

bool list_t_construct (list_t* this, size_t size)
{
    assert (this);
    this->max_size = (size == 0)? LIST_MIN_SIZE : size;
    this->size = 0;

    this->storage = (list_node_t*)calloc(this->max_size, sizeof(list_node_t));
    if (!this->storage)
    {
        BOOM();
        list_t_destruct(this);

        return false;
    }
    list_node_t* storage = this->storage;
    // Init free memory map
    this->free = 0;
    size_t i = 0;
    for (; i < this->max_size - 1; i++){
        storage[i].next = i + 1;
        storage[i].is_valid = false;
    }
    // Invaidate the last one
    storage[i].next = SIZE_MAX;

    this->is_valid = true;
    this->is_sorted = true;

    // Setting with poison
    this->first = SIZE_MAX;
    this->last = SIZE_MAX;

    //list_t_dump(this);
    //getchar();
    return true;
}

bool list_t_realloc (list_t* this, size_t new_size)
{
    ASSERT_OK(list_t, this);
    assert (new_size > 0);

    this->storage = (list_node_t*)realloc(this->storage, new_size * sizeof(list_node_t));
    list_node_t* storage = this->storage;
    if (!storage)
    {
        BOOM();
        list_t_destruct(this);

        return false;
    }

    // Init free memory map

    this->free = this->max_size;
    this->max_size = new_size;
    size_t i = this->free;
    for (; i < this->max_size - 1; i++){
        storage[i].next = i + 1;
        storage[i].is_valid = false;
    }
    // Invaidate the last one
    storage[i].next = SIZE_MAX;

    return true;
}

void list_t_destruct (list_t* this)
{
    assert (this);
    // Nodes are not invalidated, too slow

    // If space has been allocated
    if (this->storage)
        free(this->storage);
    this->storage = NULL;

    this->size = 0;
    this->max_size = 0;
    this->is_sorted = false;

    this->first = SIZE_MAX;
    this->last = SIZE_MAX;

    this->free = SIZE_MAX;

    this->is_valid = false;
}

list_node_t* list_t_bsearch (const char word[],
                             const list_node_t* storage,
                             const size_t size)
{
    assert (storage);
    assert (word);

    size_t lim = size;
    list_node_t* p;
    list_node_t* base = storage;
    int cmp = 0;
    // Divide by 2
    for (; lim != 0; lim >>= 1){
		p = base + (lim >> 1); // And again
		cmp = strcmp (word, p->word);
		if (cmp == 0)
			return (p);
		if (cmp > 0) {	/* key > p: move right */
			base = p++;
			lim--;
		}		/* else move left */
	}
	return (NULL);
}

list_node_t* list_t_search (const list_t* this,
                            const char word[])
{
    ASSERT_OK(list_t, this);
    assert (word);

    list_node_t* storage = this->storage;
    // If the list is sorted, binary search is better
    if (this->is_sorted)
        return list_t_bsearch (word, storage, this->size);
    // Ordinary search is used by default
    size_t i = this->first;
    list_node_t* node = NULL;
    // While element is present
    for (; i < SIZE_MAX; i = node->next){
        node = storage + i;
        if (!strcmp (node->word, word))
            break;
    }

    return (i == SIZE_MAX? NULL : node);
}

void list_node_t_construct (list_node_t* this, char word[])
{
    assert (this);
    assert (word);

    this->amount = 1;
    this->next = SIZE_MAX;
    this->prev = SIZE_MAX;
    this->word = word;
    this->is_allocated = false;
    this->is_valid = true;
}

bool list_node_t_construct_dup (list_node_t* this, const char word[])
{
    assert (this);
    assert (word);

    char* word_dup = strdup(word);

    if (!word_dup)
        return false;
    list_node_t_construct(this, word_dup);
    this->is_allocated = true;
    ASSERT_OK(list_node_t, this);

    return true;
}

void list_node_t_destruct (list_node_t* this)
{
    assert (this);
    this->is_valid = 0;
    this->pos = SIZE_MAX;
    this->head = NULL;
    this->next = SIZE_MAX;
    this->prev = SIZE_MAX;
    this->amount = 0;
    if (this->is_allocated && this->word)
        free (this->word);
    this->word = NULL;
}

list_node_t* list_t_alloc (list_t* this)
{
    ASSERT_OK(list_t, this);
    if (list_t_is_full(this))
        list_t_realloc(this, this->max_size * REALLOC_MULT);
    list_node_t* storage = this->storage;
    // Allocating space
    size_t node_pos = this->free;
   // printf ("Allocated node: %lu\n", node_pos);
    list_node_t* node = storage + node_pos;
    node->pos = node_pos;
    node->head = this;
    // Update free space address
    this->free = storage[node_pos].next;
   // printf ("Free: %lu\n", this->free);
    // We've broken the order
    this->is_sorted = false;
//        printf ("Word:%s\n", word);
//    list_t_dump(this);
 //   getchar();

    return (node);
}

void list_t_prepend (list_t* this, list_node_t* node)
{
    ASSERT_OK(list_t, this);
    assert(node);
    //Let's do the linking
    // If the list was empty, establish new first node
    size_t node_pos = node->pos;
    if (list_t_is_empty(this)){
        this->first = node_pos;
        this->last = node_pos;
    }
    else{
        this->storage[this->first].prev = node_pos;
        node->next = this->first;
        this->first = node_pos;
    }
    this->size++;
    this->last = node->pos;
}

void list_t_append (list_t* this, list_node_t* node)
{
    ASSERT_OK(list_t, this);
    assert(node);
    //Let's do the linking
    // If the list was empty, establish new first node
    size_t node_pos = node->pos;
    if (list_t_is_empty(this)){
        this->first = node_pos;
        this->last = node_pos;
    }
    else{
        this->storage[this->last].next = node_pos;
        node->prev = this->last;
        this->last = node_pos;
    }
    this->size++;
    this->last = node->pos;

  //  BOOM();
  //  list_t_dump(this);
  //  getchar();
}

// Appends node to the end of the list
// Doesn't care if there is a copy already
bool list_t_append_word (list_t* this, char word[])
{
    ASSERT_OK(list_t, this);
    list_node_t* node = list_t_alloc(this);
    // Construct node in newly allocated space
    list_node_t_construct (node, word);
    list_t_append(this, node);

    return true;
}

bool list_t_append_word_dup (list_t* this, const char word[])
{
    ASSERT_OK(list_t, this);
    list_node_t* node = list_t_alloc(this);
    // Construct node in newly allocated space
    if (!list_node_t_construct_dup (node, word)){
        list_t_destruct(this);
        return false;
    }
    list_t_append(this, node);

    return true;
}

void list_t_add (list_t* this,
                 char word[])
{
    ASSERT_OK(list_t, this);
    assert (word);

    list_node_t* node = list_t_search(this, word);
    //printf ("Search: %p\n", node);
    if (node)
        node->amount++;
    else{
        list_t_append_word(this, word);
    }
}

bool list_t_add_dup (list_t* this,
                     const char word[])
{
    ASSERT_OK(list_t, this);
    assert (word);

    list_node_t* node = list_t_search(this, word);
    if (node){
        node->amount++;
        return true;
    }
    else
        return (list_t_append_word_dup(this, word));
}

void list_t_dump_ (const list_t* this, const char name[])
{
    assert (this);
    printf ("%s = " ANSI_COLOR_BLUE "list_t" ANSI_COLOR_RESET " (", name);
    if (list_t_OK(this))
        printf (ANSI_COLOR_GREEN "ok" ANSI_COLOR_RESET ")\n");
    else
        printf (ANSI_COLOR_RED "ERROR" ANSI_COLOR_RESET ")\n");
    printf(ANSI_COLOR_YELLOW "-----------------------------------------------------" ANSI_COLOR_RESET "\n");
    printf("\t%-10s %lu\n", "size", this->size);
    printf("\t%-10s %lu\n", "max_size", this->max_size);
    if (this->first != SIZE_MAX)
        printf("\t%-10s %lu\n", "first", this->first);
    else
        printf("\t%-10s (" ANSI_COLOR_RED "NONE" ANSI_COLOR_RESET ")\n", "first");
    if (this->last != SIZE_MAX)
        printf("\t%-10s %lu\n", "last", this->last);
    else
        printf("\t%-10s (" ANSI_COLOR_RED "NONE" ANSI_COLOR_RESET ")\n", "last");
    if (this->free != SIZE_MAX)
        printf("\t%-10s %lu\n", "free", this->free);
    else
        printf("\t%-10s (" ANSI_COLOR_RED "NONE" ANSI_COLOR_RESET ")\n", "free");
    printf("\t%-10s %d\n", "is_sorted", this->is_sorted);
    printf("\t%-10s %p\n", "storage", this->storage);

    printf ("\n\t\t""Nodes:" "\n");

    list_node_t* node = NULL;
    size_t counter = 0;
    for (size_t i = this->first; counter < LIST_PRINT && i != SIZE_MAX; i = node->next){
        counter++;
        node = this->storage + i;
        printf (ANSI_COLOR_CYAN "\t[%lu]" ANSI_COLOR_RESET, i);
        list_node_t_dump(node);
    }
    printf(ANSI_COLOR_YELLOW "-----------------------------------------------------" ANSI_COLOR_RESET "\n");
}

bool list_t_OK (const list_t* this)
{
    assert (this);

    list_node_t* current = NULL;
    size_t next = SIZE_MAX;

    if (this->first != SIZE_MAX){
         current = this->storage + this->first;
         next = current->next;
     }

    bool is_valid = this->size <= this->max_size;

    for (size_t i = 0; i < LIST_PRINT && is_valid && next != SIZE_MAX; i++){
        list_node_t* next_node = this->storage + next;
        bool head_fail = current->head != this;
        bool next_fail = next_node->prev != current->pos;

        if (head_fail || next_fail || !current->is_valid)
            is_valid = false;

        current = next_node;
        next = current->next;
    }

    return is_valid;
}

bool list_node_t_OK (const list_node_t* this)
{
    assert (this);
    return this->word && this->is_valid;
}

void list_node_t_dump_ (const list_node_t* this, const char name[])
{
    assert (this);
    printf ("%s ="ANSI_COLOR_BLUE " list_node_t" ANSI_COLOR_RESET" (" , name);
    if (list_node_t_OK(this))
        printf (ANSI_COLOR_GREEN "ok" ANSI_COLOR_RESET ")\n");
    else
        printf (ANSI_COLOR_RED "ERROR" ANSI_COLOR_RESET ")\n");
    printf(ANSI_COLOR_YELLOW "-----------------------------------------------------" ANSI_COLOR_RESET "\n");
    if (this->pos == SIZE_MAX)
        printf("\t%-10s (" ANSI_COLOR_RED "NONE" ANSI_COLOR_RESET ")\n", "pos");
    else
        printf("\t%-10s %lu\n", "pos", this->pos);
    if (this->next == SIZE_MAX)
        printf("\t%-10s (" ANSI_COLOR_RED "NONE" ANSI_COLOR_RESET ")\n", "next");
    else
        printf("\t%-10s %lu\n", "next", this->next);
    if (this->prev == SIZE_MAX)
        printf("\t%-10s (" ANSI_COLOR_RED "NONE" ANSI_COLOR_RESET ")\n", "prev");
    else
        printf("\t%-10s %lu\n", "prev", this->prev);
    printf("\t%-10s %p\n", "head", this->head);
    printf("\t%-10s \"%s\"\n", "word", this->word);
    printf("\t%-10s %lu\n", "amount", this->amount);
    printf(ANSI_COLOR_YELLOW "-----------------------------------------------------" ANSI_COLOR_RESET "\n");
}

list_node_t* list_node_t_next (const list_node_t* this)
{
    assert (this);
    if (!this->head || this->next == SIZE_MAX)
        return (NULL);
    return (this->head->storage + this->next);
}

list_node_t* list_node_t_prev (const list_node_t* this)
{
    assert (this);
    if (!this->head || this->prev == SIZE_MAX)
        return (NULL);
    return (this->head->storage + this->prev);
}

bool list_node_t_insert_after (list_node_t* this, list_node_t* after)
{
    ASSERT_OK(list_node_t, this);
    ASSERT_OK(list_node_t, after);
    assert (this->head);
    assert (after->head);

    if (this->head != after->head)
        return false;
    if (after->next == SIZE_MAX){
        list_t_append (this->head, this);
        return (true);
    }
    list_node_t* before = this->head->storage + after->next;
    this->next = after->next;
    this->prev = after->pos;

    after->next = this->pos;
    before->prev = this->pos;
    this->head->size++;

    return true;
}

bool list_node_t_insert_before (list_node_t* this, list_node_t* before)
{
    ASSERT_OK(list_node_t, this);
    ASSERT_OK(list_node_t, before);
    assert (this->head);
    assert (before->head);
    if (this->head != before->head)
        return (false);

    // If adding before the first node
    if (this->prev == SIZE_MAX){
        list_t_prepend(this->head, this);
        return (true);
    }

    list_node_t* after = this->head->storage + before->prev;
    this->next = before->pos;
    this->prev = after->pos;

    after->next = this->pos;
    before->prev = this->pos;
    this->head->size++;

    return (true);
}

void list_node_t_remove (list_node_t* this)
{
    ASSERT_OK(list_node_t, this);
    if (!this->head)
        return;

    list_t* head = this->head;
    size_t old_free = head->free;
    head->free = this->pos;
    list_node_t* storage = head->storage;

    if (this->prev == SIZE_MAX)
        this->head->first = this->next;
    else
        storage[this->prev].next = this->next;
    if (this->next == SIZE_MAX)
        this->head->last  = this->prev;
    else
        storage[this->next].prev = this->prev;
    list_node_t_destruct(this);
    this->next = old_free;
}

void list_node_t_exchange (list_node_t* first, list_node_t* second)
{
    ASSERT_OK(list_node_t, first);
    ASSERT_OK(list_node_t, second);

    list_t* head = first->head;
    list_node_t* storage = head->storage;

    // NEED TO CHAGE
    if (first->prev){
        if (first->prev != second->pos)
            storage[first->prev].next = second->pos;
    }
    else
        head->first = second->pos;

    if (second->prev){
        if (second->prev != first->pos)
            storage[second->prev].next = first->pos;
    }
    else
        head->first = first->pos;

    if (first->next && second->prev != first->pos){
        if (first->next != second->pos)
            storage[first->next].prev = second->pos;
    }
    else
        head->last = second->pos;

    if (second->next){
        if (second->next != first->pos)
            storage[second->next].prev = first->pos;
    }
    else
        head->last = first->pos;

    size_t first_prev = first->prev, first_next = first->next;
    if (second->next == first->pos){
        first->next = second->pos;
        second->next = first_next;
        second->prev = first->pos;
        first->prev = second->prev;
    }
    else if (second->prev == first->pos){
        first->prev = second->pos;
        first->next = second->next;
        second->next = first->pos;
        second->prev = first_prev;
    }
    else{
        first->prev = second->prev;
        first->next = second->next;
        second->prev = first_prev;
        second->next = first_next;
    }
}

void list_t_exchange_num (list_t* this, size_t first, size_t second)
{
    list_node_t* first_node = this->storage + first;
    list_node_t* second_node = this->storage + second;

    list_node_t_exchange(first_node, second_node);
}

inline bool list_node_t_linked (const list_node_t* this)
{
    ASSERT_OK(list_node_t, this);
    return (this->head) ? true : false;
}

#endif // LIST_T_H_INCLUDED
