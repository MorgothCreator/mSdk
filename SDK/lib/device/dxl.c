#include "stdbool.h"
#include "string.h"
#include "stdlib.h"
#include "dxl.h"
#include "board_init.h"
#include "api/timer_api.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "api/uart_def.h"
#include "api/uart_api.h"

#define ID					(2)
#define LENGTH				(3)
#define INSTRUCTION			(4)
#define ERRBIT				(4)
#define PARAMETER			(5)
#define DEFAULT_BAUDNUMBER	(1)

void dxl_insert_checksum(char *buff)
{
	unsigned char chk = 0;
	unsigned char cnt = 0;
	for(; cnt < buff[LENGTH] + 1; cnt ++)
		chk += buff[cnt + 2];
	buff[buff[LENGTH] + 3] = ~chk;
}

bool dxl_verify_checksum(char *buff)
{
	unsigned char chk = 0;
	unsigned char cnt = 0;
	for(; cnt < buff[LENGTH] + 1; cnt ++)
		chk += buff[cnt + 2];
	chk = ~chk;
	unsigned char buff_chk = buff[buff[LENGTH] + 3];
	if(buff_chk == chk) return true;
	else return false;
}

DXL_COMM_ERR dxl_ping(DXL_ACTUATOR_t *settings, unsigned char id, unsigned char* err) {
	char tx_buff[6];
	tx_buff[0] = 0xFF;
	tx_buff[1] = 0xFF;
	tx_buff[2] = id;
	tx_buff[3] = 2;
	tx_buff[4] = DXL_PING;
	tx_buff[5] = 0x0;
	dxl_insert_checksum(tx_buff);
	gpio.out(settings->TxEnGpio, 1);
	uart.puts(settings->Uart, tx_buff, tx_buff[3] + 4);
	gpio.out(settings->TxEnGpio, 0);
	unsigned char rx_cnt = 0;
	signed short rx_char;
	char rx_buff[150];
	memset(rx_buff, 0, 150);
	bool preamble_ok = false;
	DXL_COMM_ERR errors = DXL_COMM_RXTIMEOUT;
	timer(timeout_timer);
	timer_interval(&timeout_timer, settings->timeout);
	while(1) {
		timer_enable(&timeout_timer);
		while(1) {
			rx_char = uart.getc_no_blocking(settings->Uart);
			if(rx_char != (signed short)-1) {
				if(rx_char == 0xFF && preamble_ok == false && rx_cnt < 2) {
					rx_buff[rx_cnt] = rx_char;
					rx_cnt++;
					break;
				}
				if(rx_char != 0xFF && rx_cnt == 1 && preamble_ok == false)
					rx_cnt = 0;
				if(rx_char != 0xFF && rx_cnt == 2 && preamble_ok == false)
					preamble_ok = true;
				if(preamble_ok == true) break;
			}
			if(timer_tick(&timeout_timer)) {
				return errors;
			}
		}
		if(preamble_ok) {
			if(rx_cnt < 56) rx_buff[rx_cnt++] = rx_char;
			else
				errors = DXL_COMM_OVERFLOW;
			if(rx_buff[LENGTH] > 54) {
				errors = DXL_COMM_RXCORRUPT;
			}
			if(rx_cnt > 3) {
				if(rx_cnt - 3 >= rx_buff[3]) {
					if(dxl_verify_checksum(rx_buff)) {
						*err = rx_buff[4];
						return DXL_COMM_SUCCESS;
					} else {
						errors = DXL_COMM_CHECKSUM;
					}
				}
			}
		}
	}
}

/*
 * settings = structure that contain the uart and TXEN gpio structure settings.
 * buff     = data to be writen.
 * id       = DXL actuator ID.
 * reg      = pointed register from where begin to write the buff data.
 * write_nr = number of bytes to be read.
 */
