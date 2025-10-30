/*
 * Copyright (c) 2016 Open-RnD Sp. z o.o.
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * NOTE: If you are looking into an implementation of button events with
 * debouncing, check out `input` subsystem and `samples/subsys/input/input_dump`
 * example instead.
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

#define SLEEP_TIME_MS 5

/*
 * Get button configuration from the devicetree button0 alias. This is mandatory.
 */
#define BUTTON0_NODE DT_ALIAS(button0)
#if !DT_NODE_HAS_STATUS_OKAY(BUTTON0_NODE)
#error "Unsupported board: button0 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(BUTTON0_NODE, gpios, {0});
static struct gpio_callback button_cb_data;

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}

int button_deal(void)
{
	int ret;

	if (!gpio_is_ready_dt(&button)) {
		printk("Error: button device %s is not ready\n", button.port->name);
		return 0;
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n", ret, button.port->name,
		       button.pin);
		return 0;
	}

	ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n", ret,
		       button.port->name, button.pin);
		return 0;
	}

	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);
	printk("Set up button at %s pin %d\n", button.port->name, button.pin);

	printk("Press the button\n");

	while (1) {
		// int val = gpio_pin_get_dt(&button);

		// if (val >= 0) {
		// 	printk("button0 pressed %d", val);
		// }
		k_msleep(SLEEP_TIME_MS);
	}

	return 0;
}

K_THREAD_DEFINE(button_deal_id, 1024, button_deal, NULL, NULL, NULL, 5, 0, 0);
