/*******************************************************************************
* Copyright (c) 2020, STMicroelectronics - All Rights Reserved
*
* This file is part of the VL53L5CX Ultra Lite Driver and is dual licensed,
* either 'STMicroelectronics Proprietary license'
* or 'BSD 3-clause "New" or "Revised" License' , at your option.
*
********************************************************************************
*
* 'STMicroelectronics Proprietary license'
*
********************************************************************************
*
* License terms: STMicroelectronics Proprietary in accordance with licensing
* terms at www.st.com/sla0081
*
* STMicroelectronics confidential
* Reproduction and Communication of this document is strictly prohibited unless
* specifically authorized in writing by STMicroelectronics.
*
*
********************************************************************************
*
* Alternatively, the VL53L5CX Ultra Lite Driver may be distributed under the
* terms of 'BSD 3-clause "New" or "Revised" License', in which case the
* following provisions apply instead of the ones mentioned above :
*
********************************************************************************
*
* License terms: BSD 3-clause "New" or "Revised" License.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* 3. Neither the name of the copyright holder nor the names of its contributors
* may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*
*******************************************************************************/


#include "platform.h"


PIO i2c_pio;
uint i2c_sm;
uint i2c_offset;

void i2c_pio_init(int sda, int scl)
{
	i2c_pio = pio0;
	i2c_sm = 0;
	i2c_offset = pio_add_program(i2c_pio, &i2c_program);
	i2c_program_init(i2c_pio, i2c_sm, i2c_offset, sda, scl);
}

uint8_t RdByte(
		VL53L5CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t *p_value)
{	
	// addr: higher byte first, lower byte second.
	uint8_t tmp[2] = {(uint8_t)(RegisterAdress >> 8 & 0xFF), (uint8_t)(RegisterAdress & 0xFF)};
	pio_i2c_write_blocking(i2c_pio, i2c_sm, p_platform->address, tmp, 2);
	int8_t ret = pio_i2c_read_blocking(i2c_pio, i2c_sm, p_platform->address, p_value, 1);
	return ((ret == PICO_ERROR_GENERIC) ? 255 : 0);
}

uint8_t WrByte(
		VL53L5CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t value)
{

	uint8_t tmp[3] = {(uint8_t)(RegisterAdress >> 8 & 0xFF), (uint8_t)(RegisterAdress & 0xFF), value};
	int8_t ret = pio_i2c_write_blocking(i2c_pio, i2c_sm, p_platform->address, tmp, 3);
	return ((ret == PICO_ERROR_GENERIC) ? 255 : 0);
}

uint8_t WrMulti(
		VL53L5CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t *p_values,
		uint32_t size)
{	
	uint8_t tmp[2] = {(uint8_t)(RegisterAdress >> 8), (uint8_t)(RegisterAdress & 0xFF)};
	pio_i2c_write_blocking_nostop(i2c_pio, i2c_sm, p_platform->address, tmp, 2);

	// send without Start conditions between chunks
	pio_i2c_write_blocking_nostart(i2c_pio,i2c_sm, p_platform->address, p_values, size);

	return 0;
}

uint8_t RdMulti(
		VL53L5CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t *p_values,
		uint32_t size)
{	
	uint8_t tmp[2] = {(uint8_t)(RegisterAdress >> 8 & 0xFF), (uint8_t)(RegisterAdress & 0xFF)};
	pio_i2c_write_blocking(i2c_pio, i2c_sm, p_platform->address,tmp,2);
	int8_t ret = pio_i2c_read_blocking(i2c_pio, i2c_sm, p_platform->address, p_values, size);
	return ((ret == PICO_ERROR_GENERIC) ? 255 : 0);
}

uint8_t Reset_Sensor(
		VL53L5CX_Platform *p_platform)
{
	uint8_t status = 0;
	
	/* (Optional) Need to be implemented by customer. This function returns 0 if OK */

	/* Set pin LPN to LOW */
	/* Set pin AVDD to LOW */
	/* Set pin VDDIO  to LOW */
	WaitMs(p_platform, 100);

	/* Set pin LPN of to HIGH */
	/* Set pin AVDD of to HIGH */
	/* Set pin VDDIO of  to HIGH */
	WaitMs(p_platform, 100);

	return status;
}

void SwapBuffer(
		uint8_t 		*buffer,
		uint16_t 	 	 size)
{
	uint32_t i, tmp;
	
	/* Example of possible implementation using <string.h> */
	for(i = 0; i < size; i = i + 4) 
	{
		tmp = (
		  buffer[i]<<24)
		|(buffer[i+1]<<16)
		|(buffer[i+2]<<8)
		|(buffer[i+3]);
		
		memcpy(&(buffer[i]), &tmp, 4);
	}
}	

uint8_t WaitMs(
		VL53L5CX_Platform *p_platform,
		uint32_t TimeMs)
{
	sleep_ms(TimeMs);
	return 0;
}
