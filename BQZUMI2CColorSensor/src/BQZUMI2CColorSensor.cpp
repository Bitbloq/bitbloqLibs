/*
 * BQZUMI2CColorSensor.h
 *
 * Copyright 2018 Alberto Valero <alberto.valero@bq.com>
 *                Pablo García <pablo.garcia@bq.com>
 * 								Jorge Campo <jorge.campo@bq.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


#include <Wire.h>
#include <BQZUMI2CColorSensor.h>

namespace BQ{ namespace ZUM{

// Instantiate I2CColorSensor class
I2CColorSensor::I2CColorSensor(uint8_t ui8_i2cport):colorsens_i2cport(ui8_i2cport)
{
}

void I2CColorSensor::setup(){
	for (int i=0;i<10;i++){
		if(begin()==COLOR_OK) break;
	}
}

ColorStat I2CColorSensor::begin()
{
	uint8_t ui8_DRVid	= 0;
	uint8_t ui8_DRVconf	= 0;

	switch (colorsens_i2cport) {
		case 0:
			Wire.begin();
			break;
		case 1:
			Wire1.begin();
			break;
		default:
			return COLOR_NOK_i2cport;
	}

	ui8_DRVid = readRegister(BH1745NUC_REG_SYSCTL) & BH1745NUC_SYSCTL_ID;

	if (ui8_DRVid != BH1745NUC_ID)
	{
		return COLOR_NOK_config;
	}

	// Configure sensor
	//ui8_DRVconf = BH1745NUC_SYSCTL_INT;
	//writeRegister(BH1745NUC_REG_SYSCTL, ui8_DRVconf);

	ui8_DRVconf = BH1745NUC_MCTL1_MST_160;
	writeRegister(BH1745NUC_REG_MODE_CTL1, ui8_DRVconf);

	ui8_DRVconf = BH1745NUC_MCTL2_VALID | BH1745NUC_MCTL2_RGBC_EN | BH1745NUC_MCTL2_ADCG_1X;
	writeRegister(BH1745NUC_REG_MODE_CTL2, ui8_DRVconf);

	ui8_DRVconf = BH1745NUC_MCTL3;
	writeRegister(BH1745NUC_REG_MODE_CTL3, ui8_DRVconf);

	return COLOR_OK;
}

void I2CColorSensor::writeRegister(uint8_t ui8_Reg, uint8_t ui8_data)
{
	switch (colorsens_i2cport) {
		case 0:
			Wire.beginTransmission(BH1745NUC_ADDR);
			Wire.write((byte)ui8_Reg);
			Wire.write((byte)ui8_data);
			Wire.endTransmission();
			break;
		case 1:
			Wire1.beginTransmission(BH1745NUC_ADDR);
			Wire1.write((byte)ui8_Reg);
			Wire1.write((byte)ui8_data);
			Wire1.endTransmission();
			break;
		default:
			return;
	}

}

uint8_t I2CColorSensor::readRegister(uint8_t ui8_Reg)
{
	uint8_t ui8_data = 0;

	switch (colorsens_i2cport) {
		case 0:
			Wire.beginTransmission(BH1745NUC_ADDR);
			Wire.write((byte)ui8_Reg);
			Wire.endTransmission(false);
			Wire.requestFrom((byte)BH1745NUC_ADDR, (byte)1);
			ui8_data = Wire.read();
			break;
		case 1:
			Wire1.beginTransmission(BH1745NUC_ADDR);
			Wire1.write((byte)ui8_Reg);
			Wire1.endTransmission(false);
			Wire1.requestFrom((byte)BH1745NUC_ADDR, (byte)1);
			ui8_data = Wire1.read();
			break;
		default:
			return 0;
	}

  return ui8_data;
}

float I2CColorSensor::getComponent(uint8_t ui8_component){
  float     f_Red	= 0;
  float     f_Green = 0;
  float     f_Blue	= 0;
  float     f_Clear = 0;
  getColor(&f_Red,&f_Green,&f_Blue,&f_Clear);
  switch(ui8_component){
    case 0:
      return f_Red;
    case 1:
      return f_Green;
    case 2:
      return f_Blue;
    default:
      return 0;
  }
}

void I2CColorSensor::getColor(float *f_Red, float *f_Green, float *f_Blue, float *f_Clear)
{
	uint8_t     ui8_data = 0;
	uint16_t    ui16_color = 0;

	do
	{
		ui8_data = readRegister(BH1745NUC_REG_MODE_CTL2);
	}
	while (ui8_data & BH1745NUC_MCTL2_VALID != 0);

	ui16_color = (readRegister(BH1745NUC_REG_RED_MSB)<<8) + (readRegister(BH1745NUC_REG_RED_LSB));
	*f_Red = ui16_color * BH1745NUC_RED_COMP;

	ui16_color = (readRegister(BH1745NUC_REG_GREEN_MSB)<<8) + (readRegister(BH1745NUC_REG_GREEN_LSB));
	*f_Green = ui16_color * BH1745NUC_GREEN_COMP;

	ui16_color = (readRegister(BH1745NUC_REG_BLUE_MSB)<<8) + (readRegister(BH1745NUC_REG_BLUE_LSB));
	*f_Blue = ui16_color * BH1745NUC_BLUE_COMP;

	ui16_color = (readRegister(BH1745NUC_REG_CLEAR_MSB)<<8) + (readRegister(BH1745NUC_REG_CLEAR_LSB));
	*f_Clear = ui16_color;
}

I2CColorSensor::Colors I2CColorSensor::whichColor(){
	//GET COLOR
	float	f_Red	= 0;
	float	f_Green	= 0;
	float	f_Blue	= 0;
	float	f_Clear	= 0;
	float 	f_AverageColour = 0;	
	int 	i_RedCount = 0;
	int 	i_GreenCount = 0;
	int	i_BlueCount = 0;
	int	i_BlackCount = 0;
	
	i_RedCount = 0;
	i_GreenCount = 0;
	i_BlueCount = 0;
	i_BlackCount = 0;

	for (int i_Cont = 0; i_Cont < 50; i_Cont++) {
		getColor(&f_Red, &f_Green, &f_Blue, &f_Clear);
		f_Red = f_Red * 0.7;
		f_Green = f_Green * 0.9;
		f_AverageColour = (f_Red + f_Green + f_Blue) / 3;

		if ((f_Red < 15) && (f_Green < 15) && (f_Blue < 15))	i_BlackCount++;
		 else if ((f_Red > f_Green) && (f_Red > f_Blue) && ((f_Red - f_AverageColour) > (f_AverageColour * 0.2)))   i_RedCount++;
		 else if ((f_Green > f_Red) && (f_Green > f_Blue) && ((f_Green - f_AverageColour) > (f_AverageColour * 0.2)))    i_GreenCount++;
		 else if ((f_Blue - f_AverageColour) > (f_AverageColour * 0.2))     i_BlueCount++;
	}
	    

	    if (i_BlackCount > 25) return I2CColorSensor::BLACK;
	    if ((i_RedCount + i_GreenCount + i_BlueCount) < 25) return I2CColorSensor::WHITE;
	    if ((i_GreenCount > i_RedCount) && (i_GreenCount > i_BlueCount)) return I2CColorSensor::GREEN;
	    if ((i_BlueCount > i_RedCount) && (i_BlueCount > i_GreenCount)) return I2CColorSensor::BLUE;
	    if ((i_RedCount > i_GreenCount) && (i_RedCount > i_BlueCount)) return I2CColorSensor::RED;

}
}} //end namespace BQ::ZUM
