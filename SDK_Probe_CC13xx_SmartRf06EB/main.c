/*
 * main.c
 */
#include <stdlib.h>

#include "board_init.h"

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

new_gpio* LED[4];

/* TX Configuration */
#define PAYLOAD_LENGTH      30

//static uint8_t packet[PAYLOAD_LENGTH];
static uint16_t seqNumber = 0;
static dataQueue_t Queue;
static unsigned char send_buff[0xFF];
static unsigned char rec_buff[0xFF];

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

static rfc_CMD_GET_RSSI_t RF_cmdGatRssi =
{
    .commandNo = 0x0403
};

const rfc_CMD_GET_FW_INFO_t RF_cmdGetFvInfo =
{
	.commandNo = CMD_GET_FW_INFO
};
static rfc_CMD_START_RAT_t rfc_CMD_START_RAT;
static rfc_CMD_ABORT_t rfc_CMD_ABORT;
static rfc_CMD_STOP_t rfc_CMD_STOP;
static const rfc_CMD_TRIGGER_t rfc_CMD_TRIGGER;
static volatile rfc_CMD_GET_FW_INFO_t rfc_CMD_GET_FW_INFO;
static volatile rfc_CMD_PROP_RADIO_DIV_SETUP_t rfc_CMD_PROP_RADIO_DIV_SETUP;
static volatile rfc_CMD_FS_t rfc_CMD_FS;
static volatile rfc_CMD_PROP_TX_t rfc_CMD_PROP_TX;
static volatile rfc_CMD_PROP_RX_t rfc_CMD_PROP_RX;
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


trans_status_e rf_send_pocket(unsigned char *buff, unsigned char len)
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
	if(rfc_CMD_FS.synthConf.bTxMode == 0)
	{
		memcpy((rfc_CMD_FS_t *)&rfc_CMD_FS, &RF_cmdFs, sizeof(rfc_CMD_FS_t));
		rfc_CMD_FS.synthConf.bTxMode = 1;
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
	}

	memcpy((rfc_CMD_PROP_TX_t *)&rfc_CMD_PROP_TX, &RF_cmdPropTx, sizeof(rfc_CMD_PROP_TX_t));
	rfc_CMD_PROP_TX.startTrigger.triggerType = TRIG_ABSTIME;
	rfc_CMD_PROP_TX.startTrigger.pastTrig = 1;
	rfc_CMD_PROP_TX.startTime = 0;
	memcpy(send_buff + 2, buff, len);
	send_buff[0] = (uint8_t)(seqNumber >> 8);
	send_buff[1] = (uint8_t)(seqNumber);
	rfc_CMD_PROP_TX.pPkt = send_buff;
	rfc_CMD_PROP_TX.pktLen = len + 2;
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
	return trans_status_Timeout;

}

trans_status_e rf_receive_packet(unsigned char *buff, long *rssi)
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
	if(rfc_CMD_FS.synthConf.bTxMode == 1)
	{
		memcpy((rfc_CMD_FS_t *)&rfc_CMD_FS, &RF_cmdFs, sizeof(rfc_CMD_FS_t));
		rfc_CMD_FS.synthConf.bTxMode = 0;
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
	}
	memcpy((rfc_CMD_PROP_RX_t *)&rfc_CMD_PROP_RX, &RF_cmdPropRx, sizeof(rfc_CMD_PROP_RX_t));
	rfc_CMD_PROP_RX.pQueue = &Queue;
	rfc_CMD_PROP_RX.pOutput = rec_buff;
	/*rfc_CMD_PROP_RX.startTrigger.triggerType = TRIG_ABSTIME;
	rfc_CMD_PROP_RX.startTrigger.pastTrig = 1;
	rfc_CMD_PROP_RX.startTime = 0;*/
	result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_PROP_RX);
	if(result != 1)
		return trans_status_Err;
	for(cnt = 0; cnt < 250000; cnt++)
	{
		if(rfc_CMD_PROP_RX.status == DONE_OK)
		{
			memcpy(buff, rec_buff, 0xFF);
			return trans_status_Ok;
		}
	}
	memcpy(&rfc_CMD_GET_RSSI, &RF_cmdGatRssi, sizeof(rfc_CMD_GET_RSSI_t));
	result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_GET_RSSI);
	while(result < 3 && result >= 0);
	*rssi = result;
	return trans_status_Timeout;
}

