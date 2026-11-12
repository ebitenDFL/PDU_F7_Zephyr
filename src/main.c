/*
 * Functional CANopen firmware for the VIIALA PDU.
 *
 * Outputs always initialize OFF. They can only turn on while CANopen is
 * Operational, current telemetry is valid, and no latched fault is present.
 */

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/reboot.h>
#include <canopennode.h>

#include "CO_OD.h"
#include "pdu_adc.h"
#include "pdu_control.h"
#include "pdu_telemetry.h"

LOG_MODULE_REGISTER(viiala_pdu, LOG_LEVEL_INF);

#define CAN_INTERFACE DEVICE_DT_GET(DT_CHOSEN(zephyr_canbus))
#define CAN_BITRATE_KBPS_DEFAULT \
	(DT_PROP(DT_CHOSEN(zephyr_canbus), bitrate) / 1000)
#define ADC_SAMPLE_PERIOD_MS 5U
#define COMM_APPLY_MAGIC 0xA5U
#define COMM_APPLY_REJECTED 0xFFU

static bool bitrate_supported(uint16_t bitrate_kbps)
{
	return bitrate_kbps == 125U || bitrate_kbps == 250U ||
	       bitrate_kbps == 500U || bitrate_kbps == 1000U;
}

int main(void)
{
	struct canopen_context can = { .dev = CAN_INTERFACE };
	CO_NMT_reset_cmd_t reset = CO_RESET_NOT;
	uint8_t node_id = CONFIG_CANOPEN_NODE_ID;
	uint16_t bitrate_kbps = CAN_BITRATE_KBPS_DEFAULT;
	bool adc_ready;
	bool currents_valid = false;
	uint16_t raw[PDU_OUTPUT_COUNT];
	int64_t next_adc_ms = 0;
	int64_t next_swo_heartbeat_ms = 0;

	if (pdu_control_init() != 0 || !device_is_ready(can.dev)) {
		LOG_ERR("Output GPIO or CAN initialization failed");
		return 0;
	}

	pdu_telemetry_init_safe();
	adc_ready = pdu_adc_init() == 0;
	if (!adc_ready) {
		LOG_ERR("AD7490 initialization failed; outputs locked OFF");
	}

	OD_pendingNodeID = node_id;
	OD_pendingBitrate_kbps = bitrate_kbps;
	LOG_INF("VIIALA_PDU1_V0 boot: all %u outputs OFF", PDU_OUTPUT_COUNT);
	next_swo_heartbeat_ms = k_uptime_get() + 5000;

	do {
		reset = CO_RESET_NOT;
		pdu_control_all_off();

		if (CO_init(&can, node_id, bitrate_kbps) != CO_ERROR_NO) {
			LOG_ERR("CANopen initialization failed");
			return 0;
		}

		CO_CANsetNormalMode(CO->CANmodule[0]);
		LOG_INF("CANopen node %u at %u kbit/s", node_id, bitrate_kbps);

		if (IS_ENABLED(CONFIG_CANOPENNODE_PROGRAM_DOWNLOAD)) {
			canopen_program_download_attach(CO->NMT, CO->SDO[0], CO->em);
		}

		next_adc_ms = k_uptime_get();
		while (reset == CO_RESET_NOT) {
			uint16_t timeout_ms = 1U;
			int64_t timestamp = k_uptime_get();
			bool operational =
				CO->NMT->operatingState == CO_NMT_OPERATIONAL;

			if (adc_ready && timestamp >= next_adc_ms) {
				uint16_t active = pdu_control_active_outputs();

				if (pdu_adc_sample(raw, active) == 0) {

					currents_valid = true;
					for (size_t i = 0; i < PDU_OUTPUT_COUNT; ++i) {
						uint32_t current_ma =
							(active & BIT(i)) != 0U ?
							pdu_adc_raw_to_ma(raw[i]) : 0U;

						(void)pdu_telemetry_set_current(
							i, raw[i], current_ma);
					}
				} else {
					currents_valid = false;
					pdu_telemetry_invalidate_currents();
				}
				next_adc_ms = timestamp + ADC_SAMPLE_PERIOD_MS;
			}

			pdu_control_tick(timestamp, operational, currents_valid);
			pdu_telemetry_tick();

			if (OD_commApply == COMM_APPLY_MAGIC) {
				if (OD_pendingNodeID >= 1U && OD_pendingNodeID <= 127U &&
				    bitrate_supported(OD_pendingBitrate_kbps)) {
					OD_outputRequest = 0U;
					pdu_control_all_off();
					node_id = OD_pendingNodeID;
					bitrate_kbps = OD_pendingBitrate_kbps;
					OD_commApply = 0U;
					reset = CO_RESET_COMM;
				} else {
					OD_commApply = COMM_APPLY_REJECTED;
				}
			}

			if (IS_ENABLED(CONFIG_PDU_SWO_DEBUG) &&
			    timestamp >= next_swo_heartbeat_ms) {
				LOG_INF("uptime=%u outputs=0x%04x current=%u mA",
					OD_uptime_s, OD_outputStates,
					OD_totalCurrent_mA);
				next_swo_heartbeat_ms = timestamp + 5000;
			}

			if (reset == CO_RESET_NOT) {
				reset = CO_process(CO, 1U, &timeout_ms);
			}
			if (reset == CO_RESET_NOT && timeout_ms > 0U) {
				k_sleep(K_MSEC(MIN(timeout_ms, 1U)));
			}
		}

		pdu_control_all_off();
		CO_delete(&can);
	} while (reset != CO_RESET_APP);

	if (IS_ENABLED(CONFIG_REBOOT)) {
		sys_reboot(SYS_REBOOT_COLD);
	}

	return 0;
}
