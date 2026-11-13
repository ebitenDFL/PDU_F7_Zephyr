/* SPDX-License-Identifier: Apache-2.0 */

#ifndef PDU_ADC_H_
#define PDU_ADC_H_

#include <stdint.h>

#include "pdu_telemetry.h"

int pdu_adc_init(void);
int pdu_adc_sample(uint16_t raw[PDU_OUTPUT_COUNT], uint16_t active_outputs);
int pdu_adc_read_input_voltage(uint16_t *raw, uint16_t *millivolts);
uint32_t pdu_adc_raw_to_ma(uint16_t raw);

#endif /* PDU_ADC_H_ */
