#include "key.h"
#include "stdio.h"
#include "tim.h"
#include "i2c_hal.h"

Button B1={GPIOB,GPIO_PIN_0},B2={GPIOB,GPIO_PIN_1},B3={GPIOB,GPIO_PIN_2},B4={GPIOA,GPIO_PIN_0};
Button* buttons[4]={&B1,&B2,&B3,&B4};
Time_TypeDef nowTime;

//retVal is 1:short press ,2:long press ,0:no press
uint16_t KEY_PressCheck(Button* B){
	uint16_t retVal=0;
	switch(B->KEY_State){
		case KEY_UP:{
			B->KEY_State=(HAL_GPIO_ReadPin(B->GPIOx,B->GPIO_Pin)==GPIO_PIN_RESET?KEY_CHECK:KEY_UP);
			break;
		}
		case KEY_CHECK:{
			if(B!=&B1){
				if(!B->KEY_pressTick) B->KEY_pressTick=HAL_GetTick();
				if(HAL_GPIO_ReadPin(B->GPIOx,B->GPIO_Pin)==GPIO_PIN_RESET){
					if(HAL_GetTick()-B->KEY_pressTick>800){
						B->KEY_State=KEY_PRESS_LONG;
						retVal=2;
					}
					else B->KEY_State=KEY_CHECK;
				}
				else{
					if(HAL_GetTick()-B->KEY_pressTick>2){
						B->KEY_State=KEY_PRESS_SHORT;
						retVal=1;
					}
					else B->KEY_State=KEY_UP;
				}
				
				if(B->KEY_State!=KEY_CHECK) B->KEY_pressTick=0;
			}
			else{
				B->KEY_State=(HAL_GPIO_ReadPin(B->GPIOx,B->GPIO_Pin)==GPIO_PIN_RESET?KEY_PRESS_SHORT:KEY_UP);
				if(B->KEY_State==KEY_PRESS_SHORT) retVal=1;
			}
			break;
		}
		case KEY_PRESS_SHORT:{
			if(B!=&B1) B->KEY_State=KEY_UP;
			else B->KEY_State=(HAL_GPIO_ReadPin(B->GPIOx,B->GPIO_Pin)==GPIO_PIN_RESET?KEY_PRESS_SHORT:KEY_UP);
			break;
		}
		case KEY_PRESS_LONG:{
			B->KEY_State=(HAL_GPIO_ReadPin(B->GPIOx,B->GPIO_Pin)==GPIO_PIN_RESET?KEY_PRESS_LONG:KEY_UP);
			break;
		}
	}
	if(B==&B3) return (B->KEY_State==KEY_PRESS_LONG?2:(B->KEY_State==KEY_PRESS_SHORT?1:0));
	else return retVal;
}

Time_TypeDef temTime;
uint8_t b4_count;
void KEY_Scan(void){
	uint16_t b1_state=0,b2_state=0,b3_state=0,b4_state=0;
	b1_state=KEY_PressCheck(&B1);
	b2_state=KEY_PressCheck(&B2);
	b3_state=KEY_PressCheck(&B3);
	b4_state=KEY_PressCheck(&B4);
	if(b1_state){
		nowNum=(nowNum%5)+1;
		EEPROM_ReadTime(nowNum);
	}
	if(b2_state){
		if(b2_state==1){
			nowState=Setting;
			if(highLightPos!=12) highLightPos+=3;
			else highLightPos=6;
		}
		else{
			EEPROM_WriteTime(nowNum,nowTime);
		}
	}
	if(b3_state){
		if(!strcmp(nowState,Setting)){
			if(highLightPos==6) nowTime.hour=(++nowTime.hour)%100;
			else if(highLightPos==9) nowTime.minute=(++nowTime.minute)%60;
			else if(highLightPos==12) nowTime.second=(++nowTime.second)%60;
		}
	}
	if(b4_state){
		if(b4_state==1){
			if(strcmp(nowState,Running)&&strcmp(nowState,Pause)){
				temTime=nowTime;
				nowState=Running;
			}
			
			b4_count=(b4_count+1)%2;
			if(b4_count){
				HAL_TIM_Base_Start_IT(&htim1);
				nowState=Running;
			}
			else{
				HAL_TIM_Base_Stop_IT(&htim1);
				nowState=Pause;
			}
		}
		else{
			if(!strcmp(nowState,Running)){
				nowState=Standby;
				nowTime=temTime;
				temTime.hour=0;
				temTime.minute=0;
				temTime.second=0;
				b4_count=0;
				HAL_TIM_Base_Stop_IT(&htim1);
			}
		}
	}
	
}