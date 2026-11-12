/* SPDX-License-Identifier: Apache-2.0 */

#include <errno.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>

#include "pdu_adc.h"

#define AD7490_WRITE              BIT(15)
#define AD7490_CHANNEL_SHIFT      10U
#define AD7490_POWER_NORMAL       (3U << 8)
#define AD7490_RANGE_REFIN        BIT(5)
#define AD7490_CODING_BINARY      BIT(4)
#define AD7490_RESULT_MASK        0x0FFFU
#define AD7490_CHANNEL_MASK       0xF000U
#define AD7490_SPI_FREQUENCY_HZ   1000000U

#define AD7490_REFERENCE_MV       2500U
#define NCV84045_SENSE_RATIO_TYP  1230U
#define CURRENT_SENSE_RESISTOR_OHM 301U
#define AD7490_FULL_SCALE_COUNTS  4095U

static const struct device *const spi_dev = DEVICE_DT_GET(DT_NODELABEL(spi1));
static const struct gpio_dt_spec adc_cs =
	GPIO_DT_SPEC_GET(DT_NODELABEL(adc_cs), gpios);
static const struct gpio_dt_spec current_sense_enable =
	GPIO_DT_SPEC_GET(DT_NODELABEL(current_sense_enable), gpios);

static const struct spi_config spi_cfg = {
	.frequency = AD7490_SPI_FREQUENCY_HZ,
	.operation = SPI_OP_MODE_MASTER | SPI_WORD_SET(8) |
		     SPI_TRANSFER_MSB | SPI_MODE_CPOL,
	.slave = 0,
};

static uint16_t ad7490_control_word(uint8_t channel)
{
	return AD7490_WRITE | ((uint16_t)(channel & 0x0FU) << AD7490_CHANNEL_SHIFT) |
	       AD7490_POWER_NORMAL | AD7490_RANGE_REFIN | AD7490_CODING_BINARY;
}

static int ad7490_transfer(uint16_t tx_word, uint16_t *rx_word)
{
	uint8_t tx[2] = { (uint8_t)(tx_word >> 8), (uint8_t)tx_word };
	uint8_t rx[2] = { 0U, 0U };
	const struct spi_buf tx_buf = { .buf = tx, .len = sizeof(tx) };
	const struct spi_buf rx_buf = { .buf = rx, .len = sizeof(rx) };
	const struct spi_buf_set tx_set = { .buffers = &tx_buf, .count = 1 };
	const struct spi_buf_set rx_set = { .buffers = &rx_buf, .count = 1 };
	int ret;

	/* GPIO_ACTIVE_LOW makes logical 1 assert CS and logical 0 release it. */
	ret = gpio_pin_set_dt(&adc_cs, 1);
	if (ret != 0) {
		return ret;
	}

	k_busy_wait(1U);
	ret = spi_transceive(spi_dev, &spi_cfg, &tx_set, &rx_set);
	k_busy_wait(1U);
	(void)gpio_pin_set_dt(&adc_cs, 0);
	k_busy_wait(1U);

	if (ret == 0 && rx_word != NULL) {
		*rx_word = ((uint16_t)rx[0] << 8) | rx[1];
	}

	return ret;
}

static int ad7490_read_channel(uint8_t channel, uint16_t *raw)
{
	uint16_t response;
	uint16_t control = ad7490_control_word(channel);
	int ret;

	/* The first transfer selects the channel. The second returns that result. */
	ret = ad7490_transfer(control, &response);
	if (ret != 0) {
		return ret;
	}

	ret = ad7490_transfer(control, &response);
	if (ret != 0) {
		return ret;
	}

	if (((response & AD7490_CHANNEL_MASK) >> 12) != channel) {
		return -EIO;
	}

	*raw = response & AD7490_RESULT_MASK;
	return 0;
}

int pdu_adc_init(void)
{
	uint16_t ignored;
	int ret;

	if (!device_is_ready(spi_dev) || !gpio_is_ready_dt(&adc_cs) ||
	    !gpio_is_ready_dt(&current_sense_enable)) {
		return -ENODEV;
	}

	ret = gpio_pin_configure_dt(&adc_cs, GPIO_OUTPUT_INACTIVE);
	if (ret != 0) {
		return ret;
	}

	ret = gpio_pin_configure_dt(&current_sense_enable, GPIO_OUTPUT_INACTIVE);
	if (ret != 0) {
		return ret;
	}

	/* Datasheet power-up sequence: two dummy transfers, then normal mode. */
	ret = ad7490_transfer(0xFFFFU, &ignored);
	if (ret == 0) {
		ret = ad7490_transfer(0xFFFFU, &ignored);
	}
	if (ret != 0) {
		return ret;
	}

	ret = gpio_pin_set_dt(&current_sense_enable, 1);
	if (ret != 0) {
		return ret;
	}

	/* Allow the NCV84045 current-sense outputs to settle. */
	k_sleep(K_USEC(100));
	return 0;
}

int pdu_adc_sample(uint16_t raw[PDU_OUTPUT_COUNT])
{
	for (uint8_t channel = 0U; channel < PDU_OUTPUT_COUNT; ++channel) {
		int ret = ad7490_read_channel(channel, &raw[channel]);

		if (ret != 0) {
			return ret;
		}
	}

	return 0;
}

uint32_t pdu_adc_raw_to_ma(uint16_t raw)
{
	uint64_t numerator = (uint64_t)raw * AD7490_REFERENCE_MV *
			     NCV84045_SENSE_RATIO_TYP;
	uint32_t denominator = AD7490_FULL_SCALE_COUNTS *
			       CURRENT_SENSE_RESISTOR_OHM;

	return (uint32_t)((numerator + (denominator / 2U)) / denominator);
}
