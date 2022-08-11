#ifndef __PARKING_H__
#define __PARKING_H__

#include "main.h"
#include "string.h"

#define Parking_Success 0
#define Parking_Fail 1

typedef struct{
	uint16_t year;
	uint16_t month;
	uint16_t data;
	uint16_t hour;
	uint16_t minute;
	uint16_t second;
}Time_TypeDef;


#define CNBR 1
#define VNBR 2
typedef struct{
	char carId[5];
	Time_TypeDef parkingTime;
	uint16_t carType;
}Car_TypeDef;

typedef struct{
	uint16_t CNBR_Count;
	uint16_t VNBR_Count;
	uint16_t IDLE_Count;
	double CNBR_Price;
	double VNBR_Price;
	Car_TypeDef Car_queue[8];
}ParkingLot_TypeDef;

extern ParkingLot_TypeDef pk;


void ParkingLot_RemoveCar(char id[5],uint16_t type);
int ParkingLot_AddCar(char id[5],Time_TypeDef time,uint16_t type);
int ParkingLot_GetParkingLoc(char id[5],uint16_t type);
Time_TypeDef ParkingLot_Str2Time(void);
int ParkingLot_TimeCheck(Time_TypeDef time);
int ParkingLot_HourCount(Time_TypeDef t1,Time_TypeDef t2);

#endif


