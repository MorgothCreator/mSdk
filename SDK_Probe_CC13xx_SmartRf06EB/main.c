/*
 * main.c
 */
#include <stdlib.h>

#include "board_init.h"
#include "RFQueue.h"

#include "api/gpio_api.h"
#include "api/gpio_def.h"
#include "api/uart_api.h"



#include "tx_smartrf_settings.h"
#include "driver/rf_mailbox.h"
#include "driver/rf_common_cmd.h"
#include "driver/rf_prop_cmd.h"
#include "driver/rfc.h"
#include "driver/osc.h"
#include "driver/prcm.h"

#include "driver/rf_prop_cmd.h"
#include "driver/rf_prop_mailbox.h"
#include "driver/rf_common_cmd.h"

#include "general/rf_patch_cpe_genfsk.h"

new_gpio* LED[4];

/* TX Configuration */
#define PAYLOAD_LENGTH      30

/* RX Configuration */
#define DATA_ENTRY_HEADER_SIZE 8  /* Constant header size of a Generic Data Entry */
#define MAX_LENGTH             100 /* Max length byte the radio will accept */
#define NUM_DATA_ENTRIES       2  /* NOTE: Only two data entries supported at the moment */
#define NUM_APPENDED_BYTES     2  /* The Data Entries data field will contain:
                                   * 1 Header byte (RF_cmdPropRx.rxConf.bIncludeHdr = 0x1)
                                   * Max 30 payload bytes
                                   * 1 status byte (RF_cmdPropRx.rxConf.bAppendStatus = 0x1) */

#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_ALIGN (rxDataEntryBuffer, 4);
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment = 4
#endif
uint8_t rxDataEntryBuffer[RF_QUEUE_DATA_ENTRY_BUFFER_SIZE(NUM_DATA_ENTRIES,
                                                                 MAX_LENGTH,
                                                                 NUM_APPENDED_BYTES)];
static rfc_dataEntryGeneral_t* currentDataEntry;
static rfc_propRxOutput_t rxStatistics;
static dataQueue_t dataQueue;
static uint8_t packetLength;
static uint8_t* packetDataPointer;
//static uint8_t packet[PAYLOAD_LENGTH];
static uint16_t seqNumber = 0;


static unsigned char send_buff[0xFF];

static rfc_CMD_FS_POWERUP_t RF_cmdFsPowerup =
{
	.commandNo = CMD_FS_POWERUP
};

static rfc_CMD_START_RAT_t RF_cmdStartRat =
{
	.commandNo = CMD_START_RAT
};

static rfc_CMD_ABORT_t RF_cmdAbort =
{
	.commandNo = CMD_ABORT
};

static rfc_CMD_STOP_t RF_cmdStop =
{
	.commandNo = 0x0402
};

static rfc_CMD_GET_RSSI_t RF_cmdGetRssi =
{
    .commandNo = 0x0403
};

const rfc_CMD_GET_FW_INFO_t RF_cmdGetFvInfo =
{
	.commandNo = CMD_GET_FW_INFO
};


static volatile rfc_CMD_RX_TEST_t rfc_CMD_RX_TEST;
static volatile rfc_CMD_TX_TEST_t rfc_CMD_TX_TEST;

static rfc_CMD_FS_POWERUP_t rfc_CMD_FS_POWERUP;
static rfc_CMD_START_RAT_t rfc_CMD_START_RAT;
static rfc_CMD_ABORT_t rfc_CMD_ABORT;
static rfc_CMD_STOP_t rfc_CMD_STOP;
static const rfc_CMD_TRIGGER_t rfc_CMD_TRIGGER;
static volatile rfc_CMD_GET_FW_INFO_t rfc_CMD_GET_FW_INFO;
static volatile rfc_CMD_PROP_RADIO_DIV_SETUP_t rfc_CMD_PROP_RADIO_DIV_SETUP;
static volatile rfc_CMD_FS_t rfc_CMD_FS;
static volatile rfc_CMD_PROP_TX_t rfc_CMD_PROP_TX;
static volatile rfc_CMD_PROP_RX_t rfc_CMD_PROP_RX;
static volatile rfc_CMD_PROP_TX_ADV_t rfc_CMD_PROP_TX_ADV;
static volatile rfc_CMD_PROP_RX_ADV_t rfc_CMD_PROP_RX_ADV;
static rfc_CMD_GET_RSSI_t rfc_CMD_GET_RSSI =
{
	.commandNo = 0x0403
};

