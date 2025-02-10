#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/sensor/icm42x70.h>
#include <zephyr/drivers/sensor/tdk_apex.h>
#include <zephyr/logging/log.h>
#include "imu/inv_imu_apex.h"

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

/* Sensor device name */
#define SENSOR_NODE DT_NODELABEL(icm42670)

/* Thread stack size */
#define STACK_SIZE 1024
#define THREAD_PRIORITY 5

/* Thread for sensor reading */
static struct k_thread sensor_thread;
K_THREAD_STACK_DEFINE(sensor_stack, STACK_SIZE);

/* Callback function for motion trigger */
static void motion_trigger_handler(const struct device *dev,
				 const struct sensor_trigger *trig)
{
	int rc;
	struct sensor_value accel[3], gyro[3];

	/* Get accelerometer data */
	rc = sensor_sample_fetch_chan(dev, SENSOR_CHAN_ACCEL_XYZ);
	if (rc != 0) {
		LOG_ERR("Failed to fetch accelerometer data: %d", rc);
		return;
	}

	rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_X, &accel[0]);
	rc |= sensor_channel_get(dev, SENSOR_CHAN_ACCEL_Y, &accel[1]);
	rc |= sensor_channel_get(dev, SENSOR_CHAN_ACCEL_Z, &accel[2]);
	if (rc != 0) {
		LOG_ERR("Failed to get accelerometer values: %d", rc);
		return;
	}

	/* Get gyroscope data */
	rc = sensor_sample_fetch_chan(dev, SENSOR_CHAN_GYRO_XYZ);
	if (rc != 0) {
		LOG_ERR("Failed to fetch gyroscope data: %d", rc);
		return;
	}

	rc = sensor_channel_get(dev, SENSOR_CHAN_GYRO_X, &gyro[0]);
	rc |= sensor_channel_get(dev, SENSOR_CHAN_GYRO_Y, &gyro[1]);
	rc |= sensor_channel_get(dev, SENSOR_CHAN_GYRO_Z, &gyro[2]);
	if (rc != 0) {
		LOG_ERR("Failed to get gyroscope values: %d", rc);
		return;
	}

	/* Print sensor values */
	LOG_INF("Motion detected!");
	LOG_INF("Accel (m/s^2) X: %d.%06d, Y: %d.%06d, Z: %d.%06d",
		accel[0].val1, accel[0].val2,
		accel[1].val1, accel[1].val2,
		accel[2].val1, accel[2].val2);
	LOG_INF("Gyro (dps) X: %d.%06d, Y: %d.%06d, Z: %d.%06d",
		gyro[0].val1, gyro[0].val2,
		gyro[1].val1, gyro[1].val2,
		gyro[2].val1, gyro[2].val2);
}

void main(void)
{
	const struct device *const dev = DEVICE_DT_GET(SENSOR_NODE);
	struct sensor_trigger trigger = {
		.type = SENSOR_TRIG_DATA_READY,
		.chan = SENSOR_CHAN_APEX_MOTION,
	};
	int rc;

	/* Check if device is ready */
	if (!device_is_ready(dev)) {
		LOG_ERR("%s: device not ready.", dev->name);
		return;
	}

	LOG_INF("Device %s is ready", dev->name);

	/* Configure accelerometer */
	struct sensor_value accel_config[] = {
		{
			.val1 = 16,    /* Full scale range: ±16g */
			.val2 = 0
		},
		{
			.val1 = 50,    /* Output data rate: 50Hz */
			.val2 = 0
		}
	};

	rc = sensor_attr_set(dev, SENSOR_CHAN_ACCEL_XYZ,
			    SENSOR_ATTR_FULL_SCALE,
			    &accel_config[0]);
	if (rc != 0) {
		LOG_ERR("Failed to set accelerometer full scale: %d", rc);
		return;
	}

	rc = sensor_attr_set(dev, SENSOR_CHAN_ACCEL_XYZ,
			    SENSOR_ATTR_SAMPLING_FREQUENCY,
			    &accel_config[1]);
	if (rc != 0) {
		LOG_ERR("Failed to set accelerometer sampling frequency: %d", rc);
		return;
	}

	/* Configure wake-on-motion using APEX */
	struct sensor_value wom_config = {
		.val1 = TDK_APEX_WOM,  /* Enable Wake-on-Motion */
		.val2 = 0
	};

	rc = sensor_attr_set(dev, SENSOR_CHAN_APEX_MOTION,
			    SENSOR_ATTR_CONFIGURATION,
			    &wom_config);
	if (rc != 0) {
		LOG_ERR("Failed to configure WoM: %d", rc);
		return;
	}

	/* Wait for WoM to initialize */
	k_sleep(K_MSEC(100));

	/* Configure motion trigger handler */
	rc = sensor_trigger_set(dev, &trigger, motion_trigger_handler);
	if (rc != 0) {
		LOG_ERR("Failed to set trigger: %d", rc);
		return;
	}

	LOG_INF("Wake-on-Motion trigger configured successfully");
	LOG_INF("Move the device to trigger motion detection...");

	/* Sleep forever, waiting for motion triggers */
	while (1) {
		k_sleep(K_MSEC(1000));
	}
}
