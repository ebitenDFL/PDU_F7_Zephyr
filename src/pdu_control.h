/* SPDX-License-Identifier: Apache-2.0 */

#ifndef PDU_CONTROL_H_
#define PDU_CONTROL_H_

#include <stdbool.h>
#include <stdint.h>

int pdu_control_init(void);
void pdu_control_all_off(void);
uint16_t pdu_control_active_outputs(void);
void pdu_control_tick(int64_t now_ms, bool canopen_operational,
		      bool current_measurement_valid);

#endif /* PDU_CONTROL_H_ */
