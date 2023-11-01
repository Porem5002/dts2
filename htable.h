#ifndef DTS2_HTABLE_HEADER
#define DTS2_HTABLE_HEADER

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define dts2_var(NAME) dts2_var2(NAME, __LINE__)
#define dts2_var2(NAME, LINE) dts2_var3(NAME, LINE)
#define dts2_var3(NAME, LINE) NAME##_l##LINE

#define typedef_htnode(TKEY, TVALUE, NAME) \
typedef struct NAME NAME; \
struct NAME \
{ \
    TKEY key; \
    TVALUE value; \
    NAME* next; \
}

#define typedef_htable(TKEY, TNODE, NODE_CAP, NAME) \
typedef struct NAME NAME; \
struct NAME \
{ \
    size_t (*hash_func) (TKEY, size_t); \
    TNODE* nodes [NODE_CAP]; \
}

#define htable_cap(TABLE_PTR) (sizeof((TABLE_PTR)->nodes)/sizeof((TABLE_PTR)->nodes[0]))

#define htable_set(TNODE, TABLE_PTR, KEY, VALUE) do \
{ \
    size_t dts2_var(hash) = (TABLE_PTR)->hash_func((KEY), htable_cap(TABLE_PTR)); \
    \
    TNODE** dts2_var(node) = &(TABLE_PTR)->nodes[dts2_var(hash)]; \
    \
    while(*dts2_var(node) != NULL) \
    {\
        if((*dts2_var(node))->key == (KEY)) \
        { \
            (*dts2_var(node))->value = (VALUE); \
            goto dts2_var(htable_set_end); \
        } \
        \
        dts2_var(node) = &(*dts2_var(node))->next; \
    } \
    \
    TNODE* dts2_var(new_node) = malloc(sizeof(TNODE)); \
    dts2_var(new_node)->key = (KEY); \
    dts2_var(new_node)->value = (VALUE); \
    dts2_var(new_node)->next = NULL; \
    \
    (*dts2_var(node)) = dts2_var(new_node); \
dts2_var(htable_set_end): \
    ; \
} while (0)

#define htable_getptr(TNODE, TABLE_PTR, KEY, DEST_PTR) do \
{\
    size_t dts2_var(hash) = (TABLE_PTR)->hash_func((KEY), htable_cap(TABLE_PTR));\
    \
    TNODE** dts2_var(node) = &(TABLE_PTR)->nodes[dts2_var(hash)];\
    \
    while(*dts2_var(node) != NULL)\
    {\
        if((*dts2_var(node))->key == (KEY))\
        {\
            *(DEST_PTR) = &(*dts2_var(node))->value;\
            goto dts2_var(htable_getptr_end);\
        }\
        \
        dts2_var(node) = &(*dts2_var(node))->next;\
    }\
    \
    *(DEST_PTR) = NULL;\
dts2_var(htable_getptr_end):\
    ;\
} while(0);

#define htable_get(TNODE, TVALUE, TABLE_PTR, KEY, OUT_CHECK_PTR, OUT_VALUE_PTR) do \
{ \
    TVALUE* dts2_var(htable_get_valptr); \
    htable_getptr(TNODE, (TABLE_PTR), (KEY), &dts2_var(htable_get_valptr)); \
    \
    if(dts2_var(htable_get_valptr) != NULL) \
    {\
        *(OUT_VALUE_PTR) = *dts2_var(htable_get_valptr);\
        break;\
    }\
    \
    *(OUT_CHECK_PTR) = dts2_var(htable_get_valptr) != NULL;\
} while(0);

#define htable_contains(TNODE, TVALUE, TABLE_PTR, KEY, DEST_PTR) do \
{ \
    TVALUE* dts2_var(htable_get_valptr); \
    htable_getptr(TNODE, (TABLE_PTR), (KEY), &dts2_var(htable_get_valptr)); \
    *(DEST_PTR) = dts2_var(htable_get_valptr) != NULL;\
} while(0);

#define htable_free(TNODE, TABLE_PTR) do \
{\
    for(size_t dts2_var(i) = 0; dts2_var(i) < htable_cap(TABLE_PTR); dts2_var(i)++)\
    {\
        TNODE* dts2_var(node) = (TABLE_PTR)->nodes[dts2_var(i)]; \
        \
        while(dts2_var(node) != NULL)\
        {\
            TNODE* dts2_var(next) = dts2_var(node)->next;\
            free(dts2_var(node));\
            dts2_var(node) = dts2_var(next);\
        }\
    }\
    \
    memset((TABLE_PTR), 0, sizeof(*(TABLE_PTR)));\
} while(0);

#endif