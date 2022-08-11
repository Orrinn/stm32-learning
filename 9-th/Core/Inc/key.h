#ifndef __KEY_H__
#define __KEY_H__

#include "main.h"

#define KEY_UP 0
#define KEY_CHECK 1
#define KEY_PRESS_SHORT 2
#define KEY_PRESS_LONG 3

#define Standby "Standby"
#define Setting "Setting"
#define Running "Running"
#define Pause   "Pause  "

typedef struct{
	uint16_t hour;
	uint16_t minute;
	uint16_t second;
}Time_TypeDef;


typedef struct{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	uint16_t KEY_State;
	uint32_t KEY_pressTick;
}Button;

extern Time_TypeDef nowTime,temTime;
extern uint8_t b4_count;

void KEY_Scan(void);

#endif