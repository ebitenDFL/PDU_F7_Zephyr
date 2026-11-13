/* SPDX-License-Identifier: Apache-2.0 */

#include <errno.h>
#include <limits.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>

#include <stm32_ll_adc.h>
#include <stm32_ll_bus.h>

#include "pdu_mcu_sensors.h"

#define ADC_CONVERSION_TIMEOUT_US 1000U
#define ADC_AVERAGE_SAMPLES          8U

static int read_channel(uint32_t channel, uint16_t *sample)
{
	uint32_t timeout = ADC_CONVERSION_TIMEOUT_US;

	LL_ADC_SetChannelSamplingTime(ADC1, channel, LL_ADC_SAMPLINGTIME_480CYCLES);
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, channel);
	LL_ADC_ClearFlag_EOCS(ADC1);
	LL_ADC_REG_StartConversionSWStart(ADC1);

	while (!LL_ADC_IsActiveFlag_EOCS(ADC1)) {
		if (timeout-- == 0U) {
			return -ETIMEDOUT;
		}
		k_busy_wait(1U);
	}

	*sample = LL_ADC_REG_ReadConversionData12(ADC1);
	return 0;
}

static int read_average(uint32_t channel, uint16_t *average)
{
	uint32_t sum = 0U;
	uint16_t sample;
	int ret;

	for (uint8_t i = 0U; i < ADC_AVERAGE_SAMPLES; ++i) {
		ret = read_channel(channel, &sample);
		if (ret != 0) {
			return ret;
		}
		sum += sample;
	}

	*average = (uint16_t)((sum + ADC_AVERAGE_SAMPLES / 2U) /
				     ADC_AVERAGE_SAMPLES);
	return 0;
}

int pdu_mcu_sensors_init(void)
{
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
	LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC1),
			      LL_ADC_CLOCK_SYNC_PCLK_DIV4);
	LL_ADC_SetResolution(ADC1, LL_ADC_RESOLUTION_12B);
	LL_ADC_SetDataAlignment(ADC1, LL_ADC_DATA_ALIGN_RIGHT);
	LL_ADC_REG_SetSequencerLength(ADC1, LL_ADC_REG_SEQ_SCAN_DISABLE);
	LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);
	LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);
	LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1),
				       LL_ADC_PATH_INTERNAL_VREFINT |
				       LL_ADC_PATH_INTERNAL_TEMPSENSOR);
	LL_ADC_Enable(ADC1);
	k_busy_wait(20U);
	return 0;
}

int pdu_mcu_sensors_sample(int16_t *temperature_dC, uint16_t *vdda_mV)
{
	uint16_t vref_raw;
	uint16_t temp_raw;
	int32_t temp_at_cal_vdda;
	int32_t denominator;
	int32_t temperature;
	uint32_t vdda;
	int ret;

	if (temperature_dC == NULL || vdda_mV == NULL) {
		return -EINVAL;
	}

	ret = read_average(LL_ADC_CHANNEL_VREFINT, &vref_raw);
	if (ret != 0 || vref_raw == 0U) {
		return ret != 0 ? ret : -ENODATA;
	}
	ret = read_average(LL_ADC_CHANNEL_TEMPSENSOR, &temp_raw);
	if (ret != 0) {
		return ret;
	}

	vdda = ((uint32_t)*VREFINT_CAL_ADDR * VREFINT_CAL_VREF + vref_raw / 2U) /
	       vref_raw;
	temp_at_cal_vdda = ((int32_t)temp_raw * (int32_t)vdda +
			     (TEMPSENSOR_CAL_VREFANALOG / 2)) /
			    TEMPSENSOR_CAL_VREFANALOG;
	denominator = (int32_t)*TEMPSENSOR_CAL2_ADDR -
		      (int32_t)*TEMPSENSOR_CAL1_ADDR;
	if (denominator == 0) {
		return -ENODATA;
	}
	temperature = TEMPSENSOR_CAL1_TEMP * 10 +
		      ((temp_at_cal_vdda - (int32_t)*TEMPSENSOR_CAL1_ADDR) *
		       (TEMPSENSOR_CAL2_TEMP - TEMPSENSOR_CAL1_TEMP) * 10) /
		      denominator;

	*temperature_dC = (int16_t)CLAMP(temperature, INT16_MIN, INT16_MAX);
	*vdda_mV = (uint16_t)MIN(vdda, UINT16_MAX);
	return 0;
}
