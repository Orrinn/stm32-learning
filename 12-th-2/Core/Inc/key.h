#ifndef __KEY_H__
#define __KEY_H__

#include "main.h"

#define KEY_UP 0
#define KEY_CHECK 1
#define KEY_DOWN 2

typedef struct{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	uint16_t KEY_State;
}Button;


void KEY_Process(void);

#endif



