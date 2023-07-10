#ifndef DTS2_DYNARRAY_HEADER
#define DTS2_DYNARRAY_HEADER

#include <stdlib.h>
#include <string.h>

#define DTS2_DYNARRAY_DEFAULT_SIZE ((size_t)4)

// Represents a dynarray of TYPE
#define dynarray(TYPE) struct { size_t capacity; size_t size; TYPE* data; }

// Same as 'dynarray_init_new' but allows the programmer to specify an allocation function to use instead of malloc
#define dynarray_init_newEx(CAP, TYPE, ALLOC_FUNC) { .capacity = (CAP), .size = 0, .data = (ALLOC_FUNC)((CAP)*sizeof(TYPE)) }
// Initializes a new dynarray of type TYPE with initial capacity CAP. To allocate memory that satisfies the initial capacity, the C's malloc function is used
#define dynarray_init_new(CAP, TYPE) dynarray_init_newEx(CAP, TYPE, malloc)

// Assigns the value 'dynarray_init_newEx(CAP, TYPE, ALLOC_FUNC)' to the array that ARRAY_PTR points to
#define dynarray_newEx(ARRAY_PTR, CAP, ALLOC_FUNC) do { (ARRAY_PTR)->capacity = (CAP); (ARRAY_PTR)->size = 0; (ARRAY_PTR)->data = (ALLOC_FUNC)((CAP)*(sizeof(*(ARRAY_PTR)->data)));  } while(0)
// Assigns the value 'dynarray_init_new(CAP, TYPE)' to the array that ARRAY_PTR points to
#define dynarray_new(ARRAY_PTR, CAP) dynarray_newEx(ARRAY_PTR, CAP, malloc)

// Same as 'dynarray_free' but allows the programmer to specify a free function to use instead of C's free
#define dynarray_freeEx(ARRAY_PTR, FREE_FUNC) do { (FREE_FUNC)((ARRAY_PTR)->data); (ARRAY_PTR)->capacity = 0; (ARRAY_PTR)->size = 0; (ARRAY_PTR)->data = NULL;  } while(0)
// Frees the dynarray that ARRAY_PTR points to using C's free function
#define dynarray_free(ARRAY_PTR) dynarray_freeEx(ARRAY_PTR, free)

// Same as 'dynarray_add' but allows the programmer to specify a reallocation function and a function to calculate the new capacity
#define dynarray_addEx(ARRAY_PTR, VALUE, REALLOC_FUNC, NEW_CAP_FUNC) do \
{ \
    const size_t sizeof_T = sizeof(*(ARRAY_PTR)->data); \
    \
    if((ARRAY_PTR)->capacity <= (ARRAY_PTR)->size) \
    { \
        const size_t new_cap = (NEW_CAP_FUNC)((ARRAY_PTR)->capacity);\
        (ARRAY_PTR)->data = (REALLOC_FUNC)((ARRAY_PTR)->data, sizeof_T*new_cap); \
        (ARRAY_PTR)->capacity = new_cap; \
    } \
    \
    (ARRAY_PTR)->data[(ARRAY_PTR)->size] = (VALUE); \
    (ARRAY_PTR)->size++; \
} while(0)

// Adds a new element with value VALUE to the end of the dynarray that ARRAT_PTR points to. Reallocation is done with C's realloc
#define dynarray_add(ARRAY_PTR, VALUE) dynarray_addEx(ARRAY_PTR, VALUE, realloc, dts2_dynarray_calc_new_cap)

// Removes the last element of the dynarray that ARRAT_PTR points to
#define dynarray_rem(ARRAY_PTR) do { if((ARRAY_PTR)->size > 0) (ARRAY_PTR)->size--; } while(0)

// Adds a new element with value VALUE at index INDEX of the dynarray that ARRAT_PTR points to. All elements with index >= INDEX will be pushed forward and VALUE will be placed at index INDEX 
#define dynarray_addat(ARRAY_PTR, INDEX, VALUE) dynarray_addatEx(ARRAY_PTR, INDEX, VALUE, realloc, dts2_dynarray_calc_new_cap)