int rf_abort()
{
	memcpy(&rfc_CMD_ABORT, &RF_cmdAbort, sizeof(rfc_CMD_ABORT_t));
	return RFCDoorbellSendTo((unsigned long)&rfc_CMD_ABORT);
}

int main(void) {
	board_init();
	OSCHF_TurnOnXosc();
	do{}while(!OSCHF_AttemptToSwitchToXosc());

	volatile int cnt = 0;
	volatile int result = 0;

	for(cnt = 0; cnt < 250000; cnt++);
	RFCClockEnable();//clocks bits are enabled properly
	for(cnt = 0; cnt < 250000; cnt++);
	PRCMPowerDomainOn(PRCM_DOMAIN_RFCORE);

	memcpy(&rfc_CMD_START_RAT, &RF_cmdStartRat, sizeof(rfc_CMD_START_RAT_t));
	result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_START_RAT);
	//while(rfc_CMD_START_RAT.status < 3);
	for(cnt = 0; cnt < 250000; cnt++);

	memcpy((rfc_CMD_GET_FW_INFO_t *)&rfc_CMD_GET_FW_INFO, &RF_cmdGetFvInfo, sizeof(rfc_CMD_GET_FW_INFO_t));
	result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_GET_FW_INFO);
	for(cnt = 0; cnt < 2500; cnt++);

	memcpy((rfc_CMD_PROP_RADIO_DIV_SETUP_t *)&rfc_CMD_PROP_RADIO_DIV_SETUP, &RF_cmdPropRadioDivSetup, sizeof(rfc_CMD_PROP_RADIO_DIV_SETUP_t));
	result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_PROP_RADIO_DIV_SETUP);
	while(rfc_CMD_PROP_RADIO_DIV_SETUP.status < 3);

	memcpy((rfc_CMD_FS_t *)&rfc_CMD_FS, &RF_cmdFs, sizeof(rfc_CMD_FS_t));
	rfc_CMD_FS.synthConf.bTxMode = 0;
	result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_FS);
	while(rfc_CMD_FS.status < 3);
	//time = RF_getCurrentTime();




	/*memcpy(&rfc_CMD_TX_TEST, &RF_cmdTxTest, sizeof(rfc_CMD_TX_TEST_t));
	result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_TX_TEST);
	for(cnt = 0; cnt < 2500; cnt++);
	UARTprintf(DebugCom, "TX response = %d.\n\r" , (unsigned long)rfc_CMD_TX_TEST.status);*/

	//UARTPuts(DebugCom, &buff, PAYLOAD_LENGTH);

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
//		for(cnt = 0; cnt < 2500000; cnt++);

		//rf_send_pocket((unsigned char *)"Cutare", sizeof("Cutare"));
		unsigned char buff[0xFF];
		long rssi;
		if(rf_receive_packet(buff, &rssi) == trans_status_Ok)
		{
			UARTPuts(DebugCom, (char *)buff, 0xFF);
			UARTprintf(DebugCom, "RSSI = %d.\n\r" , rssi);
		}
		else
			rf_abort();
		UARTprintf(DebugCom, "RSSI = %d.\n\r" , rssi);
		/*for(cnt = 0; cnt < 2500; cnt++);
		memcpy(&rfc_CMD_GET_RSSI, &RF_cmdGatRssi, sizeof(rfc_CMD_GET_RSSI_t));
		result= RFCDoorbellSendTo((unsigned long)&rfc_CMD_GET_RSSI);
		while(result < 3 && result >= 0);
		UARTprintf(DebugCom, "RSSI = %d.\n\r" , result);*/
	}
	return 0;
}
