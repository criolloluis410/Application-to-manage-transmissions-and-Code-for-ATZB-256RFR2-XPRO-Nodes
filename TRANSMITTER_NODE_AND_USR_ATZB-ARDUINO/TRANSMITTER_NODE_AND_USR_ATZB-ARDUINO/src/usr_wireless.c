//Libraries
#include "usr_wireless.h"
#include "sio2host.h"
#include "wireless_config.h"
#include "stdint.h"
#include "math.h"
#include "stdio.h"
//Variables used
int obtainNB=1;//int max=0;	
uint8_t batteryVoltaje[7];
uint8_t dataPayload[100]={-2, '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'};
uint16_t sequenceNumber=0;

int transmissionsNum=0;
int contTX=0;
int interframeTime=200;

int blocksNumber=0;
int contBL=0;//2
int indicatorBL=0;
int interblockTime=200;//5000

int transmissionPower=0;//-31

int frameType=0;//1
int transmissionBegin=0;
int frameLength=0;
int listenParam=0;

//The main flow of the function involves the preparation and transmission of a data packet containing 
//information about the battery and transmission power. Subsequently, the function manages the transmission 
//sequence of these packets, with a limit on the number of transmissions and the ability to send special 
//messages for data flow control.

void usr_wireless_app_task(void)
{
	newPtTx=transmissionPower;
	
	if (transmissionBegin==1)
	{// Transmission activation variable
		obtainNB=0;
		// Obtaining battery level ////////////////////////////////////////////////////////////
		char charBattery[20]; // Store the battery level converted into a string
		float floatBattery=get_bat_sensor_data();// Get battery level
		ftoa(floatBattery, charBattery, 4);// Convert the battery level from float to a hexadecimal string
		dataPayload[7]=(uint8_t)charBattery[0];// Store the first four values of the string in the NB vector
		dataPayload[8]=(uint8_t)charBattery[1];
		dataPayload[9]=(uint8_t)charBattery[2];
		dataPayload[10]=(uint8_t)charBattery[3];
		//dataPayload[11]='_';
		// Get power level
		char Prx[3]="000"; // Adjust the size according to your needs
		// Use sprintf to convert the integer to a string
		sprintf(Prx,"%03d",transmissionPower);
		dataPayload[11]=(uint8_t)Prx[0];// Store the first three values of the string in the NB vector
		dataPayload[12]=(uint8_t)Prx[1];
		dataPayload[13]=(uint8_t)Prx[2];
		
		if (contTX > 0)//Number of times it will be transmitted
		{//Transmission of the message and its size
			char NumSec[6];
			char NumSecArd[4];
			if (frameType==1)
			{
				LED_Toggle(LED0);
				LED_Toggle(LED1);
				frameLength=23;
				dataPayload[0]='C';
				sprintf(NumSec,"0x%04X",sequenceNumber);
				dataPayload[1]=(uint8_t)NumSec[0];// Store the values of the sequence number
				dataPayload[2]=(uint8_t)NumSec[1];
				dataPayload[3]=(uint8_t)NumSec[2];
				dataPayload[4]=(uint8_t)NumSec[3];
				dataPayload[5]=(uint8_t)NumSec[4];
				dataPayload[6]=(uint8_t)NumSec[5];
				NumSecArd[0]=(uint8_t)NumSec[2];// Store the sequence number values for the Arduino
				NumSecArd[1]=(uint8_t)NumSec[3];
				NumSecArd[2]=(uint8_t)NumSec[4];
				NumSecArd[3]=(uint8_t)NumSec[5];
				sio2host_tx(NumSecArd,4);
			}
			else if (frameType==2)
			{
				LED_Toggle(LED0);
				LED_Toggle(LED1);
				frameLength=100;
				dataPayload[0]='L';
				sprintf(NumSec,"0x%04X",sequenceNumber);
				dataPayload[1]=(uint8_t)NumSec[0];// Store the values of the sequence number
				dataPayload[2]=(uint8_t)NumSec[1];
				dataPayload[3]=(uint8_t)NumSec[2];
				dataPayload[4]=(uint8_t)NumSec[3];
				dataPayload[5]=(uint8_t)NumSec[4];
				dataPayload[6]=(uint8_t)NumSec[5];
				NumSecArd[0]=(uint8_t)NumSec[2];// Store the sequence number values for the Arduino
				NumSecArd[1]=(uint8_t)NumSec[3];
				NumSecArd[2]=(uint8_t)NumSec[4];
				NumSecArd[3]=(uint8_t)NumSec[5];
				sio2host_tx(NumSecArd,4);
			}
			transmit_sample_frame(dataPayload, frameLength);
			delay_ms(interframeTime);//Waiting time between sent frames
			sequenceNumber++;
			contTX--;
		}
		
		else if(contTX <= 0)
		{//Variable reset
			uint8_t msgBL[11]="Bloque_0___";
			contTX=0;
			//LED_On(LED0);
			if (contBL > 1)
			{
				//LED_On(LED1);
				transmissionBegin=1;
				contTX=transmissionsNum;
				
				indicatorBL++;
				msgBL[7]=indicatorBL+'0';
				transmit_sample_frame(msgBL,8);
				delay_ms(interblockTime);//Waiting time between sent frames
				contBL--;
			}
			else if (contBL<= 1)
			{
				indicatorBL++;
				msgBL[7]=indicatorBL+'0';
				transmit_sample_frame(msgBL,11);
				transmissionBegin=0;
				delay_ms(interframeTime);//Waiting time between sent frames
				indicatorBL=0;
			}
		}
	}
	//delay_ms(10);??
}