#ifndef DTS2_DEBUG_ACTIVE

// Same as 'dynarray_addat' but allows the programmer to specify a reallocation function and a function to calculate the new capacity
#define dynarray_addatEx(ARRAY_PTR, INDEX, VALUE, REALLOC_FUNC, NEW_CAP_FUNC) do \
{ \
    const size_t sizeof_T = sizeof(*(ARRAY_PTR)->data); \
    \
    if((ARRAY_PTR)->capacity <= (ARRAY_PTR)->size) \
    { \
        const size_t new_cap = (NEW_CAP_FUNC)((ARRAY_PTR)->capacity);\
        (ARRAY_PTR)->data = (REALLOC_FUNC)((ARRAY_PTR)->data, sizeof_T*new_cap); \
        (ARRAY_PTR)->capacity = new_cap; \
    } \
    \
    memmove(&(ARRAY_PTR)->data[(INDEX)+1], &(ARRAY_PTR)->data[(INDEX)], sizeof_T*((ARRAY_PTR)->size-(INDEX))); \
    (ARRAY_PTR)->data[(INDEX)] = (VALUE); \
    (ARRAY_PTR)->size++; \
} while(0)

#else

// Same as 'dynarray_addat' but allows the programmer to specify a reallocation function and a function to calculate the new capacity
#define dynarray_addatEx(ARRAY_PTR, INDEX, VALUE, REALLOC_FUNC, NEW_CAP_FUNC) do \
{ \
    if((INDEX) < 0 || (INDEX) > (ARRAY_PTR)->size) \
    { \
        fprintf(stderr, "DTS2 ERROR at %s:%d: Adding an element at an invalid index of a dynarray!\n", __FILE__, __LINE__); \
        fprintf(stderr, "\tdynarray info -> (index: %u, dynarray size: %u, dynarray cap: %u)\n", (unsigned)(INDEX), (unsigned)(ARRAY_PTR)->size, (unsigned)(ARRAY_PTR)->capacity); \
        fprintf(stderr, "\tvalid index range -> (min: 0, max: size)\n"); \
        exit(EXIT_FAILURE); \
    } \
    \
    const size_t sizeof_T = sizeof(*(ARRAY_PTR)->data); \
    \
    if((ARRAY_PTR)->capacity <= (ARRAY_PTR)->size) \
    { \
        const size_t new_cap = (NEW_CAP_FUNC)((ARRAY_PTR)->capacity);\
        (ARRAY_PTR)->data = (REALLOC_FUNC)((ARRAY_PTR)->data, sizeof_T*new_cap); \
        (ARRAY_PTR)->capacity = new_cap; \
    } \
    \
    memmove(&(ARRAY_PTR)->data[(INDEX)+1], &(ARRAY_PTR)->data[(INDEX)], sizeof_T*((ARRAY_PTR)->size-(INDEX))); \
    (ARRAY_PTR)->data[(INDEX)] = (VALUE); \
    (ARRAY_PTR)->size++; \
} while(0)

#endif

#ifndef DTS2_DEBUG_ACTIVE

// Removes element of index INDEX from the dynarray that ARRAY_PTR points to. If the dynarray is empty no operation is perfomed
#define dynarray_remat(ARRAY_PTR, INDEX) do \
{ \
    const size_t sizeof_T = sizeof(*(ARRAY_PTR)->data); \
    memmove(&(ARRAY_PTR)->data[(INDEX)], &(ARRAY_PTR)->data[(INDEX)+1], sizeof_T*((ARRAY_PTR)->size-(INDEX)-1)); \
    (ARRAY_PTR)->size--; \
} while(0)\

#else

