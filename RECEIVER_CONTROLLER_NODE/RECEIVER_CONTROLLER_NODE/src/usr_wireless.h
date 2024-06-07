
#ifndef USR_WIRELESS_H_
#define USR_WIRELESS_H_

#include "wireless_api.h"
#define max_dato 101
typedef struct
{
	uint8_t length0;						// Longitud de Trama
	uint16_t fcf;							// fcf
	uint8_t num_sec;						// Numero de secuencia
	uint16_t dir_PAN;							// Direccion PAN
	uint16_t dir_destination;						// Direccion ORIGEN
	uint16_t dir_source;    					// Direccion DESTINO
	uint8_t payload_802_15_4[max_dato];           // Payload
	uint16_t fcs;                           // frame control secquence
} frame_ieee_802_15_42;

frame_ieee_802_15_42 received_frame;
//////////////////////////////////////////////////////////////////////////
void usr_wireless_app_task(void);

void usr_frame_received_cb(frame_info_t *frame);

void usr_frame_transmitted_cb(retval_t status, frame_info_t *frame);

#endif