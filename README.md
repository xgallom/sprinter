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

## Pin mapping

| Pin  | Name      | Mode      | Peripheral | Function              |
|------|-----------|-----------|------------|-----------------------|
| PA0  | SRV1_POS  | PWM2      | TIM2_CH1   | Servo 1 position      |
| PA1  | SRV2_POS  | PWM2      | TIM2_CH1   | Servo 2 position      |
| PA8  | USB_SOF   | SOF       | USB_OTG_FS | USB start of signal   |
| PA9  | LOG_TX    | TX        | USART1     | Log serial transmit   |
| PA10 | LOG_RX    | RX        | USART1     | Log serial receive    |
| PA11 | USB_DM    | DM        | USB_OTG_FS | USB data minus        |
| PA12 | USB_DP    | DP        | USB_OTG_FS | USB data plus         |
| PA13 | TMS       | SWDIO     | SYS_JTMS   | JTag test mode select |
| PA14 | TCK       | SWCLK     | SYS_JTCK   | JTag clock            |
| PB0  | LED_GREEN | OUT_PP    | GPIO       | Green LED             |
| PB7  | LED_BLUE  | OUT_PP    | GPIO       | Blue LED              |
| PB10 | STEP1_DIR | OUT_PP    | GPIO       | Stepper 1 direction   |
| PB11 | STEP2_DIR | OUT_PP    | GPIO       | Stepper 2 direction   |
| PB14 | LED_RED   | OUT_PP    | GPIO       | Red LED               |
| PC0  | SUN_ADC1  | IN10      | ADC1       | Suntracker analog in  |
| PC1  | SUN_ADC2  | IN11      | ADC1       | Suntracker analog in  |
| PC2  | SUN_ADC3  | IN12      | ADC1       | Suntracker analog in  |
| PC3  | SUN_ADC4  | IN13      | ADC1       | Suntracker analog in  |
| PC4  | SUN_ADC5  | IN14      | ADC1       | Suntracker analog in  |
| PC6  | ENG4_SPD  | PWM2      | TIM8_CH1   | Engine 4 speed        |
| PC7  | ENG3_SPD  | PWM2      | TIM8_CH2   | Engine 3 speed        |
| PC8  | ENG2_SPD  | PWM2      | TIM8_CH3   | Engine 2 speed        |
| PC9  | ENG1_SPD  | PWM2      | TIM8_CH4   | Engine 1 speed        |
| PC13 | USER_BTN  | IE RT     | GPIO       | User button interrupt |
| PC14 | OSC32_IN  | OSC32_IN  | RCC        | Oscillator input      |
| PC15 | OSC32_OUT | OSC32_OUT | RCC        | Oscillator output     |
| PD0  | STEP1_EN  | OUT_PP    | GPIO       | Stepper 1 enable      |
| PD1  | STEP2_EN  | OUT_PP    | GPIO       | Stepper 2 enable      |
| PD5  | RF_TX     | TX        | USART2     | RF module transmit    |
| PD6  | RF_RX     | RX        | USART2     | RF module receive     |
| PD8  | STLK_TX   | TX        | USART3     | STLink transmit       |
| PD9  | STLK_RX   | RX        | USART3     | STLink receive        |
| PD12 | ENG5_SPD  | PWM2      | TIM4_CH1   | Engine 5 speed        |
| PD13 | ENG6_SPD  | PWM2      | TIM4_CH2   | Engine 6 speed        |
| PD14 | ENG7_SPD  | PWM2      | TIM4_CH3   | Engine 7 speed        |
| PE7  | ENG1_DIR  | OUT_PP    | GPIO       | Engine 1 direction    |
| PE8  | ENG2_DIR  | OUT_PP    | GPIO       | Engine 2 direction    |
| PE9  | ENG3_DIR  | OUT_PP    | GPIO       | Engine 3 direction    |
| PE10 | ENG4_DIR  | OUT_PP    | GPIO       | Engine 4 direction    |
| PE11 | ENG5_DIR  | OUT_PP    | GPIO       | Engine 5 direction    |
| PE12 | ENG6_DIR  | OUT_PP    | GPIO       | Engine 6 direction    |
| PE14 | ENG7_DIR  | OUT_PP    | GPIO       | Engine 7 direction    |
| PG2  | STEP1_CTL | OUT_PP    | GPIO       | Stepper 1 control     |
| PG3  | STEP2_CTL | OUT_PP    | GPIO       | Stepper 2 control     |
| PG6  | USB_PWR   | OUT_PP    | GPIO       | USB power switch on   |
| PG7  | USB_OC    | IN        | GPIO       | USB over current      |
| PH0  | MCO       | OSC_IN    | RCC        | Oscillator input      |
| PH1  | OSC_OUT   | OSC_OUT   | RCC        | Oscillator output     |

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
