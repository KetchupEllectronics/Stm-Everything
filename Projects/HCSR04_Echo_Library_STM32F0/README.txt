HCSR04 Echo distance sensor library for STM32F0 MCUs

Connect TRIG to OUTPUT PIN
Connect ECHO to INPUT PIN with a voltage divider ( 5V -> 3.3V ) ECHOpin ---> 1kOhm --- (INPUT pin) --- 2kOhm ---> GND

In the main function config the HCSR04 settings: 
Example:

 HAL_TIM_Base_Start(&htim1);  // don't forget to start the timer what you choose;

  HCSR04_Config sensor; 
  sensor.Timer = &htim1; 	//Set up the timer what you will use. (configurate in IOC: Clock source: internal) 
  sensor.TRIG_Port = TRIG_GPIO_Port;			 use Timer frequency is 1Mhz or 2Mhz; 
  sensor.TRIGPin = TRIG_Pin;
  sensor.ECHO_Port = ECHO_GPIO_Port;
  sensor.ECHOPin = ECHO_Pin;
  
//Initialize sensor
  HCSR04_Init(&sensor);


uint16_t distance = HCSR04_getDistance(&sensor, 15, 0, 1);

parameters: ( your sensor, samples, unit, freqency in Mhz)
samples: the program makes some measures ( number of samples ) and calculates median
( more you sample, more precisious but more longer); Recommend 5-10; 5-> 50ms+ sound echos; 10 -> 100ms+sound echos

