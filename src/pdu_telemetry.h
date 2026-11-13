/* SPDX-License-Identifier: Apache-2.0 */

#ifndef PDU_TELEMETRY_H_
#define PDU_TELEMETRY_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PDU_OUTPUT_COUNT 15U
#define PDU_OUTPUT_MASK  0x7FFFU

#define PDU_TELEMETRY_UNKNOWN_U16 UINT16_MAX
#define PDU_TELEMETRY_UNKNOWN_U32 UINT32_MAX
#define PDU_TELEMETRY_UNKNOWN_I16 INT16_MAX

enum pdu_telemetry_validity {
	PDU_VALID_OUTPUT_STATES = (1UL << 0),
	PDU_VALID_CURRENT_1     = (1UL << 1),
	PDU_VALID_CURRENT_15    = (1UL << 15),
	PDU_VALID_TOTAL_CURRENT = (1UL << 16),
	PDU_VALID_SUPPLY_VOLTAGE = (1UL << 17),
	PDU_VALID_BOARD_TEMPERATURE = (1UL << 18),
	PDU_VALID_MCU_TEMPERATURE = (1UL << 19),
	PDU_VALID_FAULT_MASKS = (1UL << 20),
	PDU_VALID_DIAGNOSTICS = (1UL << 21),
	PDU_VALID_MCU_SUPPLY = (1UL << 22),
};

void pdu_telemetry_init_safe(void);
void pdu_telemetry_tick(void);
void pdu_telemetry_invalidate_currents(void);
void pdu_telemetry_set_supply_voltage(uint16_t millivolts, bool valid);
void pdu_telemetry_set_mcu_temperature(int16_t decicelsius, bool valid);
void pdu_telemetry_set_mcu_supply(uint16_t millivolts, bool valid);
void pdu_telemetry_set_output_states(uint16_t actual, uint16_t commanded);
void pdu_telemetry_set_fault_masks(uint16_t all_faults,
				   uint16_t over_current,
				   uint16_t open_load,
				   uint16_t disabled);
int pdu_telemetry_set_current(size_t channel, uint16_t raw_adc,
			      uint32_t current_mA);

#endif /* PDU_TELEMETRY_H_ */
