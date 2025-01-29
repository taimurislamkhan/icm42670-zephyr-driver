.. _icm42670_sample:

ICM42670 6-Axis IMU Sample Application
####################################

Overview
********

This sample application demonstrates how to use the ICM42670 6-axis IMU (Inertial Measurement Unit)
sensor with the Zephyr RTOS. The ICM42670 is a high-performance 6-axis MEMS motion tracking device
that combines a 3-axis gyroscope and a 3-axis accelerometer. The driver supports both SPI and I2C
communication interfaces.

Features
********

* Configurable sample rates for both accelerometer and gyroscope
* Accelerometer ranges: ±2g, ±4g, ±8g, ±16g
* Gyroscope ranges: ±250, ±500, ±1000, ±2000 dps
* Support for both SPI and I2C communication interfaces
* Simple API for sensor initialization and data reading
* Separate functions for reading accelerometer and gyroscope data
* Error handling and status reporting
* Automatic retry mechanism for device initialization

Hardware Setup
*************

The sample is designed to work with the nRF52840 DK and ICM42670 sensor. You can choose between
SPI and I2C interfaces by using the appropriate version from the respective folders.

SPI Version (spi_version/)
-------------------------

Required Connections:
~~~~~~~~~~~~~~~~~~~

* SCK: P0.31
* MOSI: P0.30
* MISO: P0.29
* CS: P0.28
* INT: P0.18 (Interrupt pin)

I2C Version (i2c_version/)
-------------------------

Required Connections:
~~~~~~~~~~~~~~~~~~~

* SDA: P0.26 (Arduino A4)
* SCL: P0.27 (Arduino A5)
* VDD: 3.3V
* GND: GND

Building and Running
*******************

1. Choose the version you want to use (SPI or I2C)
2. Copy all files from the respective version folder to the root directory
3. Build and flash in the usual way:

.. zephyr-app-commands::
   :zephyr-app: samples/sensor/icm42670
   :board: nrf52840dk_nrf52840
   :goals: build flash
   :compact:

Sample Output
************

When the application runs successfully, you should see IMU data being printed to the console:

.. code-block:: console

    Starting ICM42670 application
    ICM42670 initialized successfully
    Accel (m/s^2): X=-0.23, Y=0.15, Z=9.81
    Gyro (dps): X=0.02, Y=-0.01, Z=0.00

API Documentation
***************

The driver provides the following key functions:

* ``icm42670_init()`` - Initialize the sensor (includes retry mechanism)
* ``icm42670_read_all()`` - Read both accelerometer and gyroscope data
* ``icm42670_read_accel()`` - Read only accelerometer data
* ``icm42670_read_gyro()`` - Read only gyroscope data
* ``icm42670_set_accel_rate()`` - Set accelerometer sample rate
* ``icm42670_set_gyro_rate()`` - Set gyroscope sample rate

Version Organization
******************

The repository contains two versions of the driver:

* ``spi_version/`` - Contains the SPI implementation
* ``i2c_version/`` - Contains the I2C implementation

Each version includes its own:
* Source files (src/)
* Board overlay files (boards/)
* Project configuration (prj.conf)

References
**********

* `ICM42670 Product Page <https://invensense.tdk.com/products/motion-tracking/6-axis/icm-42670-p/>`_
* `Zephyr Project <https://www.zephyrproject.org/>`_

Author
******

* Taimur Islam Khan (taimurislamkhan@gmail.com)
* Version: 0.1
* Date: 25-01-2025
