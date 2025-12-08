# Pico VL53L5CX with PIO I2C

A modified fork of [akionu/pico-vl53l5cx](https://github.com/akionu/pico-vl53l5cx) that adds **PIO-based I2C support** to the Raspberry Pi Pico VL53L5CX time-of-flight sensor driver.

## Overview

This project is a modified fork of akionu's [Pico VL53L5CX driver](https://github.com/akionu/pico-vl53l5cx). The key modification from the original project is the replacement of hardware I2C with **PIO (Programmable I/O) I2C**, allowing you to use **any two GPIO pin** as SDA/SCL instead of being limited to the Pico's fixed I2C pins (provided that they are consecutive GP pins, although you can bypass this by removing `assert(pin_scl == pin_sda + 1);` in i2c.pio).


### Key Files

- **`api/pio_i2c.c` and `api/pio_i2c.h`** - PIO I2C implementation
- **`api/i2c.pio`** - PIO assembly code for I2C protocol
- **`api/platform.c`** - Platform-specific I2C initialization using PIO
- **`examples`** folder - Contains 11 example programs that test out different features of the VL53L5CX distance sensor. In each example program, you have to specify which GPIO pins are to be used as SCL/SDA pins in the macros section. I've only tested out `ex1_ranging_basic` and `ex2_get_set_params` so far, but hopefully they should all at least detect the VL53L5CX and successfully load the firmware.

### Building
1. I developed this using a Pico 2W, so make sure to modify the following line in "/CMakeLists.txt" (the one in the top level project folder) if you're using a different Pico:
```
set(PICO_BOARD pico2_w)
```

2. Set the following environment variable to your Pico SDK path:
```
export PICO_SDK_PATH=/path/to/pico-sdk
```

3. Clone project
```
git clone https://github.com/tobias-sung/pico-vl53l5cx-pio.git
cd pico-vl53l5cx-pio
```

4. Build
```
mkdir build
cd build

cmake ..
make
```
This builds all libraries and examples. If you just want to build one of the examples, then after `cmake ..` enter:
```
make <example_executable_name>
```
e.g.
```
make ex1_ranging_basic
```

### Loading onto Pico
1. Disconnect the Pico from USB
2. Hold the BOOTSEL button
3. Connect the Pico via USB while holding BOOTSEL. The Pico will appear as a mass storage device (RPI-RP2)

*Using `ex1_ranging_basic.c` as an example*
4. Copy `ex1_ranging_basic.uf2` from `build/ex1_ranging_basic/` onto the RPI-RP2 storage device, or:

```
cp build/examples/ex1_ranging_basic/ex1_ranging_basic.uf2 /media/YOUR_USERNAME/RPI-RP2/
```

5. You can then use a serial program to check the output (every example has set `pico_enable_stdio_usb` to true, so all `printf` statements should be outputted via the Pico's USB connection). I use minicom:
```
minicom -D /dev/ttyACM0 -b 115200
```

### Using in other projects
Just copy the `/api/` folder into your project, then in the top-level `CMakeLists.txt` add `add_subdirectory(“api”)`.

Then add: 
```
target_link_libraries(target_name
    pico_stdlib
    hardware_pio
    vl53l5cx
)
```    
to the `CMakeLists.txt` file in the source code directory you're building.

