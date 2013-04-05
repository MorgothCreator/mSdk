/*
 * smc.h
 *
 *  Created on: Jan 18, 2013
 *      Author: XxXx
 */

#ifndef SMC_H_
#define SMC_H_

#define PL310_POR	5

/* PL310 */
#define OMAP44XX_PL310_BASE		0x48242000

/* PL310 */
#define OMAP44XX_PL310_AUX_CTRL		(OMAP44XX_PL310_BASE + 0x104)

/* Defines for ROM Services */
/* PL310 power on reset */
#define ROM_SERVICE_PL310_AUXCR_SVC	0x109
#define ROM_SERVICE_PL310_POR_SVC	0x113
/*
 * Define PPA provided Services here PPA_SERVICE_xyz
 * IMPORTANT: PPA service may change based on PPA used,
 * It may be better to define and use them in board files instead
 * Define only official TI *REQUIRED* PPA services here.
 */
#define PPA_SERVICE_PL310_POR		0x23

/* Call romcode service */
extern unsigned int omap_smc_rom(unsigned int rom_svc,
		unsigned int arg);

/**
 * omap_smc_ppa() - Entry to ROM code's routine Pub2SecDispatcher.
 * @appl_id:	HAL Service number
 * @proc_id:	for ppa services usually 0.
 * @flag:	service priority
 * @...pargs:	Depending on the PPA service used.
 *
 * This routine manages the entry to secure HAL API.
 * XXX: Use only with MMU disabled!
 */
typedef unsigned int (** const __secure_pub_to_sec_dispatch)
	(unsigned int appl_id, unsigned int proc_id, unsigned int flag, ...);
#define omap_smc_ppa (*(__secure_pub_to_sec_dispatch) 0x00028400)

#endif /* SMC_H_ */
