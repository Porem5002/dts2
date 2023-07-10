#if !defined(DTS2_CONVERT_ARRAY_DYNARRAY_HEADER) && \
    defined(DTS2_ARRAY_HEADER) && \
    defined(DTS2_DYNARRAY_HEADER)
#define DTS2_CONVERT_ARRAY_DYNARRAY_HEADER

// Converts the dynarray that SRC_DYNARRAY_PTR points to, to an array of the same type and stores it in the array that DEST_ARRAY_PTR points to, after this operation the dynarray in SRC_DYNARRAY_PTR should not be used
#define array_from_dynarray(DEST_ARRAY_PTR, SRC_DYNARRAY_PTR) do \
{ \
    const size_t sizeof_T = sizeof(*(SRC_DYNARRAY_PTR)->data); \
    \
    (DEST_ARRAY_PTR)->size = (SRC_DYNARRAY_PTR)->size; \
    (DEST_ARRAY_PTR)->data = (SRC_DYNARRAY_PTR)->data; \
    \
    if((DEST_ARRAY_PTR)->data != NULL) \
        (DEST_ARRAY_PTR)->data = realloc((DEST_ARRAY_PTR)->data, (DEST_ARRAY_PTR)->size*sizeof_T); \
    \
} while(0)

// Converts the array that SRC_ARRAY_PTR points to, to a dynarray of the same type and stores it in the dynarray that DEST_DYNARRAY_PTR points to, after this operation the array in SRC_ARRAY_PTR should not be used
#define dynarray_from_array(DEST_DYNARRAY_PTR, SRC_ARRAY_PTR) do \
{ \
    const size_t sizeof_T = sizeof(*(SRC_ARRAY_PTR)->data); \
    \
    (DEST_DYNARRAY_PTR)->capacity = (SRC_ARRAY_PTR)->size; \
    (DEST_DYNARRAY_PTR)->size = (SRC_ARRAY_PTR)->size; \
    (DEST_DYNARRAY_PTR)->data = (SRC_ARRAY_PTR)->data; \
    \
} while(0)

#endif