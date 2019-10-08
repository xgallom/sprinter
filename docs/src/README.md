# Main sPrinter Control Software 

This document provides information about the control software of the sPrinter rover,
developed by students of the Faculty of Electrical Engineering and Information
Technology STU in Bratislava.

While it was previously developed in ac6 System Workbench, we decided to transfer
to regain control over our toolchain and transfer to non-managed environment.
We currently use `cmake` and compile with `arm-none-eabi-gcc` with `newlib` and `c++17`.

We generate the initialization via `STM CubeMX`, and use `CLion` as our IDE.

Debugging is done via the open source `Open OCD` debugger. It's pretty cool.

Fuck Eclipse dude, seriously.