//The primary function flow involves receiving a frame of data and processing it. Initially, it clears any previous data from the reception 
//structure and then copies the received frame into memory. Subsequently, it checks if the frame originated from a specific node. 
//If so, it extracts and interprets certain commands embedded within the frame payload. These commands dictate various parameters 
//such as transmission count, inter-frame delay, number of blocks, inter-block delay, transmission power, and frame type. 
//Additionally, it sets flags based on received commands to enable or disable listening for further commands.

void usr_frame_received_cb(frame_info_t *frame)
{
	memset(&received_frame,0,sizeof(received_frame));// Removes previous information from the reception structure
	memcpy(&received_frame,frame->mpdu,sizeof(received_frame));
	//bmm_buffer_free(frame->buffer_header);
	
	if (received_frame.dir_source==0x0002) // If this condition is met, the frame is from the connected node
	{
		uint8_t initRx[3];//initial
		initRx[0]=(uint8_t)received_frame.payload_802_15_4[0];// stores the first four values ​​of the string in the vector N
		initRx[1]=(uint8_t)received_frame.payload_802_15_4[1];
		initRx[2]=(uint8_t)received_frame.payload_802_15_4[2];
		///////////////////////////You must meet this condition to listen to the orders of the next plot
		if ( initRx[0]=='i' && initRx[1]=='n' && initRx[2]=='i' )
		{
			listenParam=1;

		}
		if (listenParam==1)
		{
			int lengthFrameRx=((int)received_frame.length)-11;
			char ord=received_frame.payload_802_15_4[lengthFrameRx-1];
			
			if (ord=='a')
			{
				//LED_Toggle(LED0);
				transmissionsNum = atoi(received_frame.payload_802_15_4);
				//contParametros++;
			}
			else if(ord=='b')
			{
				//LED_Toggle(LED1);
				interframeTime = atoi(received_frame.payload_802_15_4);
			}
			else if(ord=='c')
			{
				blocksNumber = atoi(received_frame.payload_802_15_4);
			}
			else if(ord=='d')
			{
				interblockTime = atoi(received_frame.payload_802_15_4);
				interblockTime =interblockTime*1000;
			}
			else if(ord=='e')
			{
				//LED_Toggle(LED2);
				transmissionPower = atoi(received_frame.payload_802_15_4);
			}
			else if(ord=='f')
			{
				frameType = atoi(received_frame.payload_802_15_4);
			}
			//////////////////////////////////////Stop listening to orders
			else if(ord=='n')
			{
				//LED_Toggle(LED0);
				uint8_t noListen[3];
				noListen[0]=(uint8_t)received_frame.payload_802_15_4[0];// Stores the first four values ​​of the string in the vector N
				noListen[1]=(uint8_t)received_frame.payload_802_15_4[1];
				noListen[2]=(uint8_t)received_frame.payload_802_15_4[2];
				if (noListen[0]=='f' && noListen[1]=='i' && noListen[2]=='n')
				{
					LED_Off(LED0);
					LED_On(LED1);
					LED_On(LED2);
					listenParam=0;
					//contParametros==0;
					transmissionBegin=1;
					contTX=transmissionsNum;
					contBL=blocksNumber;
					//max=0;
				}
			}
		}
	}
}

