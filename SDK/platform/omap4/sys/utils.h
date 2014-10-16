/*
 * (C) Copyright 2010
 * Texas Instruments, <www.ti.com>
 * Aneesh V <aneesh@ti.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef _UTILS_H_
#define _UTILS_H_

static inline signed long log_2_n_round_up(unsigned long n)
{
	signed long log2n = -1;
	unsigned long temp = n;

	while (temp) {
		log2n++;
		temp >>= 1;
	}

	if (n & (n - 1))
		return log2n + 1; /* not power of 2 - round up */
	else
		return log2n; /* power of 2 */
}

static inline signed long log_2_n_round_down(unsigned long n)
{
	signed long log2n = -1;
	unsigned long temp = n;

	while (temp) {
		log2n++;
		temp >>= 1;
	}

	return log2n;
}

#endif
