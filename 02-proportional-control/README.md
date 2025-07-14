# Proportional Control Firmware

This stage introduces a simple proportional controller implemented on an
STM32F446 microcontroller.

## Building with CMake
1. Install the ARM `gcc-arm-none-eabi` toolchain.
2. Configure and build using the provided presets:
   ```bash
   cmake --preset Debug
   cmake --build build/Debug
   ```
   The final binary `02-proportional-control.elf` will appear in
   `build/Debug`.

## Building with STM32CubeIDE
1. Open `02-proportional-control.ioc` in STM32CubeIDE.
2. Generate the project when prompted.
3. Use the IDE's **Build** button to compile and flash the firmware.
