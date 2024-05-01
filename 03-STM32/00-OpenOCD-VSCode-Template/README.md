
## Customization Steps

For a different target update the:
- linker script for specific board e.g. `STM32F303RETX_FLASH.ld` 

in `Makefile`:
- `LDSCRIPT` variable (arg for arm-none-eabi-gcc with `-T` flag) 
- OpenOCD target argument i.e. `-f target/stm32f3x.cfg` 
- `ASM_SOURCES` e.g. `startup_stm32f303retx.s`
- `C_DEFS` e.g. `-DSTM32F303xE` for symbol definitions (more useful for HAL)


For new headers/includes update:
- `Makefile`:
    - `C_SOURCES`
    - `C_INCLUDES`


## Overview

- Build process is via the Makefile using the `arm-none-eabi-gcc` version as installed via the STM32 command line tools 
- Firmware upload & Debuggin is done viaOpenOCD over STLink (transport interface SWD by default?)

Commands:
- Build & Flash at same time via VSCode (`CMD+SHIFT+B`) → Makefile (i.e. `make flash`) → OpenOCD 

## Important files / changes for use

GCC path: is hardcoded and not in system environment variables, but is mainly used in 
- `tasks.json` in the `GCC_PATH` environment variable when we call `make`
- for Intelliense in `c_cpp_properties.json` (for the C/C++ Extension from Microsoft)

Other things to edit
- include paths & define's in the `c_cpp_properties.json`, currently not in use but an example can be found in `c_cpp_properties_old.json` (I think this just affects Intellisense and not compilation)


## Other
- FPU disabled see FLOAT-ABI variable