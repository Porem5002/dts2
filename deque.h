#ifndef DTS2_DEQUE_HEADER
#define DTS2_DEQUE_HEADER

#include <stdlib.h>

#define DTS2_DEQUE_DEFAULT_ELEM_CAP ((size_t)8)
#define DTS2_DEQUE_ELEM_CAP_OR_DEFAULT(ELEM_CAP) ((ELEM_CAP) != 0 ? (ELEM_CAP) : DTS2_DEQUE_DEFAULT_ELEM_CAP)

#define deque(TYPE) struct { size_t size; size_t element_cap; size_t block_cap; TYPE** blocks; }

#define deque_capacity(DEQUE_PTR) ((DEQUE_PTR)->element_cap*(DEQUE_PTR)->block_cap)
#define deque_cap(DEQUE_PTR) deque_capacity(DEQUE_PTR)

#define deque_init_new(ELEM_CAP) \
{ \
    .size = 0, \
    .element_cap = DTS2_DEQUE_ELEM_CAP_OR_DEFAULT(ELEM_CAP), \
    .block_cap = 0, \
    .blocks = NULL \
}

#define deque_new(DEQUE_PTR, ELEM_CAP) do \
{ \
    (DEQUE_PTR)->size = 0; \
    (DEQUE_PTR)->element_cap = DTS2_DEQUE_ELEM_CAP_OR_DEFAULT(ELEM_CAP); \
    (DEQUE_PTR)->block_cap = 0; \
    (DEQUE_PTR)->blocks = NULL; \
} while (0)

#define deque_freeEx(DEQUE_PTR, FREE_FUNC) do \
{ \
    for (size_t i = 0; i < (DEQUE_PTR)->block_cap; i++) \
        (FREE_FUNC)((DEQUE_PTR)->blocks[i]); \
    (FREE_FUNC)((DEQUE_PTR)->blocks); \
    \
    (DEQUE_PTR)->size = 0; \
    (DEQUE_PTR)->element_cap = 0; \
    (DEQUE_PTR)->block_cap = 0; \
    (DEQUE_PTR)->blocks = NULL; \
} while(0)

#define deque_free(DEQUE_PTR) deque_freeEx(DEQUE_PTR, free)

#define deque_addEx(DEQUE_PTR, VALUE, ALLOC_FUNC, REALLOC_FUNC) do \
{ \
    if((DEQUE_PTR)->element_cap * (DEQUE_PTR)->block_cap <= (DEQUE_PTR)->size) \
    { \
        const size_t sizeof_Tptr = sizeof(*(DEQUE_PTR)->blocks); \
        const size_t sizeof_T = sizeof(**(DEQUE_PTR)->blocks); \
        \
        (DEQUE_PTR)->block_cap++; \
        (DEQUE_PTR)->blocks = (REALLOC_FUNC)((DEQUE_PTR)->blocks, (DEQUE_PTR)->block_cap*sizeof_Tptr); \
        (DEQUE_PTR)->blocks[(DEQUE_PTR)->block_cap-1] = (ALLOC_FUNC)((DEQUE_PTR)->element_cap*sizeof_T); \
    } \
    \
    const size_t index = (DEQUE_PTR)->size; \
    (DEQUE_PTR)->blocks[(index) / (DEQUE_PTR)->element_cap][(index) % (DEQUE_PTR)->element_cap] = (VALUE); \
    (DEQUE_PTR)->size++; \
} while (0)

#define deque_add(DEQUE_PTR, VALUE) deque_addEx(DEQUE_PTR, VALUE, malloc, realloc)
#define deque_rem(DEQUE_PTR) do { if((DEQUE_PTR)->size > 0) (DEQUE_PTR)->size--; } while(0)

#ifndef DTS2_DEBUG_ACTIVE

#define deque_eptr(DEQUE_PTR, INDEX) (&((DEQUE_PTR)->blocks[(INDEX) / (DEQUE_PTR)->element_cap][(INDEX) % (DEQUE_PTR)->element_cap]))
#define deque_eget(DEQUE_PTR, INDEX) ((DEQUE_PTR)->blocks[(INDEX) / (DEQUE_PTR)->element_cap][(INDEX) % (DEQUE_PTR)->element_cap])
#define deque_eset(DEQUE_PTR, INDEX, VALUE) ((DEQUE_PTR)->blocks[(INDEX) / (DEQUE_PTR)->element_cap][(INDEX) % (DEQUE_PTR)->element_cap] = (VALUE))

#else

#define deque_eptr(DEQUE_PTR, INDEX) (dts2_debug_deque_index(__LINE__, __FILE__, (INDEX), (DEQUE_PTR)->size, deque_cap(DEQUE_PTR)) ? \
    (&((DEQUE_PTR)->blocks[(INDEX) / (DEQUE_PTR)->element_cap][(INDEX) % (DEQUE_PTR)->element_cap])) : \
    (&((DEQUE_PTR)->blocks[(INDEX) / (DEQUE_PTR)->element_cap][(INDEX) % (DEQUE_PTR)->element_cap])))

#define deque_eget(DEQUE_PTR, INDEX) (dts2_debug_deque_index(__LINE__, __FILE__, (INDEX), (DEQUE_PTR)->size, deque_cap(DEQUE_PTR)) ? \
    ((DEQUE_PTR)->blocks[(INDEX) / (DEQUE_PTR)->element_cap][(INDEX) % (DEQUE_PTR)->element_cap]) : \
    ((DEQUE_PTR)->blocks[(INDEX) / (DEQUE_PTR)->element_cap][(INDEX) % (DEQUE_PTR)->element_cap]))
    
#define deque_eset(DEQUE_PTR, INDEX, VALUE) do \
{ \
    dts2_debug_deque_index(__LINE__, __FILE__, (INDEX), (DEQUE_PTR)->size, deque_cap(DEQUE_PTR)); \
    (DEQUE_PTR)->blocks[(INDEX) / (DEQUE_PTR)->element_cap][(INDEX) % (DEQUE_PTR)->element_cap] = (VALUE); \
} while (0)

static inline int dts2_debug_deque_index(int line, const char* file, size_t index, size_t size, size_t capacity)
{
    if(index < size) return 1;

    fprintf(stderr, "DTS2 ERROR at %s:%d: Accessing an invalid element of a deque! (index: %d, deque size: %d, deque capacity: %d)\n", file, line, (int)index, (int)size, (int)capacity);
    exit(EXIT_FAILURE);
    return 0;
}

#endif

#endif