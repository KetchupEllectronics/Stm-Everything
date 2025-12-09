#ifndef HCSR04_H
#define HCSR04_H

#include "stm32f0xx_hal.h"  // FONTOS: Ez kell a GPIO és Timer típusokhoz!
#include <stdint.h>

typedef struct{
	GPIO_TypeDef* TRIG_Port;
	uint16_t TRIGPin;
	GPIO_TypeDef* ECHO_Port;
	uint16_t ECHOPin;
	TIM_HandleTypeDef* Timer;
} HCSR04_Config;

void HCSR04_Init(HCSR04_Config *sensor);
uint32_t HCSR04_getDistance(HCSR04_Config *sensor, int numOfMeasures, int unit, int freq);
#endif
