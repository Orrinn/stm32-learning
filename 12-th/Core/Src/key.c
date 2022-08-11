#include "key.h"
#include "lcd.h"
#include "stdio.h"
#include "parking.h"
#include "tim.h"


KEY_TypeDef B1={GPIOB,GPIO_PIN_0,KEY_UP};
KEY_TypeDef B2={GPIOB,GPIO_PIN_1,KEY_UP};
KEY_TypeDef B3={GPIOB,GPIO_PIN_2,KEY_UP};
KEY_TypeDef B4={GPIOA,GPIO_PIN_0,KEY_UP};
KEY_TypeDef NoButtonDown;

int KEY_PressCheck(KEY_TypeDef* KEY){
	uint16_t isChange=0;
	switch(KEY->STATE){
		case KEY_UP:{
			KEY->STATE=(HAL_GPIO_ReadPin(KEY->GPIOx,KEY->GPIO_PIN)==GPIO_PIN_RESET?KEY_CHECK:KEY_UP);
			break;
		}
		case KEY_CHECK:{
			KEY->STATE=(HAL_GPIO_ReadPin(KEY->GPIOx,KEY->GPIO_PIN)==GPIO_PIN_RESET?KEY_DOWN:KEY_UP);
			isChange=1;
			break;
		}
		case KEY_DOWN:{
			KEY->STATE=(HAL_GPIO_ReadPin(KEY->GPIOx,KEY->GPIO_PIN)==GPIO_PIN_RESET?KEY_DOWN:KEY_UP);
			break;
		}
		default:break;
	}
	
	return isChange;
}

uint16_t currentPage,lastPage;
uint16_t timState;


void KEY_Proc(void){
	if(KEY_PressCheck(&B1)){
		currentPage=(currentPage==DATA_PAGE?PARA_PAGE:DATA_PAGE);
	}
	else if(KEY_PressCheck(&B2)){
		if(currentPage==PARA_PAGE){
			pk.CNBR_Price+=0.5;
			pk.VNBR_Price+=0.5;
		}
	}
	else if(KEY_PressCheck(&B3)){
		if(currentPage==PARA_PAGE){
			pk.CNBR_Price-=0.5;
			pk.VNBR_Price-=0.5;
		}
	}
	else if(KEY_PressCheck(&B4)){
		printf("b4 press\r\n");
		timState=(timState+1)%2;
		if(timState==TIM_OFF) HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_2);
		else{
			printf("TIM ON\r\n");
			HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
		}
	}
}

