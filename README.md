# Main sPrinter control software

This repository contains the control software of the sPrinter rover,
developed by students of the Faculty of Electrical Engineering and Information
Technology STU in Bratislava.

While it was previously developed in ac6 System Workbench, we decided to transfer
to regain control over our toolchain and transfer to non-managed environment.
We currently use `cmake` and compile with `arm-none-eabi-gcc` with `newlib` and `c++17`.

We generate the initialization via `STM CubeMX`, and use `CLion` as our IDE.

Debugging is done via the open source `Open OCD` debugger. It's pretty cool.

Fuck Eclipse dude, seriously.

## Project Structure

- __The root__ contains the `CMakeLists_template.txt`, which is used to generate
the root `CMakeLists` on configuration update. There is also the `.ld` linker script,
and the `.svd` file of the chip for peripheral mapping for the debugger.

- __docs__ contains markdown sources for documentation, which are used to generate HTML via mdbook.

- __startup__ contains a standard generated loader in assembly, with interrupt
vectors and so on.

- __Drivers__ contains the `CMSIS` and the `STM32F4 HAL Driver`, which are used
so we don't have to memory map peripheral's ourselves (miss those good old times).

- __Inc / Src__ contain the generated code which contains some macros and globals,
as well as the initialization code.

- __sprinter__ is where the fun is at. This is our application root, which is linked
via the `sprinter` library to the `CubeMX` core, via the `run()` entry point, and has
additional specific configuration for our c++17 sources.

  - __sprinter__ is the top-most library, which is linked to the rest of the application.
  it privately links every other library into itself, and provides the entry point to
  transfer to cpp land. It also contains the global symbols for the stack protection mechanism.

  - __core__ is the core library, which will eventually contain everything that is cory.

  - There is nothing else at the moment, as I said we are in the process of transfer and rewriting
  all the code from the stdperiph to HAL.

## Building requirements

- `cmake` _>= 3.7_
- `arm-none-eabi-binutils`
- `arm-none-eabi-gcc`
- `arm-none-eabi-newlib`
- `open-ocd`

## Remarks

I guess the sources are pretty useless for you, unless you have a copy
of our rover, which you don't! Just kidding, please fund us.

## License

I don't actually know how does this academic stuff works, and if we have
any license requirements as it was developed as a part of our school project
(as if there was anyone putting in money and time except us). So I would
release it under the __MIT License__, but I don't know if I can, so I won't put any
on it.
