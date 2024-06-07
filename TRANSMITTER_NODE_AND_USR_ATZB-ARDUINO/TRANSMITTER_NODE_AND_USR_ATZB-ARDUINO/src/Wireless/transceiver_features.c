/**
 * \file transceiver_features.c
 *
 * Created: 30/4/2024 14:46:43
 * Author: NetSoSe
 *
 * \brief  API file of transceiver feature access as generated by Project Wizard
 *
 * Copyright (c) 2014 Atmel Corporation. All rights reserved.
 *
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 * \page license License
 * Copyright(c) 2014, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */
/*
#include "transceiver_features.h"

//TODO (Project Wizard) - Transceiver Features - Call this function to read the battery voltage
float get_bat_sensor_data(void)
{
	float bat_voltage;
	bat_voltage = reverse_float(convert_mv_to_v(tfa_get_batmon_voltage()));
	return bat_voltage;
}
*/
/*Reverses a float variable*/
//float reverse_float( const float float_val )
//{
//	#if UC3
//	float retuVal;
//	char *floatToConvert = (char *)&float_val;
//	char *returnFloat = (char *)&retuVal;	
	/* swap the bytes into a temporary buffer */
//	returnFloat[0] = floatToConvert[3];
//	returnFloat[1] = floatToConvert[2];
//	returnFloat[2] = floatToConvert[1];
//	returnFloat[3] = floatToConvert[0];
//	return retuVal;
//	#else
//	return float_val; //nothing to be done for Little Endian System
//	#endif
//}

/* Converts milli Volt into Volt*/
//float convert_mv_to_v(uint16_t float_val)
//{
//	return (float_val * (1.0/1000));
//}