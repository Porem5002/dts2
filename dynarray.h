#ifndef DTS2_DYNARRAY_HEADER
#define DTS2_DYNARRAY_HEADER

#include <stdlib.h>

#define DTS2_DYNARRAY_DEFAULT_SIZE ((size_t)4)

#define dynarray(TYPE) struct { size_t capacity; size_t size; TYPE* data; }

#define dynarray_init_newEx(CAP, TYPE, ALLOC_FUNC) { .capacity = (CAP), .size = 0, .data = (ALLOC_FUNC)((CAP)*sizeof(TYPE)) }
#define dynarray_init_new(CAP, TYPE) dynarray_init_newEx(CAP, TYPE, malloc)

#define dynarray_newEx(ARRAY_PTR, CAP, ALLOC_FUNC) do { (ARRAY_PTR)->capacity = (CAP); (ARRAY_PTR)->size = 0; (ARRAY_PTR)->data = (ALLOC_FUNC)((CAP)*(sizeof(*(ARRAY_PTR)->data)));  } while(0)
#define dynarray_new(ARRAY_PTR, CAP) dynarray_newEx(ARRAY_PTR, CAP, malloc)

#define dynarray_freeEx(ARRAY_PTR, FREE_FUNC) do { (FREE_FUNC)((ARRAY_PTR)->data); (ARRAY_PTR)->capacity = 0; (ARRAY_PTR)->size = 0; (ARRAY_PTR)->data = NULL;  } while(0)
#define dynarray_free(ARRAY_PTR) dynarray_freeEx(ARRAY_PTR, free)

#define dynarray_addEx(ARRAY_PTR, VALUE, REALLOC_FUNC, NEW_CAP_FUNC) do \
{ \
    if((ARRAY_PTR)->capacity <= (ARRAY_PTR)->size) \
    { \
        const size_t new_cap = (NEW_CAP_FUNC)((ARRAY_PTR)->capacity);\
        (ARRAY_PTR)->data = (REALLOC_FUNC)((ARRAY_PTR)->data, new_cap); \
        (ARRAY_PTR)->capacity = new_cap; \
    } \
    \
    (ARRAY_PTR)->data[(ARRAY_PTR)->size] = (VALUE); \
    (ARRAY_PTR)->size++; \
} while(0)

#define dynarray_add(ARRAY_PTR, VALUE) dynarray_addEx(ARRAY_PTR, VALUE, realloc, dts2_dynarray_calc_new_cap)

#ifndef DTS2_DEBUG_ACTIVE

#define dynarray_eptr(ARRAY_PTR, INDEX) (&((ARRAY_PTR)->data[INDEX]))
#define dynarray_eget(ARRAY_PTR, INDEX) ((ARRAY_PTR)->data[INDEX])
#define dynarray_eset(ARRAY_PTR, INDEX, VALUE) ((ARRAY_PTR)->data[INDEX] = (VALUE))

#else
#include <stdio.h>

#define dynarray_eptr(ARRAY_PTR, INDEX) (dts2_debug_dynarray_index(__LINE__, __FILE__, INDEX, (ARRAY_PTR)->size, (ARRAY_PTR)->capacity) ? (&((ARRAY_PTR)->data[INDEX])) : (&((ARRAY_PTR)->data[INDEX])))
#define dynarray_eget(ARRAY_PTR, INDEX) (dts2_debug_dynarray_index(__LINE__, __FILE__, INDEX, (ARRAY_PTR)->size, (ARRAY_PTR)->capacity) ? ((ARRAY_PTR)->data[INDEX]) : ((ARRAY_PTR)->data[INDEX]))
#define dynarray_eset(ARRAY_PTR, INDEX, VALUE) do { dts2_debug_dynarray_index(__LINE__, __FILE__, INDEX, (ARRAY_PTR)->size, (ARRAY_PTR)->capacity); ((ARRAY_PTR)->data[INDEX] = (VALUE)); } while (0)

static inline int dts2_debug_dynarray_index(int line, const char* file, size_t index, size_t size, size_t capacity)
{
    if(index < size) return 1;

    fprintf(stderr, "DTS2 ERROR at %s:%d: Accessing an invalid element of an dynarray! (index: %d, dynarray size: %d, dynarray cap: %d)\n", file, line, (int)index, (int)size, (int)capacity);
    exit(EXIT_FAILURE);
    return 0;
}

#endif

static inline size_t dts2_dynarray_calc_new_cap(size_t capacity)
{
    if(capacity == 0) return (DTS2_DYNARRAY_DEFAULT_SIZE);

    return capacity + capacity/2;
}

#endif