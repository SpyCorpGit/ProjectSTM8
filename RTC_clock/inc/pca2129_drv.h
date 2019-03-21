/*
 * pca2129_i2c.h
 *
 *  Created on: 25.02.2013
 *      Author: BARS
 */

#ifndef PCA2129_I2C_H_
#define PCA2129_I2C_H_


#include "I2c_drv.h"


typedef struct Time{
	
	unsigned char hour;
	unsigned char min;
	unsigned char sec;
	
}Time;

typedef struct tTime{
	
	unsigned char t1_hour;
	unsigned char t1_min;
	unsigned char t1_sec;
	unsigned char t2_hour;
	unsigned char t2_min;
	unsigned char t2_sec;
	
}tTime;

typedef struct Date{
	
	unsigned char day;
	unsigned char month;
	unsigned char year;
	unsigned char weekday;
	
}Date; 

unsigned char bcd (unsigned char data);
void pca2129_init(unsigned char t_meas, unsigned char out); // инициализация
void pca2129_set_time(unsigned char hour,unsigned char min,unsigned char sec); // установка времени
void pca2129_get_time(unsigned char *hour,unsigned char *min,unsigned char *sec); // чтение времени
void pca2129_set_date(unsigned char day, unsigned char month, unsigned char year, unsigned char weekday); // установка даты
void pca2129_get_date(unsigned char *day, unsigned char *month, unsigned char *year, unsigned char *weekday); // чтение даты
void pca2129_set_alarm(unsigned char hour, unsigned char min, unsigned char sec, unsigned char day, unsigned char start); // установка будильника
unsigned char pca2129_get_alarm(); // чтение состояния будильника
unsigned char low_bat(); // чтение состояния батареи
void WDT(unsigned char en, unsigned char clock, unsigned char period); // настройка сторожевого таймера

#endif /* PCA2129_I2C_H_ */
