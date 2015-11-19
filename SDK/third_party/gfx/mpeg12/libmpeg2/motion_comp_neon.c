/*
 * motion_comp_neon.c
 * Copyright (C) 2009 Rémi Denis-Courmont
 *
 * This file is part of mpeg2dec, a free MPEG-2 video stream decoder.
 * See http://libmpeg2.sourceforge.net/ for updates.
 *
 * mpeg2dec is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * mpeg2dec is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with mpeg2dec; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "../config.h"

#ifdef ARCH_ARM_NEON

#include <stdint.h>
#include <string.h>

#include "../include/mpeg2.h"
#include "../include/attributes.h"
#include "mpeg2_internal.h"

/* dest = ref */
static void MC_put_o_16_neon (uint8_t * dest, const uint8_t * ref,
			      const int stride, int height)
{
    do {
	memcpy (dest, ref, 16);
	ref += stride;
	dest += stride;
    } while (--height);
}

static void MC_put_o_8_neon (uint8_t * dest, const uint8_t * ref,
			     const int stride, int height)
{
    do {
	memcpy (dest, ref, 8);
	ref += stride;
	dest += stride;
    } while (--height);
}

/* dest = (src1 + src2 + 1) / 2 */
static void MC_avg_1_16_neon (uint8_t * dest, const uint8_t * src1,
			      const uint8_t * src2,
			      const int stride, unsigned height)
{
    do {
	asm volatile (
	    "vld1.u8 {q0}, [%[src1]]\n"
	    "vld1.u8 {q1}, [%[src2]]\n"
	    "vrhadd.u8 q0, q0, q1\n"
	    /* XXX: three cycles stall */
	    "vst1.u8 {q0}, [%[dest]]\n"
	    :
	    : [dest]"r"(dest), [src1]"r"(src1), [src2]"r"(src2)
	    : "memory", "q0", "q1");
	src1 += stride;
	src2 += stride;
	dest += stride;
    } while (--height);
}

static void MC_avg_1_8_neon (uint8_t * dest, const uint8_t * src1,
			     const uint8_t * src2,
			     const int stride, unsigned height)
{
    do {
	asm volatile (
	    "vld1.u8 {d0}, [%[src1]]\n"
	    "vld1.u8 {d1}, [%[src2]]\n"
	    "vrhadd.u8 d0, d0, d1\n"
	    "vst1.u8 {d0}, [%[dest]]\n"
	    :
	    : [dest]"r"(dest), [src1]"r"(src1), [src2]"r"(src2)
	    : "memory", "q0");
	
	src1 += stride;
	src2 += stride;
	dest += stride;
    } while (--height);
}

/* dest = (dest + ((src1 + src2 + 1) / 2) + 1) / 2 */
static void MC_avg_2_16_neon (uint8_t * dest, const uint8_t * src1,
			      const uint8_t * src2,
			      const int stride, unsigned height)
{
    do {
	asm volatile (
	    "vld1.u8 {q0}, [%[src1]]\n"
	    "vld1.u8 {q1}, [%[src2]]\n"
	    "vrhadd.u8 q0, q0, q1\n"
	    "vld1.u8 {q2}, [%[dest]]\n"
	    /* XXX: one cycle stall */
	    "vrhadd.u8 q0, q0, q2\n"
	    /* XXX: three cycles stall */
	    "vst1.u8 {q0}, [%[dest]]\n"
	    :
	    : [dest]"r"(dest), [src1]"r"(src1), [src2]"r"(src2)
	    : "memory", "q0", "q1", "q2");
	src1 += stride;
	src2 += stride;
	dest += stride;
    } while (--height);
}

static void MC_avg_2_8_neon (uint8_t * dest, const uint8_t * src1,
			     const uint8_t * src2,
			     const int stride, unsigned height)
{
    do {
	asm volatile (
	    "vld1.u8 {d0}, [%[src1]]\n"
	    "vld1.u8 {d1}, [%[src2]]\n"
	    "vrhadd.u8 d0, d0, d1\n"
	    "vld1.u8 {d2}, [%[dest]]\n"
	    "vrhadd.u8 d0, d0, d2\n"
	    "vst1.u8 {d0}, [%[dest]]\n"
	    :
	    : [dest]"r"(dest), [src1]"r"(src1), [src2]"r"(src2)
	    : "memory", "q0", "d2");
	src1 += stride;
	src2 += stride;
	dest += stride;
    } while (--height);
}

static void MC_avg_o_16_neon (uint8_t * dest, const uint8_t * ref,
			      const int stride, int height)
{
    MC_avg_1_16_neon (dest, dest, ref, stride, height);
}

static void MC_avg_o_8_neon (uint8_t * dest, const uint8_t * ref,
			     const int stride, int height)
{
    MC_avg_1_8_neon (dest, dest, ref, stride, height);
}

static void MC_put_x_16_neon (uint8_t * dest, const uint8_t * ref,
			      const int stride, int height)
{
    MC_avg_1_16_neon (dest, ref, ref + 1, stride, height);
}

static void MC_put_x_8_neon (uint8_t * dest, const uint8_t * ref,
			     const int stride, int height)
{
    MC_avg_1_8_neon (dest, ref, ref + 1, stride, height);
}

static void MC_avg_x_16_neon (uint8_t * dest, const uint8_t * ref,
			      const int stride, int height)
{
    MC_avg_2_16_neon (dest, ref, ref + 1, stride, height);
}