uint32_t time;

typedef enum{
	trans_status_Ok = 0,
	trans_status_Timeout = -1,
	trans_status_Busy = -2,
	trans_status_Err = -3,
}trans_status_e;

int rf_abort();


#define USE_ADV_RX_TX	1

trans_status_e rf_send_pocket(unsigned char *buff, unsigned char len, unsigned char addr, unsigned long ui32Freq)
{
	if(/*rfc_CMD_PROP_RX.status == IDLE || */
			rfc_CMD_PROP_RX.status == PENDING ||
			rfc_CMD_PROP_RX.status == ACTIVE ||
			/*rfc_CMD_PROP_TX.status == IDLE || */
			rfc_CMD_PROP_TX.status == PENDING ||
			rfc_CMD_PROP_TX.status == ACTIVE)
		return trans_status_Busy;
	volatile int cnt = 0;
	volatile int result = 0;
	//if(rfc_CMD_FS.synthConf.bTxMode == 0)
	//{
		memcpy((rfc_CMD_FS_t *)&rfc_CMD_FS, &RF_cmdFs, sizeof(rfc_CMD_FS_t));
		rfc_CMD_FS.synthConf.bTxMode = 1;
		 /* Set the frequency */
		ui32Freq /= 1000;
		//rfc_CMD_FS.frequency = (uint16_t)(ui32Freq / 1000);
		//rfc_CMD_FS.fractFreq = ((ui32Freq - (rfc_CMD_FS.frequency * 1000)) * 65);
		result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_FS);
		if(result != 1)
			return trans_status_Err;
		for(cnt = 0; cnt < 2500; cnt++)
		{
			if(rfc_CMD_FS.status == DONE_OK)
				break;
		}
		if(cnt == 2500)
			return trans_status_Err;
	//}
	//for(cnt = 0; cnt < 250000; cnt++);
#if 1
	memcpy((rfc_CMD_PROP_TX_t *)&rfc_CMD_PROP_TX, &RF_cmdPropTx, sizeof(rfc_CMD_PROP_TX_t));
	rfc_CMD_PROP_TX.startTrigger.triggerType = TRIG_ABSTIME;
	rfc_CMD_PROP_TX.startTrigger.pastTrig = 1;
	rfc_CMD_PROP_TX.startTime = 0;
	memcpy(send_buff + 3, buff, len);
	send_buff[0] = addr;
	send_buff[1] = (uint8_t)(seqNumber >> 8);
	send_buff[2] = (uint8_t)(seqNumber);
	rfc_CMD_PROP_TX.pPkt = send_buff;
	rfc_CMD_PROP_TX.pktLen = len + 2;
	rfc_CMD_PROP_TX.pktConf.bFsOff = 0;
	result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_PROP_TX);
	if(result != 1)
		return trans_status_Err;
	for(cnt = 0; cnt < 2500000; cnt++)
	{
		if(rfc_CMD_PROP_TX.status == PROP_DONE_OK)
		{
			seqNumber++;
			return trans_status_Ok;
		}
	}
#else
	memcpy((rfc_CMD_PROP_TX_ADV_t *)&rfc_CMD_PROP_TX_ADV, &RF_cmdPropTx, sizeof(rfc_CMD_PROP_TX_ADV_t));
	rfc_CMD_PROP_TX_ADV.startTrigger.triggerType = TRIG_ABSTIME;
	rfc_CMD_PROP_TX_ADV.startTrigger.pastTrig = 1;
	rfc_CMD_PROP_TX_ADV.startTime = 0;
	memcpy(send_buff + 3, buff, len);
	send_buff[0] = addr;
	send_buff[1] = (uint8_t)(seqNumber >> 8);
	send_buff[2] = (uint8_t)(seqNumber);
	rfc_CMD_PROP_TX_ADV.pPkt = send_buff;
	rfc_CMD_PROP_TX_ADV.pktLen = len + 2;
	rfc_CMD_PROP_TX_ADV.pktConf.bFsOff = 0;
	result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_PROP_TX_ADV);
	if(result != 1)
		return trans_status_Err;
	for(cnt = 0; cnt < 2500000; cnt++)
	{
		if(rfc_CMD_PROP_TX_ADV.status == PROP_DONE_OK)
		{
			seqNumber++;
			return trans_status_Ok;
		}
	}
#endif
	return trans_status_Timeout;

}

