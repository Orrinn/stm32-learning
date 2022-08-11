#include "parking.h"
#include "stdio.h"

ParkingLot_TypeDef pk={0,0,8,3.5,2.0};

//decade_index: 十位的index，unit_index:个位的index
int Char2Int(char decade_index,char unit_index){
	return (rxBuffer[decade_index]-'0')*10+(rxBuffer[unit_index]-'0');
}

//for rxBuffer:10-11:YY,12-13:MM,14-15:DD,16-17:HH,18-19:mm,20-21:SS
Time_TypeDef ParkingLot_Str2Time(){
	int year=Char2Int(10,11);
	int month=Char2Int(12,13);
	int data=Char2Int(14,15);
	int hour=Char2Int(16,17);
	int minute=Char2Int(18,19);
	int second=Char2Int(20,21);
	Time_TypeDef time={year,month,data,hour,minute,second};
	return time;
}


const uint16_t DATA[12]={31,28,31,30,31,30,31,31,30,31,30,31};

//return the standard hours from  00-00-00-00-00-00;
int ParkingLot_Time2Hour(Time_TypeDef t){
	int Yh=t.year*365*24;
  int Mh=0;
  for(int i=0;i<t.month-1;i++) Mh+=DATA[i]*24;
  int Dh=t.data*24;
  int Hh=t.hour;
  int tot=Yh+Mh+Dh+Hh;
  return tot;
}

int ParkingLot_HourCount(Time_TypeDef t1,Time_TypeDef t2){
	int h1=0,h2=0,diff=0;
	h1=ParkingLot_Time2Hour(t1);
	h2=ParkingLot_Time2Hour(t2);
	if(t2.minute>t1.minute) diff=h2-h1+1;
	else diff=h2-h1;
	
	return diff;
}

// 1:incorrect,0:correct
//
int ParkingLot_TimeCheck(Time_TypeDef time){
	if(time.month>12||time.month<1) return 1;
	else if(time.data<1||time.data>DATA[time.month-1]) return 1;
	else if(time.hour>23||time.hour<0) return 1;
	else if(time.minute>59||time.minute<0) return 1;
	else if(time.second>59||time.second<0) return 1;
	else return 0;
}

int ParkingLot_AddCar(char id[5],Time_TypeDef time,uint16_t type){
	if(pk.IDLE_Count==0) return Parking_Fail;
 	for(int i=0;i<8;i++){
		if(!strcmp(pk.Car_queue[i].carId,"\0\0\0\0")){
			strcpy(pk.Car_queue[i].carId,id);
			//printf("adding car,pk id is:%s,id is:%s\r\n",pk.Car_queue[i].carId,id);
      pk.Car_queue[i].parkingTime=time;
      pk.Car_queue[i].carType=type;
      break;
    }
	}

  if(type==CNBR) pk.CNBR_Count++;
  else pk.VNBR_Count++;
  pk.IDLE_Count--;

}

//if exits,return index,else return -1
int ParkingLot_GetParkingLoc(char id[5],uint16_t type){
	for(int i=0;i<8;i++){ 
		if((!strcmp(pk.Car_queue[i].carId,id))&&(pk.Car_queue[i].carType==type)) return i;
		//printf("%d-th,pk is:%s,id is :%s,compare res is %d \r\n",i,pk.Car_queue[i].carId,id,strcmp(pk.Car_queue[i].carId,id));
	}
	return -1;
}

void ParkingLot_RemoveCar(char id[5],uint16_t type){
	int loc=ParkingLot_GetParkingLoc(id,type);
	
	if(loc!=-1){
		if(pk.Car_queue[loc].carType==CNBR) pk.CNBR_Count--;
		else pk.VNBR_Count--;
		pk.IDLE_Count++;
		
		memset(pk.Car_queue[loc].carId,0,sizeof(pk.Car_queue[loc].carId));
	}
}