// Removes element of index INDEX from the dynarray that ARRAY_PTR points to. If the dynarray is empty no operation is perfomed
#define dynarray_remat(ARRAY_PTR, INDEX) do \
{ \
    if((INDEX) < 0 || (INDEX) >= (ARRAY_PTR)->size) \
    { \
        fprintf(stderr, "DTS2 ERROR at %s:%d: Removing an element at an invalid index of a dynarray!\n", __FILE__, __LINE__); \
        fprintf(stderr, "\tdynarray info -> (index: %u, dynarray size: %u, dynarray cap: %u)\n", (unsigned)(INDEX), (unsigned)(ARRAY_PTR)->size, (unsigned)(ARRAY_PTR)->capacity); \
        fprintf(stderr, "\tvalid index range -> (min: 0, max: size-1)\n"); \
        exit(EXIT_FAILURE); \
    } \
    \
    const size_t sizeof_T = sizeof(*(ARRAY_PTR)->data); \
    memmove(&(ARRAY_PTR)->data[(INDEX)], &(ARRAY_PTR)->data[(INDEX)+1], sizeof_T*((ARRAY_PTR)->size-(INDEX)-1)); \
    (ARRAY_PTR)->size--; \
} while(0)\

#endif

#ifndef DTS2_DEBUG_ACTIVE

// Returns a pointer to the element of index INDEX of the dynarray that ARRAY_PTR points to and performs bounds checking if DTS2_DEBUG_ACTIVE is defined
#define dynarray_eptr(ARRAY_PTR, INDEX) (&((ARRAY_PTR)->data[INDEX]))

// Returns the value of the element of index INDEX of the dynarray that ARRAY_PTR points to and performs bounds checking if DTS2_DEBUG_ACTIVE is defined
#define dynarray_eget(ARRAY_PTR, INDEX) ((ARRAY_PTR)->data[INDEX])

// Assigns VALUE to the element of index INDEX of the dynarray that ARRAY_PTR points to and performs bounds checking if DTS2_DEBUG_ACTIVE is defined
#define dynarray_eset(ARRAY_PTR, INDEX, VALUE) ((ARRAY_PTR)->data[INDEX] = (VALUE))

#else
#include <stdio.h>

// Returns a pointer to the element of index INDEX of the dynarray that ARRAY_PTR points to and performs bounds checking if DTS2_DEBUG_ACTIVE is defined
#define dynarray_eptr(ARRAY_PTR, INDEX) (dts2_debug_dynarray_index(__LINE__, __FILE__, INDEX, (ARRAY_PTR)->size, (ARRAY_PTR)->capacity) ? (&((ARRAY_PTR)->data[INDEX])) : (&((ARRAY_PTR)->data[INDEX])))

// Returns the value of the element of index INDEX of the dynarray that ARRAY_PTR points to and performs bounds checking if DTS2_DEBUG_ACTIVE is defined
#define dynarray_eget(ARRAY_PTR, INDEX) (dts2_debug_dynarray_index(__LINE__, __FILE__, INDEX, (ARRAY_PTR)->size, (ARRAY_PTR)->capacity) ? ((ARRAY_PTR)->data[INDEX]) : ((ARRAY_PTR)->data[INDEX]))

// Assigns VALUE to the element of index INDEX of the dynarray that ARRAY_PTR points to and performs bounds checking if DTS2_DEBUG_ACTIVE is defined
#define dynarray_eset(ARRAY_PTR, INDEX, VALUE) do { dts2_debug_dynarray_index(__LINE__, __FILE__, INDEX, (ARRAY_PTR)->size, (ARRAY_PTR)->capacity); ((ARRAY_PTR)->data[INDEX] = (VALUE)); } while (0)

static inline int dts2_debug_dynarray_index(int line, const char* file, size_t index, size_t size, size_t capacity)
{
    if(index < size) return 1;

    fprintf(stderr, "DTS2 ERROR at %s:%d: Accessing an invalid element of an dynarray! (index: %d, dynarray size: %d, dynarray cap: %d)\n", file, line, (int)index, (int)size, (int)capacity);
    exit(EXIT_FAILURE);
    return 0;
}

#endif
// Default function used by dts2 to calculate the new capacity of a dynarray
static inline size_t dts2_dynarray_calc_new_cap(size_t capacity)
{
    if(capacity == 0) return (DTS2_DYNARRAY_DEFAULT_SIZE);

    return capacity + capacity/2;
}

#endif