trans_status_e rf_receive_packet(unsigned char *buff, unsigned char *len, unsigned char addr1, unsigned char addr2, uint32_t ui32Freq)
{
	if(/*rfc_CMD_PROP_RX.status == IDLE || */
			rfc_CMD_PROP_RX.status == PENDING ||
			rfc_CMD_PROP_RX.status == ACTIVE ||
			/*rfc_CMD_PROP_TX.status == IDLE || */
			rfc_CMD_PROP_TX.status == PENDING ||
			rfc_CMD_PROP_TX.status == ACTIVE)
		return trans_status_Busy;
	volatile int cnt = 0;
	volatile int result = 0;
	//if(rfc_CMD_FS.synthConf.bTxMode == 1)
	//{
		memcpy((rfc_CMD_FS_t *)&rfc_CMD_FS, &RF_cmdFs, sizeof(rfc_CMD_FS_t));
		rfc_CMD_FS.synthConf.bTxMode = 0;

		 /* Set the frequency */
		/*rfc_CMD_FS.frequency = (uint16_t)(ui32Freq / 1000000);
		rfc_CMD_FS.fractFreq = (uint16_t) (((uint64_t)ui32Freq -
		            (rfc_CMD_FS.frequency * 1000000)) * 65536 / 1000000);*/
		ui32Freq /= 1000;
		//rfc_CMD_FS.frequency = (uint16_t)(ui32Freq / 1000);
		//rfc_CMD_FS.fractFreq = ((ui32Freq - (rfc_CMD_FS.frequency * 1000)) * 65);
		result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_FS);
		if(result != 1)
			return trans_status_Err;
		for(cnt = 0; cnt < 2500; cnt++)
		{
			if(rfc_CMD_FS.status == DONE_OK)
				break;
		}
		if(cnt == 2500)
			return trans_status_Err;
	//}
#if 1
	memcpy((rfc_CMD_PROP_RX_t *)&rfc_CMD_PROP_RX, &RF_cmdPropRx, sizeof(rfc_CMD_PROP_RX_t));
	rfc_CMD_PROP_RX.pQueue = &dataQueue;
	rfc_CMD_PROP_RX.pOutput = (uint8_t*)&rxStatistics;
	rfc_CMD_PROP_RX.startTrigger.triggerType = TRIG_NOW;
	rfc_CMD_PROP_RX.pktConf.bFsOff = 0;
	rfc_CMD_PROP_RX.startTrigger.pastTrig = 1;
	rfc_CMD_PROP_RX.startTime = 0;
	rfc_CMD_PROP_RX.address0 = addr1;
	rfc_CMD_PROP_RX.address1 = addr2;
	rfc_CMD_PROP_RX.pktConf.bChkAddress = 1;
	result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_PROP_RX);
	if(result != 1)
		return trans_status_Err;
	for(cnt = 0; cnt < 1000000; cnt++)
	{
		if(rfc_CMD_PROP_RX.status >= PROP_DONE_OK && rfc_CMD_PROP_RX.status <= PROP_DONE_BUSYTIMEOUT)
		{
			/* Get current unhandled data entry */
			currentDataEntry = RFQueue_getDataEntry();
			/* Handle the packet data, located at &currentDataEntry->data:
			* - Length is the first byte with the current configuration
			* - Data starts from the second byte */
			packetLength      = *(uint8_t*)(&currentDataEntry->data);
			packetDataPointer = (uint8_t*)(&currentDataEntry->data + 1);
			/* Copy the payload + the status byte to the packet variable */
			memcpy(buff, packetDataPointer, (packetLength));
			*len = packetLength;

			RFQueue_nextEntry();
			return trans_status_Ok;
		}
	}
