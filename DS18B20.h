/*
 * DS18B20.h
 *
 *  Created on: Nov 10, 2020
 *      Author: marco
 */

#ifndef DS18B20_H_
#define DS18B20_H_

#include "main.h"


int8_t DS18B20_Start (void);

void DS18B20_Write (uint8_t data);

uint8_t DS18B20_Read (void);

float ReadTemperature(void);


#endif /* DS18B20_H_ */