//This function appears to be a callback invoked after a frame has been transmitted

void usr_frame_transmitted_cb(retval_t status, frame_info_t *frame)
{

}

//////////////////////////////////////////////////////////////////////////////

//This function reverse reverses a character array str of length len. 
//It achieves this by swapping the characters from the beginning and end of the array until the midpoint is reached.

void reverse(char* str, int len)
{
	int i = 0, j = len - 1, temp;
	while (i < j) {
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++;
		j--;
	}
}

//This function intToStr converts an integer x into a string str and returns the length of the string. 
//It also pads the string with zeros if the number of digits required (d) is more than the actual number of digits in x. 
//It achieves this by repeatedly dividing x by 10 to extract each digit, converting it to a character, and storing it in the str array. 
//Finally, it calls the reverse function to reverse the string since the digits were stored in reverse order during the conversion process.

int intToStr(int x, char str[], int d)
{
	int i = 0;
	while (x) {
		str[i++] = (x % 10) + '0';
		x = x / 10;
	}
	
	// If number of digits required is more, then
	// add 0s at the beginning
	while (i < d)
	str[i++] = '0';
	
	reverse(str, i);
	str[i] = '\0';
	return i;
}

//This function, ftoa, converts a floating-point number n into a string res, specifying the number of digits to be displayed after the decimal point (afterpoint). 
//It separates the integer part (ipart) and the fractional part (fpart) of n, converts the integer part into a string, storing it in res. 
//If afterpoint is nonzero, it adds a decimal point to the string and calculates the digits of the fractional part, then appends them to the res string.

void ftoa(float n, char* res, int afterpoint)
{
	// Extract integer part
	int ipart = (int)n;
	
	// Extract floating part
	float fpart = n - (float)ipart;
	
	// convert integer part to string
	int i = intToStr(ipart, res, 0);
	
	// check for display option after point
	if (afterpoint != 0) {
		res[i] = '.'; // add dot
		
		// Get the value of fraction part upto given no.
		// of points after dot. The third parameter
		// is needed to handle cases like 233.007
		fpart = fpart * pow(10, afterpoint);
		
		intToStr((int)fpart, res + i + 1, afterpoint);
	}
}
////////////////////////////////////////////////////////////////////////////////////7

//This function get_bat_sensor_data retrieves battery sensor data. 
//It first obtains the battery voltage in millivolts using the function tfa_get_batmon_voltage(), 
//converts it to volts, and then reverses the order of the floating-point number before returning the battery voltage value.

float get_bat_sensor_data(void)
{
	float bat_voltage;
	bat_voltage = reverse_float(convert_mv_to_v(tfa_get_batmon_voltage()));
	return bat_voltage;
}

/*Reverses a float variable*/
float reverse_float( const float float_val )
{
	#if UC3
	float retuVal;
	char *floatToConvert = (char *)&float_val;
	char *returnFloat = (char *)&retuVal;
	/* swap the bytes into a temporary buffer */
	returnFloat[0] = floatToConvert[3];
	returnFloat[1] = floatToConvert[2];
	returnFloat[2] = floatToConvert[1];
	returnFloat[3] = floatToConvert[0];
	return retuVal;
	#else
	return float_val; //nothing to be done for Little Endian System
	#endif
}

/* Converts milli Volt into Volt*/
float convert_mv_to_v(uint16_t float_val)
{
	return (float_val * (1.0/1000));
}