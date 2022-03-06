/*
 * DS18B20.c
 *
 *  Created on: Nov 10, 2020
 *      Author: marco
 */


#include "DS18B20.h"
//#include <stdio.h>

#include "main.h"


int8_t DS18B20_Start (void) //Initialization function
{
	int8_t Response = 0;

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = DS18B20_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);


	HAL_GPIO_WritePin (DS18B20_GPIO_Port, DS18B20_Pin, 0);  // pull the pin low
	delay (480);   // delay according to datasheet
	//HAL_Delay(480);

	GPIO_InitStruct.Pin = DS18B20_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);
	delay (80);    // delay according to datasheet
	//HAL_Delay(80);


	if (!(HAL_GPIO_ReadPin (DS18B20_GPIO_Port, DS18B20_Pin))) Response = 1;    // if the pin is low i.e the presence pulse is detected (if it goes to 0)
	else Response = -1;

	delay (400); // 480 us delay totally.
	//HAL_Delay(400);
	return Response;
}


void DS18B20_Write (uint8_t data)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = DS18B20_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);

	for (int i=0; i<8; i++)
	{

		if ((data & (1<<i))!=0)  // if the bit is high
		{
			// write 1

			GPIO_InitStruct.Pin = DS18B20_Pin;
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStruct.Pull = GPIO_PULLUP;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
			HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);
			HAL_GPIO_WritePin (DS18B20_GPIO_Port, DS18B20_Pin, 0);  // pull the pin LOW
			delay (1);  // wait for 1 us
			//HAL_Delay(1);


			GPIO_InitStruct.Pin = DS18B20_Pin;
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = GPIO_PULLUP;
			HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);			//delay (50);  // wait for 60 us
			delay(50);
		}

		else  // if the bit is low
		{
			// write 0

			GPIO_InitStruct.Pin = DS18B20_Pin;
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStruct.Pull = GPIO_PULLUP;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
			HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);

			HAL_GPIO_WritePin (DS18B20_GPIO_Port, DS18B20_Pin, 0);  // pull the pin LOW
			delay (50);  // wait for 60 us
			//HAL_Delay(50);


			GPIO_InitStruct.Pin = DS18B20_Pin;
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = GPIO_PULLUP;
			HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);
		}
	}
}


uint8_t DS18B20_Read (void)
{
	uint8_t value=0;

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = DS18B20_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);

	for (int i=0;i<8;i++)
	{
		GPIO_InitStruct.Pin = DS18B20_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);

		HAL_GPIO_WritePin (DS18B20_GPIO_Port, DS18B20_Pin, 0);  // pull the data pin LOW
		delay (2);  // wait for 2 us
		//HAL_Delay(2);


		GPIO_InitStruct.Pin = DS18B20_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);  // set as input
		if (HAL_GPIO_ReadPin (DS18B20_GPIO_Port, DS18B20_Pin))  // if the pin is HIGH
		{
			value |= 1<<i;  // read = 1 //read data input
		}
		delay (60);  // wait for 60 us
		//HAL_Delay(60);

	}
	return value;
}

float ReadTemperature(void){
	  uint8_t Temp_byte1, Temp_byte2;
	  uint16_t TEMP;
	  float Temperature = 0;
	  int8_t presence = 0;



	  presence = DS18B20_Start ();
	  if (presence==1){
		  HAL_Delay (1);
		  DS18B20_Write (0xCC);  // skip ROM
		  DS18B20_Write (0x44);  // convert t
		  HAL_Delay (800);

		  presence = DS18B20_Start ();
		  if (presence==1){
			  HAL_Delay(1);
			  DS18B20_Write (0xCC);  // skip ROM
			  DS18B20_Write (0xBE);  // Read Scratch-pad

			  Temp_byte1 = DS18B20_Read();
			  Temp_byte2 = DS18B20_Read();
			  TEMP = (Temp_byte2<<8)|Temp_byte1;
			  Temperature = (float)TEMP/16;
		  }
	  }

	  if (presence!=1){
		  return -100;
	  }



	return Temperature;
}
































