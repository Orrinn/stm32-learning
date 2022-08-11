#ifndef __KEY_H__
#define __KEY_H__

#include "main.h"

#define TIM_OFF 0
#define TIM_ON 1

#define KEY_UP 0
#define KEY_CHECK 1
#define KEY_DOWN 2

typedef struct{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_PIN;
	uint16_t STATE;
}KEY_TypeDef;

extern KEY_TypeDef B1,B2,B3,B4;
extern uint16_t currentPage,lastPage;
extern uint16_t timState;

int KEY_PressCheck(KEY_TypeDef* KEY);
void KEY_Proc(void);

#endif