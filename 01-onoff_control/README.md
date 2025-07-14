# On/Off Control Firmware

This folder contains the simplest firmware example using a bang-bang
controller for the LED brightness demo.

## Building with CMake
1. Install the ARM `gcc-arm-none-eabi` toolchain and ensure
   `arm-none-eabi-gcc` is in your `PATH`.
2. Configure and build using the presets:
   ```bash
   cmake --preset Debug
   cmake --build build/Debug
   ```
   The resulting ELF binary can be flashed to the board from the
   `build/Debug` directory.

## Building with STM32CubeIDE
1. Open `01-onoff_control.ioc` in STM32CubeIDE.
2. Let the IDE generate the project files if prompted.
3. Click **Build** to compile and **Run** to flash the board.
