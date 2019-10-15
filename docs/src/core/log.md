# Core Library - Logging Functionality

## Headers

- `core/log.h` Contains the 
  - `void log(Args ... args)` function,
  which allows logging any implemented type into the default log USART.

- `core/fatal.h` Contains the unrecoverable
  - `void fatal(Args ... args)` function, 
  which freezes the system along with reporting.

## Extending the Logger

If you want to be able to log a special non-trivial type `T`,
you need to provide a `void logImpl::log(T)` function for it.

> If you want the logging system to know your type, you must include your `logImpl` function declaration __before__
  you include the log header.

## Example Usage

### Log Example
```cpp
#include <core/log.h>

void printFoo(uint32_t x, const char *y)
{
    log("The x is: ", x, ", and the y is: \"", y, "\"\n");
}
```

### Fatal Example
```cpp
#include <core/fatal.h>

void printBar(uint32_t *x)
{
    if(!x)
        fatal("Oh no, nullptr dereference in printBar! - x = ", uintptr_t(x), "\n");

    log("The *x is: ", *x, "\n"); 
}
```

### Extending the Logger Example
```cpp
// Foo.h
struct Foo {
    uint32_t x;
    uint16_t ys[8];
};

namespace logImpl { void log(const Foo &foo); }

// Foo.cpp
#include "Foo.h"
#include <core/log.h>

namespace logImpl {
    void log(const Foo &foo)
    {
        log("Foo { x: ", foo.x, ", ys: [ ");
        for(const auto &y: foo.ys)
            log(y, " ");
        log("] }");
    }
}
```