DXL_COMM_ERR dxl_read(DXL_ACTUATOR_t *settings, unsigned char* buff, unsigned char id, unsigned char reg, unsigned char read_nr, unsigned char *dxl_err) {
	if(read_nr > 50 || read_nr + reg > 50 || id == 255) return false;
	char tx_buff[8];
	tx_buff[0] = 0xFF;
	tx_buff[1] = 0xFF;
	tx_buff[2] = id;
	tx_buff[3] = 4;
	tx_buff[4] = DXL_READ_DATA;
	tx_buff[5] = reg;
	tx_buff[6] = read_nr;
	tx_buff[7] = 0x0;
	dxl_insert_checksum(tx_buff);
	gpio.out(settings->TxEnGpio, 1);
	uart.puts(settings->Uart, tx_buff, tx_buff[3] + 4);
	gpio.out(settings->TxEnGpio, 0);
	unsigned char rx_cnt = 0;
	signed short rx_char = 0;
	char rx_buff[56];
	memset(rx_buff, 0, 56);
	bool preamble_ok = false;
	DXL_COMM_ERR errors = DXL_COMM_RXTIMEOUT;
	timer(timeout_timer);
	timer_interval(&timeout_timer, settings->timeout);
	while(1) {
		timer_enable(&timeout_timer);
		while(1) {
			rx_char = uart.getc_no_blocking(settings->Uart);
			if(rx_char != (signed short)-1) {
				if(rx_char == 0xFF && preamble_ok == false && rx_cnt < 2) {
					rx_buff[rx_cnt] = rx_char;
					rx_cnt++;
					break;
				}
				if(rx_char != 0xFF && rx_cnt == 1 && preamble_ok == false)
					rx_cnt = 0;
				if(rx_char != 0xFF && rx_cnt == 2 && preamble_ok == false)
					preamble_ok = true;
				if(preamble_ok == true) break;
			}
			if(timer_tick(&timeout_timer)) {
				return errors;
			}
		}
		if(preamble_ok) {
			if(rx_cnt < 56) rx_buff[rx_cnt++] = rx_char;
			else
				errors = DXL_COMM_OVERFLOW;
			if(rx_cnt > 3 && rx_cnt != (unsigned char)-1) {
				if(rx_buff[LENGTH] > 54) {
					errors = DXL_COMM_RXCORRUPT;
				}
				if(rx_cnt - 4 == rx_buff[LENGTH]) {
					//UARTPuts(DebugCom, rx_buff, rx_buff[LENGTH] + 4);
					if(dxl_verify_checksum(rx_buff)) {
						memcpy(buff, rx_buff + 5, read_nr);
						*dxl_err = rx_buff[4];
						return DXL_COMM_SUCCESS;
					} else {
						errors = DXL_COMM_CHECKSUM;
					}
				}
			}
		}
	}
}

static DXL_COMM_ERR dxl_write(DXL_ACTUATOR_t *settings, unsigned char comand_type, unsigned char id, unsigned char reg, unsigned char* send_data, unsigned char write_nr, unsigned char *aditional_info, unsigned char *aditional_info_len, unsigned char *dxl_err) {
	if(write_nr > 50 || write_nr + reg > 50 || id == 255) return false;
	char *tx_buff = malloc(write_nr + 7);
	if(!tx_buff) return DXL_COMM_BUFF_ALLOC;
	tx_buff[0] = 0xFF;
	tx_buff[1] = 0xFF;
	tx_buff[2] = id;
	if(write_nr) {
		tx_buff[3] = write_nr + 3;
		tx_buff[4] = comand_type;
		tx_buff[5] = reg;
		memcpy(tx_buff + 6, send_data, write_nr);
		dxl_insert_checksum(tx_buff);
		gpio.out(settings->TxEnGpio, 1);
		uart.puts(settings->Uart, tx_buff, tx_buff[3] + 7);
		gpio.out(settings->TxEnGpio, 0);
	} else {
		tx_buff[3] = 2;
		tx_buff[4] = comand_type;
		dxl_insert_checksum(tx_buff);
		gpio.out(settings->TxEnGpio, 1);
		uart.puts(settings->Uart, tx_buff, 6);
		gpio.out(settings->TxEnGpio, 0);
	}
	free(tx_buff);
	unsigned char rx_cnt = 0;
	signed short rx_char = 0;
	char rx_buff[54];
	memset(rx_buff, 0, 54);
	bool preamble_ok = false;
	DXL_COMM_ERR errors = DXL_COMM_RXTIMEOUT;
	timer(timeout_timer);
	timer_interval(&timeout_timer, settings->timeout);
	while(1) {
		timer_enable(&timeout_timer);
		while(1) {
			rx_char = uart.getc_no_blocking(settings->Uart);
			if(rx_char != (signed short)-1) {
				if(rx_char == 0xFF && preamble_ok == false && rx_cnt < 2) {
					rx_buff[rx_cnt] = rx_char;
					rx_cnt++;
					break;
				}
				if(rx_char != 0xFF && rx_cnt == 1 && preamble_ok == false)
					rx_cnt = 0;
				if(rx_char != 0xFF && rx_cnt == 2 && preamble_ok == false)
					preamble_ok = true;
				if(preamble_ok == true) break;
			}
			if(timer_tick(&timeout_timer)) {
				return errors;
			}
		}
		if(rx_cnt < 56) rx_buff[rx_cnt++] = rx_char;
		else
			errors = DXL_COMM_OVERFLOW;
		if(rx_cnt > 3 && rx_cnt != (unsigned char)-1) {
			if(rx_buff[LENGTH] > 52)
				errors = DXL_COMM_RXCORRUPT;
			if(rx_cnt - 4 == rx_buff[LENGTH]) {
				if(dxl_verify_checksum(rx_buff)) {
					if(rx_cnt - 6) memcpy(aditional_info, rx_buff + 5, rx_cnt - 6);
					*dxl_err = rx_buff[4];
					*aditional_info_len = rx_cnt - 6;
					return DXL_COMM_SUCCESS;
				} else
					errors = DXL_COMM_CHECKSUM;
			}
		}
	}
}

