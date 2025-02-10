# ICM42670 Zephyr Driver

This repository contains a Zephyr RTOS driver for the TDK ICM42670 6-axis IMU sensor, with support for Wake-on-Motion (WoM) functionality through the APEX interface.

## Prerequisites

- Zephyr RTOS development environment
- nRF52840 Development Kit
- ICM42670 sensor connected via I2C

## Setup Instructions

1. Initialize west workspace:
```bash
west init -m https://github.com/taimurislamkhan/icm42670-zephyr-driver --mr wom_experimentation
cd icm42670-zephyr-driver
west update
```

2. Build the project:
```bash
west build -b nrf52840dk/nrf52840 --pristine
```

3. Flash to your nRF52840 DK:
```bash
west flash
```

## Features

- Wake-on-Motion (WoM) detection using APEX interface
- Configurable motion thresholds
- Interrupt-driven motion detection
- I2C communication interface

## Hardware Setup

Connect the ICM42670 to your nRF52840 DK:
- VDD -> 3.3V
- GND -> GND
- SCL -> P0.27 (I2C SCL)
- SDA -> P0.26 (I2C SDA)
- INT1 -> P0.23 (Motion interrupt)

## Configuration

The driver can be configured through the following files:
- `prj.conf`: Zephyr configuration options
- `boards/nrf52840dk_nrf52840.overlay`: Device tree overlay for pin configuration
- `src/main.c`: Application code with sensor initialization and WoM setup

## License

Apache License 2.0