#else
	memcpy((rfc_CMD_PROP_RX_ADV_t *)&rfc_CMD_PROP_RX_ADV, &RF_cmdPropRx, sizeof(rfc_CMD_PROP_RX_ADV_t));
	rfc_CMD_PROP_RX_ADV.pQueue = &dataQueue;
	rfc_CMD_PROP_RX_ADV.pOutput = (uint8_t*)&rxStatistics;
	rfc_CMD_PROP_RX_ADV.startTrigger.triggerType = TRIG_NOW;
	rfc_CMD_PROP_RX_ADV.pktConf.bFsOff = 1;
	//rfc_CMD_PROP_RX.startTrigger.pastTrig = 1;
	rfc_CMD_PROP_RX_ADV.startTime = 0;
	//rfc_CMD_PROP_RX_ADV.address0 = addr1;
	//rfc_CMD_PROP_RX_ADV.address1 = addr2;
	//rfc_CMD_PROP_RX_ADV.pktConf.bChkAddress = 0;
	result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_PROP_RX_ADV);
	if(result != 1)
		return trans_status_Err;
	for(cnt = 0; cnt < 1000000; cnt++)
	{
		if(rfc_CMD_PROP_RX_ADV.status == PROP_DONE_OK)
		{
			/* Get current unhandled data entry */
			currentDataEntry = RFQueue_getDataEntry();
			/* Handle the packet data, located at &currentDataEntry->data:
			* - Length is the first byte with the current configuration
			* - Data starts from the second byte */
			packetLength      = *(uint8_t*)(&currentDataEntry->data);
			packetDataPointer = (uint8_t*)(&currentDataEntry->data + 1);
			/* Copy the payload + the status byte to the packet variable */
			memcpy(buff, packetDataPointer, (packetLength));
			*len = packetLength;

			RFQueue_nextEntry();
			return trans_status_Ok;
		}
	}
#endif
	memcpy(&rfc_CMD_GET_RSSI, &RF_cmdGetRssi, sizeof(rfc_CMD_GET_RSSI_t));
	result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_GET_RSSI);
	while(result < 3 && result >= 0);
	signed char rssi = result >> 16;
	UARTprintf(DebugCom, "RSSI = %d.\r\n" , (long) rssi);
	//*rssi = result;
	*len = 0;
	rf_abort();
	return trans_status_Timeout;
}

int rf_rssi(long *rssi)
{
	memcpy(&rfc_CMD_GET_RSSI, &RF_cmdGetRssi, sizeof(rfc_CMD_GET_RSSI_t));
	volatile int result = 0;
	result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_GET_RSSI);
	while(result < 3 && result >= 0);
	*rssi = result;
	return 0;
}

int rf_abort()
{
	memcpy(&rfc_CMD_ABORT, &RF_cmdAbort, sizeof(rfc_CMD_ABORT_t));
	return RFCDoorbellSendTo((unsigned long)&rfc_CMD_ABORT);
}
#define RX_MODE	0
#define TX_MODE	1
#define RX_TX_MODE	TX_MODE