/*
 * settings = structure that contain the uart and TXEN gpio structure settings.
 * id       = DXL actuator ID.
 * reg      = pointed register from where begin to write the buff data.
 * send_data     = data to be send.
 * write_nr = number of bytes to be send.
 * aditional_info = buffer to receive additional info from DXL.
 * aditional_info_len = return number of bytes on aditional_info buffer.
 * dxl_err = the error byte received from DXL.
 */
DXL_COMM_ERR dxl_write_data(DXL_ACTUATOR_t *settings, unsigned char id, unsigned char reg, unsigned char* send_data, unsigned char write_nr, unsigned char *aditional_info, unsigned char *aditional_info_len, unsigned char *dxl_err) {
	return dxl_write(settings, DXL_WRITE_DATA, id, reg, send_data, write_nr, aditional_info, aditional_info_len, dxl_err);
}

/*
 * settings = structure that contain the uart and TXEN gpio structure settings.
 * id       = DXL actuator ID.
 * reg      = pointed register from where begin to write the buff data.
 * send_data     = data to be send.
 * write_nr = number of bytes to be send.
 * aditional_info = buffer to receive additional info from DXL.
 * aditional_info_len = return number of bytes on aditional_info buffer.
 * dxl_err = the error byte received from DXL.
 */
DXL_COMM_ERR dxl_reg_write(DXL_ACTUATOR_t *settings, unsigned char id, unsigned char reg, unsigned char* send_data, unsigned char write_nr, unsigned char *aditional_info, unsigned char *aditional_info_len, unsigned char *dxl_err) {
	return dxl_write(settings, DXL_REG_WRITE, id, reg, send_data, write_nr, aditional_info, aditional_info_len, dxl_err);
}

/*
 * settings = structure that contain the uart and TXEN gpio structure settings.
 * id       = DXL actuator ID.
 * dxl_err = the error byte received from DXL.
 */
DXL_COMM_ERR dxl_reset(DXL_ACTUATOR_t *settings, unsigned char id, unsigned char *dxl_err) {
	unsigned char aditional_info[54];
	unsigned char aditional_info_len;
	return dxl_write(settings, DXL_RESET, id, 0, NULL, 0, aditional_info, &aditional_info_len, dxl_err);
}

/*
 * settings = structure that contain the uart and TXEN gpio structure settings.
 */
DXL_COMM_ERR dxl_action(DXL_ACTUATOR_t *settings) {
	char tx_buff[6];
	tx_buff[0] = 0xFF;
	tx_buff[1] = 0xFF;
	tx_buff[2] = 0xFE;
	tx_buff[3] = 2;
	tx_buff[4] = DXL_ACTION;
	dxl_insert_checksum(tx_buff);
	gpio.out(settings->TxEnGpio, 1);
	uart.puts(settings->Uart, tx_buff, 6);
	gpio.out(settings->TxEnGpio, 0);
	return DXL_COMM_SUCCESS;
}

/*
 * settings = structure that contain the uart and TXEN gpio structure settings.
 * id       = DXL actuator ID.
 * reg      = pointed register from where begin to write the buff data.
 * send_data_table     = table of pockets data to be send.
 * write_nr = number of bytes on each packet.
 * nr_of_pockets = number of pockets to be send.
 */
DXL_COMM_ERR dxl_synk_write(DXL_ACTUATOR_t *settings, unsigned char reg, DXL_SYNK_IND_PACKET_t *send_data_table, unsigned char write_nr, unsigned char nr_of_pockets) {
	if(write_nr > 50 || write_nr == 0 || write_nr + reg > 50) return DXL_COMM_ERROR;
	char *tx_buff = malloc((write_nr + 1) * nr_of_pockets + 7);
	if(!tx_buff) return DXL_COMM_BUFF_ALLOC;
	tx_buff[0] = 0xFF;
	tx_buff[1] = 0xFF;
	tx_buff[2] = 0xFE;
	tx_buff[3] = ((write_nr + 1) * nr_of_pockets) + 4;
	tx_buff[4] = DXL_SYNC_WRITE;
	tx_buff[5] = reg;
	tx_buff[6] = write_nr;
	unsigned char tx_buff_cnt = 7;
	unsigned char pocket_cnt = 0;
	for(; pocket_cnt < nr_of_pockets; pocket_cnt++) {
		tx_buff[tx_buff_cnt++] = send_data_table[pocket_cnt].id;
		unsigned char data_cnt = 0;
		for(; data_cnt < write_nr; data_cnt++) {
			if(tx_buff_cnt > 254) {
				return DXL_COMM_OVERFLOW;
			}
			tx_buff[tx_buff_cnt++] = send_data_table[pocket_cnt].data[data_cnt];
		}
	}
	tx_buff_cnt++;
	dxl_insert_checksum(tx_buff);
	gpio.out(settings->TxEnGpio, 1);
	uart.puts(settings->Uart, tx_buff, tx_buff_cnt);
	gpio.out(settings->TxEnGpio, 0);
	free(tx_buff);
	return DXL_COMM_SUCCESS;
}

