#include <zephyr/kernel.h>
#include <stdio.h>
#include "icm42670.h"

void main(void)
{
    struct icm42670_data sensor_data;
    int ret;

    printk("Starting ICM42670 application\n");

    /* Initialize the sensor */
    ret = icm42670_init();
    if (ret != 0) {
        printk("Failed to initialize ICM42670\n");
        return;
    }

    /* Set initial sample rates */
    ret = icm42670_set_accel_rate(100); /* 100 Hz */
    if (ret != 0) {
        return;
    }

    ret = icm42670_set_gyro_rate(100); /* 100 Hz */
    if (ret != 0) {
        return;
    }

    while (1) {
        /* Read both accelerometer and gyroscope data */
        ret = icm42670_read_all(&sensor_data);
        if (ret == 0) {
            printf("Accel (m/s^2): X=%.2f, Y=%.2f, Z=%.2f\n",
                   sensor_data.accel_x,
                   sensor_data.accel_y,
                   sensor_data.accel_z);

            printf("Gyro (dps): X=%.2f, Y=%.2f, Z=%.2f\n",
                   sensor_data.gyro_x,
                   sensor_data.gyro_y,
                   sensor_data.gyro_z);
        }

        /* Wait for 1 second before next reading */
        k_sleep(K_MSEC(1000));
    }
}
