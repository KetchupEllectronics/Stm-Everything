#include <HCSR04.h>
#include "main.h"
#include <stdint.h>

void HCSR04_Init(HCSR04_Config *sensor) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	    // 1. TRIG LÁB KONFIGURÁLÁSA (Kimenetnek KELL lennie)
	GPIO_InitStruct.Pin = sensor->TRIGPin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(sensor->TRIG_Port, &GPIO_InitStruct);


	HAL_GPIO_WritePin(sensor->TRIG_Port, sensor->TRIGPin, GPIO_PIN_RESET);

	    // 2. ECHO LÁB KONFIGURÁLÁSA (Bemenetnek KELL lennie)
	GPIO_InitStruct.Pin = sensor->ECHOPin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(sensor->ECHO_Port, &GPIO_InitStruct);

}

static uint32_t compare(const void *a, const void *b)
{
	return *(uint32_t*)a - *(uint32_t*)b;
}

static void delay_us(HCSR04_Config *sensor, uint32_t us, int freq)
{
 	__HAL_TIM_SET_COUNTER(sensor->Timer, 0);
 	while(__HAL_TIM_GET_COUNTER(sensor->Timer) < us * freq);
}

static uint32_t readHCSR04(HCSR04_Config *sensor, int freq)
{
 uint32_t localTime = 0;
 
 	HAL_GPIO_WritePin(sensor -> TRIG_Port, sensor -> TRIGPin, GPIO_PIN_SET);
	delay_us(sensor, 10, freq);
	HAL_GPIO_WritePin(sensor -> TRIG_Port, sensor -> TRIGPin, GPIO_PIN_RESET);
	__HAL_TIM_SET_COUNTER(sensor -> Timer, 0);
	while(HAL_GPIO_ReadPin(sensor -> ECHO_Port, sensor ->  ECHOPin) == 0)
	{
		if(__HAL_TIM_GET_COUNTER(sensor -> Timer) > 30000)
			return 35505;
	}
	__HAL_TIM_SET_COUNTER(sensor -> Timer, 0);
	while(HAL_GPIO_ReadPin(sensor -> ECHO_Port, sensor -> ECHOPin) == 1)
	{
		if(__HAL_TIM_GET_COUNTER(sensor -> Timer) > 65534)
			break;
	}
	localTime = __HAL_TIM_GET_COUNTER(sensor -> Timer);
	return (localTime * 10 )/ 58 * freq;
}

uint32_t HCSR04_getDistance(HCSR04_Config *sensor, int numOfMeasures, int unit, int freq)
{
			uint32_t measurements[numOfMeasures];
			for(int i=0; i<numOfMeasures -1 ; i++)
	  		{
	  			__disable_irq();
	  			measurements[i] = readHCSR04(sensor, freq);
	  			__enable_irq();
	  			HAL_Delay(5);
	  		}
	  		qsort(measurements, numOfMeasures, sizeof(uint32_t), compare);

	  		switch(unit){
	  		case 0:
	  			return measurements[2];
	  		case 1:
	  			return measurements[2] / 10;
	  		case 2:
	  			return (measurements[2]*10)/256;
	  		default: return measurements[2];
	  		}

}
