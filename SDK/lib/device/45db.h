/*
 * _45db.h
 *
 * Created: 1/1/2016 4:56:00 PM
 *  Author: John Smith
 */ 


#ifndef DEV45DB_H_
#define DEV45DB_H_

#include "api/mcspi_def.h"
#include "api/gpio_def.h"

#define DEVICE_45DB_PAGE_SIZE			528
#ifndef DEVICE_45DB_PAGES_NR
#define DEVICE_45DB_PAGES_NR			4096
#endif

#define DEVICE_45DB_RDY					7
#define DEVICE_45DB_COMP				6
#define DEVICE_45DB_CAPACITY			2
#define DEVICE_45DB_CAPACITY_MASK		(0xF<<2)
#define DEVICE_45DB_PROTECT				1

#define DEVICE_45DB_WRITE_TIMEOUT		1000000

//#####################################################
typedef struct DEV45db_Struct
{
	Mcspi_t* Spi_Struct;
	Gpio_t *WP_Port;
	Gpio_t *Reset_Port;
	unsigned char Id[5];
}dev45db_t;
//#####################################################

bool device_45db_init(dev45db_t *Settings, unsigned char Wp_PortNr, unsigned char Wp_PinNr, unsigned char Hold_PortNr, unsigned char Hold_PinNr);
bool device_45db_read_status(Mcspi_t *SpiStruct, char *status);
bool device_45db_enter_sleep(Mcspi_t *SpiStruct);
bool device_45db_resume_sleep(Mcspi_t *SpiStruct);
bool device_45db_write_buff(Mcspi_t *SpiStruct, unsigned char buff_nr, unsigned int page_nr, char *data);
bool device_45db_write_from_buffer(Mcspi_t *SpiStruct, unsigned char buff_nr, unsigned int page_nr);
bool device_45db_page_erase(Mcspi_t *SpiStruct, unsigned int page_nr);
bool device_45db_write(Mcspi_t *SpiStruct, unsigned char buff_nr, unsigned int page_nr, char *data);
bool device_45db_read(Mcspi_t *SpiStruct, unsigned int page_nr, char *data, unsigned int size);
//void device_45db_print_hex(Mcspi_t *SpiStruct, unsigned char data);
//bool device_45db_print(Mcspi_t *SpiStruct, unsigned int page_nr, unsigned int size);
bool device_45db_check_page_empty(Mcspi_t *SpiStruct, unsigned int page_nr);
bool device_45db_werify(Mcspi_t *SpiStruct, unsigned int page_nr, char *data, unsigned int size);
//#####################################################
#define new_dev45db dev45db_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_dev45db(address) free(address);

#endif /* 45DB_H_ */