int main(void) {
	OSCHF_TurnOnXosc();
	do{}while(!OSCHF_AttemptToSwitchToXosc());
	board_init();

	volatile int cnt = 0;
	volatile int result = 0;

	for(cnt = 0; cnt < 250000; cnt++);
	RFCClockEnable();//clocks bits are enabled properly
	for(cnt = 0; cnt < 250000; cnt++);
	PRCMPowerDomainOn(PRCM_DOMAIN_RFCORE | PRCM_DOMAIN_CPU | PRCM_DOMAIN_VIMS);

	//rf_patch_cpe_genfsk();

	memcpy(&rfc_CMD_START_RAT, &RF_cmdStartRat, sizeof(rfc_CMD_START_RAT_t));
	result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_START_RAT);
	//while(rfc_CMD_START_RAT.status < 3);
	for(cnt = 0; cnt < 250000; cnt++);

	//memcpy((rfc_CMD_GET_FW_INFO_t *)&rfc_CMD_GET_FW_INFO, &RF_cmdGetFvInfo, sizeof(rfc_CMD_GET_FW_INFO_t));
	//result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_GET_FW_INFO);
	//for(cnt = 0; cnt < 2500; cnt++);


	//rfc_CMD_FS_POWERUP_t
	//memcpy((rfc_CMD_FS_POWERUP_t *)&rfc_CMD_FS_POWERUP, &RF_cmdFsPowerup, sizeof(rfc_CMD_FS_POWERUP_t));
	//result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_FS_POWERUP);
	//while(result != 1);

	memcpy((rfc_CMD_PROP_RADIO_DIV_SETUP_t *)&rfc_CMD_PROP_RADIO_DIV_SETUP, &RF_cmdPropRadioDivSetup, sizeof(rfc_CMD_PROP_RADIO_DIV_SETUP_t));
	//rfc_CMD_PROP_RADIO_DIV_SETUP.symbolRate.preScale
	result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_PROP_RADIO_DIV_SETUP);
	while(rfc_CMD_PROP_RADIO_DIV_SETUP.status < 3);
	for(cnt = 0; cnt < 250000; cnt++);

	/*memcpy((rfc_CMD_FS_t *)&rfc_CMD_FS, &RF_cmdFs, sizeof(rfc_CMD_FS_t));
#if RX_TX_MODE == TX_MODE
	rfc_CMD_FS.synthConf.bTxMode = 1;
#else
	rfc_CMD_FS.synthConf.bTxMode = 0;
#endif
	result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_FS);
	while(rfc_CMD_FS.status < 3);
	//time = RF_getCurrentTime();*/

	/*memcpy((rfc_CMD_TX_TEST_t *)&rfc_CMD_TX_TEST, &RF_cmdTxTest, sizeof(rfc_CMD_TX_TEST_t));
	result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_TX_TEST);
	while(rfc_CMD_TX_TEST.status < 2);
	for(cnt = 0; cnt < 2500000; cnt++);*/
	//while(1);


	RFQueue_defineQueue(&dataQueue,  rxDataEntryBuffer, sizeof(rxDataEntryBuffer), NUM_DATA_ENTRIES, MAX_LENGTH + NUM_APPENDED_BYTES);

	//unsigned short FracFreq = 0;
	unsigned long Freq = 0x930b51de;
	Freq = 868000000;
	while(1)
	{
		/*for(cnt = 0; cnt < 2500000; cnt++);
		gpio_out(LED[0], 1);
		gpio_out(LED[1], 0);
		gpio_out(LED[2], 0);
		gpio_out(LED[3], 0);
		for(cnt = 0; cnt < 2500000; cnt++);
		gpio_out(LED[0], 0);
		gpio_out(LED[1], 1);
		gpio_out(LED[2], 0);
		gpio_out(LED[3], 0);
		for(cnt = 0; cnt < 2500000; cnt++);
		gpio_out(LED[0], 0);
		gpio_out(LED[1], 0);
		gpio_out(LED[2], 1);
		gpio_out(LED[3], 0);
		for(cnt = 0; cnt < 2500000; cnt++);
		gpio_out(LED[0], 0);
		gpio_out(LED[1], 0);
		gpio_out(LED[2], 0);
		gpio_out(LED[3], 1);*/
		if(gpio_in(PUSHBTN[0]))
			gpio_out(LED[0], 1);
		else
			gpio_out(LED[0], 0);

		if(gpio_in(PUSHBTN[1]))
			gpio_out(LED[1], 1);
		else
			gpio_out(LED[1], 0);

		if(gpio_in(PUSHBTN[2]))
			gpio_out(LED[2], 1);
		else
			gpio_out(LED[2], 0);

		if(gpio_in(PUSHBTN[3]))
			gpio_out(LED[3], 1);
		else
			gpio_out(LED[3], 0);


		//long rssi;
		//rf_rssi(&rssi);
		//UARTprintf(DebugCom, "RSSI = %d.\n\r" , rssi);
#if RX_TX_MODE == TX_MODE
		for(cnt = 0; cnt < 250000; cnt++);
		if(rf_send_pocket((unsigned char *)"CutareCutareCutareCutareCutareCutareCutareCutareCutare", sizeof("CutareCutareCutareCutareCutareCutareCutareCutareCutare"), 0xAA, Freq) == trans_status_Ok)
			UARTprintf(DebugCom, "Packet send %u\r\n", Freq);
		else
			UARTPuts(DebugCom, "ERROR sending packet\r\n", -1);
		//rfc_CMD_PROP_TX.syncWord = Freq;
		//Freq += 100;
#endif
#if RX_TX_MODE == RX_MODE
		unsigned char buff[0xFF];
		unsigned char len;
		if(rf_receive_packet(buff, &len, 0xaa, 0xbb, 868000000) == trans_status_Ok)
		{
			UARTPuts(DebugCom, (char *)buff, len);
			UARTprintf(DebugCom, "RSSI = %d, " , rxStatistics.lastRssi);
			if(rxStatistics.nRxIgnored)
				UARTPuts(DebugCom, "Packet ignored, ", -1);
			else
				UARTPuts(DebugCom, "Packet non ignored, ", -1);
			if(rxStatistics.nRxNok)
				UARTPuts(DebugCom, "Packet Nok ", -1);
			else if(rxStatistics.nRxOk)
				UARTPuts(DebugCom, "Packet Ok ", -1);
			UARTPuts(DebugCom, "\r\n", -1);
		}
		else
			rf_abort();
		Freq += 100;
#endif
	}
	return 0;
}
