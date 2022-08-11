#include "key.h"

Button B1={GPIOB,GPIO_PIN_0},B2={GPIOB,GPIO_PIN_1},B3={GPIOB,GPIO_PIN_2},B4={GPIOA,GPIO_PIN_0};
Button* buttons[4]={&B1,&B2,&B3,&B4};

int KEY_PressCheck(Button* B){
	uint16_t retVal=0;
	switch(B->KEY_State){
		case KEY_UP:{
			B->KEY_State=(HAL_GPIO_ReadPin(B->GPIOx,B->GPIO_Pin)==GPIO_PIN_RESET?KEY_CHECK:KEY_UP);
			break;
		}
		case KEY_CHECK:{
			B->KEY_State=(HAL_GPIO_ReadPin(B->GPIOx,B->GPIO_Pin)==GPIO_PIN_RESET?KEY_DOWN:KEY_UP);
			retVal=1;
			break;
		}
		case KEY_DOWN:{
			B->KEY_State=(HAL_GPIO_ReadPin(B->GPIOx,B->GPIO_Pin)==GPIO_PIN_RESET?KEY_DOWN:KEY_UP);
			break;
		}
	}
	return retVal;
}

int KEY_Scan(void){
	for(int i=0;i<4;i++)	
		if(KEY_PressCheck(buttons[i])) return (i+1);
	return 0;
}

void KEY_Process(void){
	uint16_t key=KEY_Scan();
	switch(key){
		case 1:{
			nowPage=((strcmp(nowPage,DATA_PAGE))?DATA_PAGE:PARA_PAGE);
			break;
		}
		case 2:{
			if(!strcmp(nowPage,PARA_PAGE)) PA6duty=(PA6duty==90?10:PA6duty+10);
			break;
		}
		case 3:{
			if(!strcmp(nowPage,PARA_PAGE)) PA7duty=(PA7duty==90?10:PA7duty+10);
			else{
				uint16_t tem=0;
				tem=PA6_PERIOD;
				PA6_PERIOD=PA7_PERIOD;
				PA7_PERIOD=tem;
			}
			break;
		}
		case 4:{
			nowMode=((strcmp(nowMode,MODE_AUTO))?MODE_AUTO:MODE_MANU);
			break;
		}
		
	}
	return;
}