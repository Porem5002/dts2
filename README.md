# dts2
A simple library that provides various generic data structures for C programmers.

# Setup
To setup the library for your project do the following:

- Create new directory inside your project's directory and call it "dts2"

- Copy the the **LICENSE** file and all files ending in **.h** to the "dts2" directory

The library uses C99 features, so to compile a project that uses it, make sure you have a compiler that supports the C99 standard.

# Usage
The library is composed of a bunch of header files, each of those files represents a data structure or feature.

To use one of those constructs, first you need to include the file that contains it, for example:

```C
#include "dts2/array.h" // This will allow you to use arrays
```

After including the functionality that you want, you can now use it in your code.

Additionally, there's also a macro called **DTS2_DEBUG_ACTIVE** that you can define when compiling your project. If defined, this will enable multiple checks and assertions, inside of the functions and macros of the library, that will help you find bugs in your code.

## Demonstrations
Here are some demonstrations of the different data structures and features that the library supports.

### Arrays
```C
#include <stdio.h>
// Allow usage of arrays
#include "dts2/array.h"

int main()
{
    array(int) a = array_init_new(5, int);

    array_eset(&a, 0, 100);
    array_eset(&a, 1, 261);
    array_eset(&a, 2, 123);
    array_eset(&a, 3, 90);
    array_eset(&a, 4, 456);

    for(size_t i = 0; i < a.size; i++)
    {
        int v = array_eget(&a, i);
        printf("%d ", v);
    }

    array_free(&a);
    return 0;
}

// This will print:
// 100 261 123 90 456
```

### Dynamic Arrays
```C
#include <stdio.h>
// Allow usage of dynamic arrays
#include "dts2/dynarray.h"

int main()
{
    dynarray(const char*) a = dynarray_init_new(0, const char*);

    dynarray_add(&a, "John");
    dynarray_add(&a, "Bob");
    dynarray_add(&a, "Adam");
    
    // Add "Joe" at index 1, by pushing every element
    // with index >= 1 forward and
    // setting value at index 1 to "Joe"
    dynarray_addat(&a, 1, "Joe");
    
    dynarray_addat(&a, 1, "Guy");

    // Removes element at index 1
    dynarray_remat(&a, 1);
    // Removes last element
    dynarray_rem(&a);

    for(size_t i = 0; i < a.size; i++)
    {
        const char* v = dynarray_eget(&a, i);
        printf("%s ", v);
    }

    dynarray_free(&a);
    return 0;
}

// This will print:
// John Joe Bob
```

### Deques
```C
#include <stdio.h>
// Allow usage of deques
#include "dts2/deque.h"

int main()
{
    deque(float) a = deque_init_new(0);

    deque_add(&a, 12.67);
    deque_add(&a, 3.789);
    deque_add(&a, 9002);

    float* p1 = deque_eptr(&a, 1);

    deque_add(&a, 5.59);
    deque_add(&a, 171);
    // Remove last element
    deque_rem(&a);

    // Modify value in deque through pointer
    *p1 = 666.69;

    for(size_t i = 0; i < a.size; i++)
    {
        float v = deque_eget(&a, i);
        printf("%.2f ", v);
    }

    deque_free(&a);
    return 0;
}

// This will print:
// 12.67 666.69 9002.00 5.59
```

### Conversion of Data Structures
```C
#include <stdio.h>

#include "dts2/array.h" // Allow usage of arrays
#include "dts2/dynarray.h" // Allow usage of dynarrays
#include "dts2/convert.h" // Allow usage of conversions

int main()
{
    array(char) a = array_init_new(2, char);

    array_eset(&a, 0, 'o');
    array_eset(&a, 1, 'd');

    dynarray(char) b;
    // Convert a to dynarray and store it in b
    dynarray_from_array(&b, &a);
    
    dynarray_addat(&b, 0, 'w');
    dynarray_addat(&b, 2, 'r');
    dynarray_add(&b, '\0');
    
    // Convert b back to array and store it in a
    array_from_dynarray(&a, &b);

    printf("%s\n", a.data);
    array_free(&a);

    return 0;
}

// This will print:
// word
```

# License
[MIT](https://choosealicense.com/licenses/mit/)