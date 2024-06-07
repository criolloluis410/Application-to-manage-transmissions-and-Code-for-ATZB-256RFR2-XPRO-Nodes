
#ifndef USR_WIRELESS_H_
#define USR_WIRELESS_H_

#include "wireless_api.h"
#include "tfa.h"
#define max_data 101
typedef struct
{
	uint8_t length;						// Frame length
	uint16_t fcf;							// fcf
	uint8_t num_sec;						// Sequence number
	uint16_t dir_PAN;							// PAN Direction
	uint16_t dir_destination;						// Source direction
	uint16_t dir_source;    					// Destination direction
	uint8_t payload_802_15_4[max_data];           // Payload
	uint16_t fcs;                           // frame control sequence
} frame_ieee_802_15_42;

frame_ieee_802_15_42 received_frame;

/**
* \brief This function needs to be edited by the user for adding application tasks
*/
void usr_wireless_app_task(void);

/**
* \brief This function needs to be edited by the user for adding  tasks when a frame is received
* \param frame pointer to the received frame
*/
void usr_frame_received_cb(frame_info_t *frame);

/**
* \brief This function needs to be edited by the user for adding  tasks when a frame is transmitted
* \param status  Status of frame transmission i.e MAC_SUCCESS,MAC_NO_ACK,CHANNEL_ACCESS_FAILURE etc
* \param frame pointer to the transmitted frame
*/
void usr_frame_transmitted_cb(retval_t status, frame_info_t *frame);

///////////////////////////////////////////////////////////////////////

float get_bat_sensor_data(void);

/*Reverses a float variable*/
float reverse_float( const float float_val);

/* Converts milli Volt into Volt*/
float convert_mv_to_v(uint16_t float_val);

//// Functions for converting floating numbers /////////////////////////////////////////////////////////////////////////////////////////
void reverse(char* str, int len);
int intToStr(int x, char str[], int d);
void ftoa(float n, char* res, int afterpoint);

#endif /* USER_WIRELESS_H_ */