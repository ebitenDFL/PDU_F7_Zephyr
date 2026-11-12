/* SPDX-License-Identifier: Apache-2.0 */

#include <errno.h>
#include <limits.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <canopennode.h>

#include "CO_OD.h"
#include "pdu_telemetry.h"

#define PDU_ADC_MAX_COUNT 4095U
#define PDU_CURRENT_MAX_MA (UINT16_MAX - 1U)
#define PDU_ALL_CURRENT_VALID_MASK 0x0000FFFEUL

static void update_total_current(void)
{
	uint32_t total = 0U;

	if ((OD_telemetryValidity & PDU_ALL_CURRENT_VALID_MASK) !=
	    PDU_ALL_CURRENT_VALID_MASK) {
		OD_totalCurrent_mA = PDU_TELEMETRY_UNKNOWN_U32;
		OD_telemetryValidity &= ~PDU_VALID_TOTAL_CURRENT;
		return;
	}

	for (size_t i = 0; i < PDU_OUTPUT_COUNT; ++i) {
		total += OD_outputCurrent_mA[i];
	}

	OD_totalCurrent_mA = total;
	OD_telemetryValidity |= PDU_VALID_TOTAL_CURRENT;
}

void pdu_telemetry_init_safe(void)
{
	CO_LOCK_OD();

	OD_outputStates = 0U;
	OD_outputCommanded = 0U;
	OD_outputFaults = 0U;
	OD_overCurrentFaults = 0U;
	OD_openLoadFaults = 0U;
	OD_outputDisabled = 0U;
	OD_totalCurrent_mA = PDU_TELEMETRY_UNKNOWN_U32;
	OD_supplyVoltage_mV = PDU_TELEMETRY_UNKNOWN_U16;
	OD_boardTemperature_dC = PDU_TELEMETRY_UNKNOWN_I16;
	OD_mcuTemperature_dC = PDU_TELEMETRY_UNKNOWN_I16;
	OD_uptime_s = 0U;
	OD_resetReason = PDU_TELEMETRY_UNKNOWN_U16;
	OD_brownoutCount = 0U;
	OD_canBusOffCount = 0U;
	OD_warningFlags = 0U;
	OD_telemetryValidity = PDU_VALID_OUTPUT_STATES;

	for (size_t i = 0; i < PDU_OUTPUT_COUNT; ++i) {
		OD_outputCurrent_mA[i] = PDU_TELEMETRY_UNKNOWN_U16;
		OD_currentSenseRaw[i] = PDU_TELEMETRY_UNKNOWN_U16;
		OD_peakCurrent_mA[i] = PDU_TELEMETRY_UNKNOWN_U16;
		OD_overCurrentTripCount[i] = 0U;
	}

	CO_UNLOCK_OD();
}

void pdu_telemetry_tick(void)
{
	CO_LOCK_OD();
	OD_uptime_s = (uint32_t)(k_uptime_get() / MSEC_PER_SEC);
	CO_UNLOCK_OD();
}

void pdu_telemetry_set_output_states(uint16_t actual, uint16_t commanded)
{
	CO_LOCK_OD();
	OD_outputStates = actual & PDU_OUTPUT_MASK;
	OD_outputCommanded = commanded & PDU_OUTPUT_MASK;
	OD_telemetryValidity |= PDU_VALID_OUTPUT_STATES;
	CO_UNLOCK_OD();
}

void pdu_telemetry_set_fault_masks(uint16_t all_faults,
				   uint16_t over_current,
				   uint16_t open_load,
				   uint16_t disabled)
{
	CO_LOCK_OD();
	OD_outputFaults = all_faults & PDU_OUTPUT_MASK;
	OD_overCurrentFaults = over_current & PDU_OUTPUT_MASK;
	OD_openLoadFaults = open_load & PDU_OUTPUT_MASK;
	OD_outputDisabled = disabled & PDU_OUTPUT_MASK;
	OD_telemetryValidity |= PDU_VALID_FAULT_MASKS;
	CO_UNLOCK_OD();
}

int pdu_telemetry_set_current(size_t channel, uint16_t raw_adc,
			      uint32_t current_mA)
{
	uint16_t measured_mA;
	uint32_t validity_bit;

	if (channel >= PDU_OUTPUT_COUNT || raw_adc > PDU_ADC_MAX_COUNT) {
		return -EINVAL;
	}

	measured_mA = (uint16_t)MIN(current_mA, PDU_CURRENT_MAX_MA);
	validity_bit = 1UL << (channel + 1U);

	CO_LOCK_OD();
	OD_currentSenseRaw[channel] = raw_adc;
	OD_outputCurrent_mA[channel] = measured_mA;
	if (OD_peakCurrent_mA[channel] == PDU_TELEMETRY_UNKNOWN_U16 ||
	    measured_mA > OD_peakCurrent_mA[channel]) {
		OD_peakCurrent_mA[channel] = measured_mA;
	}

	OD_telemetryValidity |= validity_bit;
	update_total_current();
	CO_UNLOCK_OD();

	return 0;
}