static void MC_avg_x_8_neon (uint8_t * dest, const uint8_t * ref,
			     const int stride, int height)
{
    MC_avg_2_8_neon (dest, ref, ref + 1, stride, height);
}

static void MC_put_y_16_neon (uint8_t * dest, const uint8_t * ref,
			      const int stride, int height)
{
    MC_avg_1_16_neon (dest, ref, ref + stride, stride, height);
}
static void MC_put_y_8_neon (uint8_t * dest, const uint8_t * ref,
			     const int stride, int height)
{
    MC_avg_1_8_neon (dest, ref, ref + stride, stride, height);
}

static void MC_avg_y_16_neon (uint8_t * dest, const uint8_t * ref,
			      const int stride, int height)
{
    MC_avg_2_16_neon (dest, ref, ref + stride, stride, height);
}

static void MC_avg_y_8_neon (uint8_t * dest, const uint8_t * ref,
			     const int stride, int height)
{
    MC_avg_2_8_neon (dest, ref, ref + stride, stride, height);
}

static void MC_put_xy_16_neon (uint8_t * dest, const uint8_t * ref,
			       const int stride, int height)
{
    do {
	asm volatile (
	    "vld1.u8 {q0}, [%[ref]]\n"
	    "vld1.u8 {q1}, [%[refx]]\n"
	    "vrhadd.u8 q0, q0, q1\n"
	    "vld1.u8 {q2}, [%[refy]]\n"
	    "vld1.u8 {q3}, [%[refxy]]\n"
	    "vrhadd.u8 q2, q2, q3\n"
	    /* XXX: three cycles stall */
	    "vrhadd.u8 q0, q0, q2\n"
	    /* XXX: three cycles stall */
	    "vst1.u8 {q0}, [%[dest]]\n"
	    :
	    : [dest]"r"(dest), [ref]"r"(ref), [refx]"r"(ref + 1),
		       [refy]"r"(ref + stride), [refxy]"r"(ref + stride + 1)
	    : "memory", "q0", "q1", "q2", "q3");
	ref += stride;
	dest += stride;
    } while (--height);
}

static void MC_put_xy_8_neon (uint8_t * dest, const uint8_t * ref,
			      const int stride, int height)
{
    do {
	asm volatile (
	    "vld1.u8 {d0}, [%[ref]]\n"
	    "vld1.u8 {d1}, [%[refx]]\n"
	    "vrhadd.u8 d0, d0, d1\n"
	    "vld1.u8 {d2}, [%[refy]]\n"
	    "vld1.u8 {d3}, [%[refxy]]\n"
	    "vrhadd.u8 d2, d2, d3\n"
	    /* XXX: three cycles stall */
	    "vrhadd.u8 d0, d0, d2\n"
	    /* XXX: three cycles stall */
	    "vst1.u8 {d0}, [%[dest]]\n"
	    :
	    : [dest]"r"(dest), [ref]"r"(ref), [refx]"r"(ref + 1),
		       [refy]"r"(ref + stride), [refxy]"r"(ref + stride + 1)
	    : "memory", "q0", "q1");
	ref += stride;
	dest += stride;
    } while (--height);
}

static void MC_avg_xy_16_neon (uint8_t * dest, const uint8_t * ref,
			       const int stride, int height)
{
    do {
 	asm volatile (
	    "vld1.u8 {q0}, [%[ref]]\n"
	    "vld1.u8 {q1}, [%[refx]]\n"
	    "vrhadd.u8 q0, q0, q1\n"
	    "vld1.u8 {q2}, [%[refy]]\n"
	    "vld1.u8 {q3}, [%[refxy]]\n"
	    "vrhadd.u8 q2, q2, q3\n"
	    "vld1.u8 {q4}, [%[dest]]\n"
	    /* XXX: one cycle stall */
	    "vrhadd.u8 q0, q0, q2\n"
	    /* XXX: three cycles stall */
	    "vrhadd.u8 q0, q4, q0\n"
	    "vst1.u8 {q0}, [%[dest]]\n"
	    :
	    : [dest]"r"(dest), [ref]"r"(ref), [refx]"r"(ref + 1),
		       [refy]"r"(ref + stride), [refxy]"r"(ref + stride + 1)
	    : "memory", "q0", "q1", "q2", "q3", "q4");
	ref += stride;
	dest += stride;
    } while (--height);
}

static void MC_avg_xy_8_neon (uint8_t * dest, const uint8_t * ref,
			      const int stride, int height)
{
    do {
	asm volatile (
	    "vld1.u8 {d0}, [%[ref]]\n"
	    "vld1.u8 {d1}, [%[refx]]\n"
	    "vrhadd.u8 d0, d0, d1\n"
	    "vld1.u8 {d2}, [%[refy]]\n"
	    "vld1.u8 {d3}, [%[refxy]]\n"
	    "vrhadd.u8 d2, d2, d3\n"
	    "vld1.u8 {d4}, [%[dest]]\n"
	    /* XXX: one cycle stall */
	    "vrhadd.u8 d0, d0, d2\n"
	    /* XXX: three cycles stall */
	    "vrhadd.u8 d0, d4, d0\n"
	    "vst1.u8 {d0}, [%[dest]]\n"
	    :
	    : [dest]"r"(dest), [ref]"r"(ref), [refx]"r"(ref + 1),
		       [refy]"r"(ref + stride), [refxy]"r"(ref + stride + 1)
	    : "memory", "q0", "q1", "d4");
	ref += stride;
	dest += stride;
    } while (--height);
}

MPEG2_MC_EXTERN (neon)

#endif /* ARCH_ARM_NEON */
