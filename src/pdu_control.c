/* SPDX-License-Identifier: Apache-2.0 */

#include <errno.h>

#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <canopennode.h>

#include "CO_OD.h"
#include "pdu_control.h"
#include "pdu_telemetry.h"

#define POWER_OUTPUTS_NODE DT_NODELABEL(power_outputs)
#define OUTPUT_SPEC(node_id) GPIO_DT_SPEC_GET(node_id, gpios),

#define PDU_LIMIT_MIN_MA          100U
#define PDU_LIMIT_MAX_MA        10000U
#define PDU_TRIP_TIME_MIN_MS        1U
#define PDU_TRIP_TIME_MAX_MS     5000U
#define PDU_INRUSH_TIME_MAX_MS  10000U

static const struct gpio_dt_spec outputs[] = {
	DT_FOREACH_CHILD(POWER_OUTPUTS_NODE, OUTPUT_SPEC)
};

static int64_t enabled_at_ms[PDU_OUTPUT_COUNT];
static int64_t over_limit_at_ms[PDU_OUTPUT_COUNT];
static uint16_t actual_outputs;

static uint16_t clamp_u16(uint16_t value, uint16_t minimum, uint16_t maximum)
{
	return MIN(MAX(value, minimum), maximum);
}

int pdu_control_init(void)
{
	BUILD_ASSERT(ARRAY_SIZE(outputs) == PDU_OUTPUT_COUNT,
		     "Power-output GPIO count must match PDU_OUTPUT_COUNT");

	for (size_t i = 0; i < ARRAY_SIZE(outputs); ++i) {
		if (!gpio_is_ready_dt(&outputs[i])) {
			return -ENODEV;
		}
		if (gpio_pin_configure_dt(&outputs[i], GPIO_OUTPUT_INACTIVE) != 0) {
			return -EIO;
		}
		enabled_at_ms[i] = 0;
		over_limit_at_ms[i] = 0;
	}

	actual_outputs = 0U;
	return 0;
}

void pdu_control_all_off(void)
{
	for (size_t i = 0; i < ARRAY_SIZE(outputs); ++i) {
		(void)gpio_pin_set_dt(&outputs[i], 0);
		enabled_at_ms[i] = 0;
		over_limit_at_ms[i] = 0;
	}
	actual_outputs = 0U;
	pdu_telemetry_set_output_states(0U, OD_outputRequest & PDU_OUTPUT_MASK);
}

uint16_t pdu_control_active_outputs(void)
{
	return actual_outputs;
}

void pdu_control_tick(int64_t now_ms, bool canopen_operational,
		      bool current_measurement_valid)
{
	uint16_t requested;
	uint16_t clear_request;
	uint16_t faults;

	CO_LOCK_OD();
	requested = OD_outputRequest & PDU_OUTPUT_MASK;
	/* Never retain an armed request across a stopped bus or invalid ADC scan. */
	if (!canopen_operational || !current_measurement_valid) {
		requested = 0U;
		OD_outputRequest = 0U;
	}
	clear_request = OD_faultClearRequest & PDU_OUTPUT_MASK;
	OD_faultClearRequest = 0U;
	faults = OD_overCurrentFaults & PDU_OUTPUT_MASK;

	for (size_t i = 0; i < PDU_OUTPUT_COUNT; ++i) {
		OD_currentLimit_mA[i] = clamp_u16(OD_currentLimit_mA[i],
			PDU_LIMIT_MIN_MA, PDU_LIMIT_MAX_MA);
		OD_currentTripTime_ms[i] = clamp_u16(OD_currentTripTime_ms[i],
			PDU_TRIP_TIME_MIN_MS, PDU_TRIP_TIME_MAX_MS);
		OD_inrushLimit_mA[i] = clamp_u16(OD_inrushLimit_mA[i],
			OD_currentLimit_mA[i], PDU_LIMIT_MAX_MA);
		OD_inrushTime_ms[i] = MIN(OD_inrushTime_ms[i],
			PDU_INRUSH_TIME_MAX_MS);
	}
	CO_UNLOCK_OD();

	/* A latched channel may only be cleared after its command is OFF. */
	faults &= ~(clear_request & ~requested);

	for (size_t i = 0; i < PDU_OUTPUT_COUNT; ++i) {
		uint16_t bit = BIT(i);
		bool desired = canopen_operational && current_measurement_valid &&
			       ((requested & bit) != 0U) && ((faults & bit) == 0U);
		bool just_enabled = desired && (actual_outputs & bit) == 0U;

		if (just_enabled) {
			enabled_at_ms[i] = now_ms;
			over_limit_at_ms[i] = 0;
		}

		/* The first current value still belongs to the OFF-state diagnostic. */
		if (desired && !just_enabled) {
			uint32_t age_ms = (uint32_t)(now_ms - enabled_at_ms[i]);
			bool inrush = age_ms < OD_inrushTime_ms[i];
			uint16_t threshold = inrush ? OD_inrushLimit_mA[i] :
						     OD_currentLimit_mA[i];
			uint16_t current = OD_outputCurrent_mA[i];

			if (current > threshold) {
				if (inrush || over_limit_at_ms[i] == 0) {
					over_limit_at_ms[i] = now_ms;
				}
				if (inrush ||
				    (uint32_t)(now_ms - over_limit_at_ms[i]) >=
					OD_currentTripTime_ms[i]) {
					faults |= bit;
					OD_overCurrentTripCount[i]++;
					desired = false;
				}
			} else {
				over_limit_at_ms[i] = 0;
			}
		}

		(void)gpio_pin_set_dt(&outputs[i], desired ? 1 : 0);
		if (desired) {
			actual_outputs |= bit;
		} else {
			actual_outputs &= ~bit;
			enabled_at_ms[i] = 0;
			over_limit_at_ms[i] = 0;
		}
	}

	pdu_telemetry_set_output_states(actual_outputs, requested);
	pdu_telemetry_set_fault_masks(faults, faults, 0U, 0U);
}
