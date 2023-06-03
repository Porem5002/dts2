#ifndef DTS2_ARRAY_HEADER
#define DTS2_ARRAY_HEADER

#include <stdlib.h>

#define array(TYPE) struct { size_t size; TYPE* data; }

#define array_init_alias1(SRC_PTR) { .size = (SRC_PTR)->size, .data = (SRC_PTR)->data }
#define array_init_alias2(SIZE, DATA_PTR) { .size = (SIZE), .data = (DATA_PTR) }

#define array_init_newEx(SIZE, TYPE, ALLOC_FUNC) { .size = (SIZE), .data = (ALLOC_FUNC)((SIZE)*sizeof(TYPE)) }
#define array_init_new(SIZE, TYPE) array_init_newEx(SIZE, TYPE, malloc)

#define array_alias1(ARRAY_PTR, SRC_PTR) do { (ARRAY_PTR)->size = (SRC_PTR)->size; (ARRAY_PTR)->data = (SRC_PTR)->data;  } while(0)
#define array_alias2(ARRAY_PTR, SIZE, DATA_PTR) do { (ARRAY_PTR)->size = (SIZE); (ARRAY_PTR)->data = (DATA_PTR);  } while(0)

#define array_newEx(ARRAY_PTR, SIZE, ALLOC_FUNC) do { (ARRAY_PTR)->size = SIZE; (ARRAY_PTR)->data = (ALLOC_FUNC)((SIZE)*(sizeof(*(ARRAY_PTR)->data)));  } while(0)
#define array_new(ARRAY_PTR, SIZE) array_newEx(ARRAY_PTR, SIZE, malloc)

#define array_freeEx(ARRAY_PTR, FREE_FUNC) do { (FREE_FUNC)((ARRAY_PTR)->data); (ARRAY_PTR)->size = 0; (ARRAY_PTR)->data = NULL;  } while(0)
#define array_free(ARRAY_PTR) array_freeEx(ARRAY_PTR, free)

#ifndef DTS2_DEBUG_ACTIVE 

#define array_eptr(ARRAY_PTR, INDEX) (&((ARRAY_PTR)->data[INDEX]))
#define array_eget(ARRAY_PTR, INDEX) ((ARRAY_PTR)->data[INDEX])
#define array_eset(ARRAY_PTR, INDEX, VALUE) ((ARRAY_PTR)->data[INDEX] = (VALUE))

#else
#include <stdio.h>

#define array_eptr(ARRAY_PTR, INDEX) (dts2_debug_array_index(__LINE__, __FILE__, INDEX, (ARRAY_PTR)->size) ? (&((ARRAY_PTR)->data[INDEX])) : (&((ARRAY_PTR)->data[INDEX])))
#define array_eget(ARRAY_PTR, INDEX) (dts2_debug_array_index(__LINE__, __FILE__, INDEX, (ARRAY_PTR)->size) ? ((ARRAY_PTR)->data[INDEX]) : ((ARRAY_PTR)->data[INDEX]))
#define array_eset(ARRAY_PTR, INDEX, VALUE) do { dts2_debug_array_index(__LINE__, __FILE__, INDEX, (ARRAY_PTR)->size); ((ARRAY_PTR)->data[INDEX] = (VALUE)); } while (0)

static inline int dts2_debug_array_index(int line, const char* file, size_t index, size_t size)
{
    if(index < size) return 1;

    fprintf(stderr, "DTS2 ERROR at %s:%d: Accessing an invalid element of an array! (index: %d, array size: %d)\n", file, line, (int)index, (int)size);
    exit(EXIT_FAILURE);
    return 0;
}

#endif

#endif