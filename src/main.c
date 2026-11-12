/*
 * Safe compile-only CANopen bring-up for the legacy STM32F7 PDU.
 *
 * All high-side driver inputs and current-sense enable are initialized
 * inactive before CAN is used. The ADC chip select is also initialized
 * inactive (physical high because it is active-low). No output-control object
 * is exposed by this milestone.
 */

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/reboot.h>
#include <canopennode.h>

#include "CO_OD.h"
#include "pdu_telemetry.h"

LOG_MODULE_REGISTER(speeder_pdu, LOG_LEVEL_INF);

#define SAFE_CONTROLS_NODE DT_NODELABEL(safe_controls)

#define SAFE_CONTROL_SPEC(node_id) GPIO_DT_SPEC_GET(node_id, gpios),

static const struct gpio_dt_spec safe_controls[] = {
	DT_FOREACH_CHILD(SAFE_CONTROLS_NODE, SAFE_CONTROL_SPEC)
};

#define CAN_INTERFACE DEVICE_DT_GET(DT_CHOSEN(zephyr_canbus))
#define CAN_BITRATE_KBPS (DT_PROP(DT_CHOSEN(zephyr_canbus), bitrate) / 1000)

static int configure_safe_controls(void)
{
	for (size_t i = 0; i < ARRAY_SIZE(safe_controls); ++i) {
		const struct gpio_dt_spec *control = &safe_controls[i];

		if (!gpio_is_ready_dt(control)) {
			return -ENODEV;
		}

		/* Inactive is physical low for driver enables and physical high for
		 * the active-low ADC chip select.
		 */
		if (gpio_pin_configure_dt(control, GPIO_OUTPUT_INACTIVE) != 0) {
			return -EIO;
		}
	}

	return 0;
}

int main(void)
{
	CO_NMT_reset_cmd_t reset = CO_RESET_NOT;
	struct canopen_context can = { .dev = CAN_INTERFACE };
	int64_t next_swo_heartbeat_ms = 0;

	if (configure_safe_controls() != 0 || !device_is_ready(can.dev)) {
		LOG_ERR("Safe-control or CAN initialization failed");
		return 0;
	}

	pdu_telemetry_init_safe();
	LOG_INF("SPEEDER PDU A001-SWO boot: all %u power outputs inactive",
		PDU_OUTPUT_COUNT);
	LOG_INF("CANopen node %u at %u kbit/s; ADC and SPI disabled",
		CONFIG_CANOPEN_NODE_ID, CAN_BITRATE_KBPS);
	next_swo_heartbeat_ms = k_uptime_get() + 5000;

	while (reset != CO_RESET_APP) {
		uint32_t elapsed_ms = 0U;

		if (CO_init(&can, CONFIG_CANOPEN_NODE_ID, CAN_BITRATE_KBPS) !=
		    CO_ERROR_NO) {
			LOG_ERR("CANopen initialization failed");
			return 0;
		}

		CO_CANsetNormalMode(CO->CANmodule[0]);
		LOG_INF("CANopen is running; outputs remain inactive");

		if (IS_ENABLED(CONFIG_CANOPENNODE_PROGRAM_DOWNLOAD)) {
			canopen_program_download_attach(CO->NMT, CO->SDO[0],
						CO->em);
		}

		while (reset == CO_RESET_NOT) {
			uint16_t timeout_ms = 1U;
			int64_t timestamp = k_uptime_get();

			pdu_telemetry_tick();
			if (IS_ENABLED(CONFIG_PDU_SWO_DEBUG) &&
			    timestamp >= next_swo_heartbeat_ms) {
				LOG_INF("heartbeat uptime=%u s outputs=0x%04x",
					OD_uptime_s, OD_outputStates);
				next_swo_heartbeat_ms = timestamp + 5000;
			}
			reset = CO_process(CO, (uint16_t)elapsed_ms, &timeout_ms);
			if (reset != CO_RESET_NOT) {
				break;
			}

			if (timeout_ms > 0U) {
				k_sleep(K_MSEC(timeout_ms));
				elapsed_ms = (uint32_t)k_uptime_delta(&timestamp);
			} else {
				elapsed_ms = 0U;
			}
		}

		CO_delete(&can);
	}

	if (IS_ENABLED(CONFIG_REBOOT)) {
		sys_reboot(SYS_REBOOT_COLD);
	}

	return 0;
}
