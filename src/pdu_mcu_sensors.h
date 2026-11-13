/* SPDX-License-Identifier: Apache-2.0 */

#ifndef PDU_MCU_SENSORS_H_
#define PDU_MCU_SENSORS_H_

#include <stdint.h>

int pdu_mcu_sensors_init(void);
int pdu_mcu_sensors_sample(int16_t *temperature_dC, uint16_t *vdda_mV);

#endif /* PDU_MCU_SENSORS_H_ */
