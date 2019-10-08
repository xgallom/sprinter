# Core Library - Logging Functionality

## Headers

- `core/log.h` contains logging utilities

- `core/fatal.h` contains the unrecoverable error function, which
  freezes the system along with reporting information 

## Example Usage

### Log usage
```cpp
#include <core/log.h>

void printFoo(uint32_t x, const char *y)
{
    log("The x is: ", x, ", and the y is: \"", y, "\"\n");
}
```

### Fatal usage
```cpp
#include <core/fatal.h>

void printBar(uint32_t *x)
{
    if(!x)
        fatal("Oh no, nullptr dereference in printBar! - x = ", uintptr_t(x), "\n");

    log("The *x is: ", *x, "\n"); 
}
```

## Extending the logger

If you want to be able to log a special non-trivial type `T`,
you need to provide a `void logImpl::log(T)` function for it.

> If you want the logging system to know your type, you must include your `logImpl` function declaration __before__
  you include the log header.

