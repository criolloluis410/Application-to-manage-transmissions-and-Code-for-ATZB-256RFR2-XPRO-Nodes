//Libraries
#include "usr_wireless.h"
#include "sio2host.h"
#include "wireless_config.h"
#include "stdio.h"
//Variables
int saveDatas=0; // Variable for Data Storage Control
uint8_t storageVector[20000]; // Vector for storing received data
uint8_t testFrame[7]; // Vector for tests
int des=0; int desAnt=0; // Variables for storage vector control
uint8_t sec=0; // Variable to store the sequence number
uint8_t ed_val; uint8_t frame_power; // Variable to store the energy level of received frames
int Npayload=0; int shift0=0; // Variables for storage vector control
int  contSto=0; int limSto=0;  // Variables to count the number of received frames
//////////////////////////////////////////////////////////////////////////

//This function manages communication between nodes and an application through serial and wireless connections. 
//When data is received from the application, it is stored. If the \t symbol is received, it transmits a connection 
//confirmation message and the stored data to the external node. If \r is received, it sends stored data to the application 
//in blocks of 256 bytes. And if it receives \b, it sends the node's source address to the application.

void usr_wireless_app_task(void)
{
	uint8_t confirmationMessage[6]="Listo";//Connection confirmation message
	confirmationMessage[5]=0xFF;//Stop buffer
	int8_t chat_input = sio2host_getchar_nowait();//Serial message acquisition
	static uint8_t payload_length,payload[aMaxPHYPacketSize-FRAME_OVERHEAD];// Vector for storing data sent by the app
	if (chat_input != -1)
	{
		payload[payload_length++] = chat_input;//Data storage for the app
		
	}// If the symbol \t is received, the node proceeds to transmit commands to the transmitter  
	if((chat_input == '\t')||(payload_length == (aMaxPHYPacketSize-FRAME_OVERHEAD))){                                                      
		sio2host_tx(confirmationMessage,6);//Transmission of the connection confirmation message
		transmit_sample_frame(payload,payload_length-1);//Transmit what is received through the serial connection to the external node
		payload_length = 0;//Variable reset
		
	}// If the symbol \r is received, the node proceeds to send data to the application
	if((chat_input == '\r')||(payload_length == (aMaxPHYPacketSize-FRAME_OVERHEAD)))
	{
		shift0=0;
		//Saving and sending of previously stored data (every 256 bytes)
		for (int i = 0; i < 42; i++)
		{
			uint8_t dataPC[255];
			for (int j = 0; j <255 ; j++)// The data is stored within the 'dataPC' vector to continue with the next 256 bytes
			{
				dataPC[j]=storageVector[shift0+j];
			}
			shift0=shift0+255;
			sio2host_tx(dataPC,255);//Send data to PC
			LED_Toggle(LED1);
		}
		LED_Off(LED1);
		
	} // If the symbol \b is received, the node sends its source address to the application
	if((chat_input == '\b')||(payload_length == (aMaxPHYPacketSize-FRAME_OVERHEAD)))
	{

		uint8_t nodeDirection[3];
		//Obtaining the source address of the node
		nodeDirection[0]=(uint8_t)(SRC_ADDR >> 8);
		nodeDirection[1]=(uint8_t)SRC_ADDR;
		nodeDirection[2]=0xFF;                  
		sio2host_tx(nodeDirection,3);//Transmission of the source address to the application
		payload_length = 0;//Variable reset
		LED_On(LED1);
	}
}

//The usr_frame_received_cb function handles frame reception. It transmits the received data to the host, clears the previous reception structure, 
//copies the information from the received frame, and verifies if it belongs to the transmitter node. If so, it activates indicator LEDs, extracts 
//the frame's energy level, verifies the frame's size, and stores the data along with the energy level in a storage vector.

void usr_frame_received_cb(frame_info_t *frame)
{
	// Setting up the reception vector
	sio2host_tx(&(frame->mpdu[FRAME_OVERHEAD-FCS_LEN+1]),(frame->mpdu[0])-FRAME_OVERHEAD);
	memset(&received_frame,0,sizeof(received_frame));// Clear previous information from the reception structure
	memcpy(&received_frame,frame->mpdu,sizeof(received_frame));// Copy the information from the source memory (buffer) to the destination
	// Verify if the received data belongs to the transmitter
	if (received_frame.dir_source==0x0001 && (received_frame.payload_802_15_4[0]=='C'||received_frame.payload_802_15_4[0]=='L')) // Si se cumple esta condicion la trama es del nodo externo
	{
		LED_Toggle(LED0);
		LED_Off(LED1);
		LED_On(LED2);
		// Extraction of the energy level
		uint8_t *payload_ptr=frame->mpdu;
		uint8_t mpdu_len =payload_ptr[0]+2;
		uint8_t frame_power=payload_ptr[mpdu_len];
		received_frame.payload_802_15_4[14]=frame_power;
		// Verification of the frame size
		int longTrRx=((int)received_frame.length0)-11;
		if (longTrRx==23||longTrRx==100)
		{
			//Storage of each of the frames sent by the transmitter node along with its energy level
			for (int i = 0; i < longTrRx; i++)
			{
				storageVector[contSto+i]=received_frame.payload_802_15_4[i];
			}
			contSto=contSto+longTrRx;
		}
		//////////////////////////////////////////////////////////////////////////
	}
	//delay_ms(10);

}

//The usr_frame_transmitted_cb function is called when a frame has been transmitted.

void usr_frame_transmitted_cb(retval_t status, frame_info_t *frame)
{
}
