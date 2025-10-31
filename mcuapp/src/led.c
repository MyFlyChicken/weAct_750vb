/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include <string.h>

#define LED0_NODE DT_ALIAS(led0)

#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

// 定义led结构体
struct led {
	struct gpio_dt_spec spec;
	uint8_t num;
};

static const struct led leds[] = {
    {
        .spec = GPIO_DT_SPEC_GET_OR(LED0_NODE, gpios, {0}),
        .num = 0,
    },
};

/**
 * @brief Blink the LED
 * @param [in] led Pointer to the LED structure
 * @param [in] sleep_ms Sleep duration in milliseconds
 *
 * @details This function configures the LED GPIO pin and starts blinking it
 *          with the specified sleep duration.
 */
void blink(const struct led *led, uint32_t sleep_ms)
{
	const struct gpio_dt_spec *spec = &led->spec;
	int cnt = 0;
	int ret;

	if (!device_is_ready(spec->port)) {
		printk("Error: %s device is not ready\n", spec->port->name);
		return;
	}

	ret = gpio_pin_configure_dt(spec, GPIO_OUTPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure pin %d (LED '%d')\n",
			ret, spec->pin, led->num);
		return;
	}

	while (1) {
		gpio_pin_set(spec->port, spec->pin, cnt % 2);
        printk("LED '%d' blink %d\n", led->num, cnt);
		k_msleep(sleep_ms);
		cnt++;
	}
}

void blink_task0(void)
{
    blink(&leds[0], 1000);
}

K_THREAD_DEFINE(blink_task0_id, 512, blink_task0, NULL, NULL, NULL,
                7, 0, 0);
