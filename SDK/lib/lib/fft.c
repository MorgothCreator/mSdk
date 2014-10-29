/*
 *  lib/fft.c
 *
 *  Copyright (C) 2013  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of Multiplatform SDK.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <math.h>
#include "../api/uart_api.h"
#include "sys/plat_properties.h"
#include "fft.h"

#ifndef PI
 #ifdef M_PI
  #define PI M_PI
 #else
  #define PI 3.14159265358979323846  /* pi */
 #endif
#endif

#if defined(FLASH_DEVICE) && defined(USE_FFT_TABLES)
#include <avr/pgmspace.h>
#define fft_read_float_data(addr)		pgm_read_float(&addr)
#define fft_read_long_data(addr)		pgm_read_dword(&addr)
#define fft_read_word_data(addr)		pgm_read_word(&addr)
#else
#define fft_read_float_data(addr)		addr
#define fft_read_long_data(addr)		addr
#define fft_read_word_data(addr)		addr
#endif
/* ############################# */
/* # Local function prototypes # */
/* ############################# */

void fft_prepare(const sound_sample *input, USED_FFT_TEMP_REAL_BUFFERS * re, float * im);
void fft_calculate(USED_FFT_TEMP_REAL_BUFFERS * re, float * im);
void fft_output(const USED_FFT_TEMP_REAL_BUFFERS *re, const float *im, FFT_OUTPUT_DATA_TYPE *output);
long reverseBits(unsigned long initial);

/* #################### */
/* # Global variables # */
/* #################### */

#ifndef USE_FFT_TABLES
/* Table to speed up bit reverse copy */
unsigned long *bitReverse;

/* The next two tables could be made to use less space in memory, since they
 * overlap hugely, but hey. */
float *sintable;
float *costable;
#endif

#ifdef USE_FFT_STATIC_BUFF
float imag[FFT_BUFFER_SIZE];
USED_FFT_TEMP_REAL_BUFFERS real[FFT_BUFFER_SIZE];
#endif

/* Table to speed up bit reverse copy */
//static unsigned int bitReverse[FFT_BUFFER_SIZE];

/* The next two tables could be made to use less space in memory, since they
 * overlap hugely, but hey. */
//static USE_FFT_BUFFERS sintable[FFT_BUFFER_SIZE / 2];
//static USE_FFT_BUFFERS costable[FFT_BUFFER_SIZE / 2];

/* ############################## */
/* # Externally called routines # */
/* ############################## */

/* --------- */
/* FFT stuff */
/* --------- */

/*
 * Initialisation routine - sets up tables and space to work in.
 * Returns a pointer to internal state, to be used when performing calls.
 * On error, returns NULL.
 * The pointer should be freed when it is finished with, by fft_close().
 */
fft_state *fft_init(void) 
{
    fft_state *state;
#ifndef USE_FFT_TABLES
    unsigned int i;
	
	sintable = (float *)calloc(1, (FFT_BUFFER_SIZE / 2) * sizeof(float));
	costable = (float *)calloc(1, (FFT_BUFFER_SIZE / 2) * sizeof(float));
	bitReverse = (unsigned long *)calloc(1, FFT_BUFFER_SIZE * sizeof(unsigned long));
#endif

    state = (fft_state *) malloc (sizeof(fft_state));

    if(!state) return NULL;

#ifndef USE_FFT_TABLES
    for(i = 0; i < FFT_BUFFER_SIZE; i++) 
	{
        bitReverse[i] = reverseBits(i);
    }
    for(i = 0; i < FFT_BUFFER_SIZE / 2; i++) 
	{
        USED_FFT_TEMP_REAL_BUFFERS j = (USED_FFT_TEMP_REAL_BUFFERS)2.0 * (USED_FFT_TEMP_REAL_BUFFERS)PI * (USED_FFT_TEMP_REAL_BUFFERS)i / (USED_FFT_TEMP_REAL_BUFFERS)FFT_BUFFER_SIZE;
        //float test;
        costable[i] = (float)cos(j);
        //test = cos(j);
        sintable[i] = (float)sin(j);
    }
#endif



/*    unsigned long *Tmp = (unsigned long*)costable;
    unsigned int TmpCnt = 0;
	UARTPuts(DebugCom, "\n\r{\n\r", -1);
    for(;TmpCnt < FFT_BUFFER_SIZE / 2; TmpCnt++)
    {
    	if(!(TmpCnt & 0x000F)) UARTPuts(DebugCom, "\n\r", -1);
    	UARTPutHexNum(DebugCom, Tmp[TmpCnt]);
    	UARTPuts(DebugCom, ",", -1);
    }
	UARTPuts(DebugCom, "};\n\r", -1);

	UARTPuts(DebugCom, "\n\r{\n\r", -1);
	Tmp = (unsigned long*)sintable;
	for(TmpCnt = 0;TmpCnt < FFT_BUFFER_SIZE / 2; TmpCnt++)
	{
		if(!(TmpCnt & 0x000F)) UARTPuts(DebugCom, "\n\r", -1);
		UARTPutHexNum(DebugCom, Tmp[TmpCnt]);
		UARTPuts(DebugCom, ",", -1);
	}
	UARTPuts(DebugCom, "};\n\r", -1);


	UARTPuts(DebugCom, "\n\r{\n\r", -1);
	Tmp = (unsigned long*)sintable;
	for(TmpCnt = 0;TmpCnt < FFT_BUFFER_SIZE; TmpCnt++)
	{
		if(!(TmpCnt & 0x000F)) UARTPuts(DebugCom, "\n\r", -1);
		UARTPutHexNum(DebugCom, bitReverse[TmpCnt]);
		UARTPuts(DebugCom, ",", -1);
	}
	UARTPuts(DebugCom, "};\n\r", -1);*/

#ifndef USE_FFT_STATIC_BUFF
	state->imag = (float *)calloc(1, FFT_BUFFER_SIZE * sizeof(float));
	state->real = (USED_FFT_TEMP_REAL_BUFFERS *)calloc(1, FFT_BUFFER_SIZE * sizeof(USED_FFT_TEMP_REAL_BUFFERS));
#else
	state->imag = imag;
	state->real = real;
#endif
    return state;
}


/*
 * Free the state.
 */
void fft_close(fft_state *state) 
{
    if(state) free(state);
}

/* ########################### */
/* # Locally called routines # */
/* ########################### */

/*
 * Prepare data to perform an FFT on
 */
void fft_prepare(const sound_sample *input, USED_FFT_TEMP_REAL_BUFFERS * re, float * im)
{
    unsigned int i;
    USED_FFT_TEMP_REAL_BUFFERS *realptr = re;
    float *imagptr = im;

    /* Get input, in reverse bit order */
    for(i = 0; i < FFT_BUFFER_SIZE; i++) 
	{
        sound_sample* ptr = (sound_sample*)&(input[fft_read_word_data(bitReverse[i]) * 2]);
        *realptr++ = (ptr[0] + ptr[1]) / 2;
        *imagptr++ = 0;
    }
}

/*
 * Take result of an FFT and calculate the intensities of each frequency
 * Note: only produces half as many data points as the input had.
 * This is roughly a consequence of the Nyquist–Shannon sampling theorem.
 *
 * The two divisions by 4 are also a consequence of this: the contributions
 * returned for each frequency are split into two parts, one at i in the
 * table, and the other at FFT_BUFFER_SIZE - i, except for i = 0 and
 * FFT_BUFFER_SIZE which would otherwise get USE_FFT_BUFFERS (and then 4* when squared)
 * the contributions.
 */
void fft_output(const USED_FFT_TEMP_REAL_BUFFERS * re, const float * im, FFT_OUTPUT_DATA_TYPE *output)
{
    FFT_OUTPUT_DATA_TYPE *outputptr = output;
    const USED_FFT_TEMP_REAL_BUFFERS *realptr   = re;
    const float *imagptr   = im;
    FFT_OUTPUT_DATA_TYPE *endptr    = output + FFT_BUFFER_SIZE / 2;

    while(outputptr <= endptr) 
	{
        *outputptr = (FFT_OUTPUT_DATA_TYPE)((*realptr * *realptr) + (*imagptr * *imagptr));
        outputptr++; realptr++; imagptr++;
    }
    /* Do divisions to keep the constant and highest frequency terms in scale
     * with the other terms. */
    *output /= 4;
    *endptr /= 4;
}

/*
 * Actually perform the FFT
 */
void fft_calculate(USED_FFT_TEMP_REAL_BUFFERS * re, float * im)
{
    unsigned char i;
    unsigned int j, k;
    unsigned long exchanges;
    float fact_real; 
	float fact_imag;
    USED_FFT_TEMP_REAL_BUFFERS tmp_real, tmp_imag;
    unsigned long factfact;

    /* Set up some variables to reduce calculation in the loops */
    exchanges = 1;
    factfact = FFT_BUFFER_SIZE / 2;

    /* Loop through the divide and conquer steps */
    for(i = FFT_BUFFER_SIZE_LOG; i != 0; i--) 
	{
        /* In this step, we have 2 ^ (i - 1) exchange groups, each with
         * 2 ^ (FFT_BUFFER_SIZE_LOG - i) exchanges
         */
        /* Loop through the exchanges in a group */
        for(j = 0; j != exchanges; j++) 
		{
            /* Work out factor for this exchange
             * factor ^ (exchanges) = -1
             * So, real = cos(j * PI / exchanges),
             *     imag = sin(j * PI / exchanges)
             */
#ifndef FLASH_DEVICE
            float *temp_real = (float*)&fft_read_float_data(costable[j * factfact]);
            float *temp_img = (float*)&fft_read_float_data(sintable[j * factfact]);
			
			fact_real = *temp_real;//(double)fft_read_float_data(costable[j * factfact]);
            fact_imag = *temp_img;//(double)fft_read_float_data(sintable[j * factfact]);
#else
			fact_real = (float)fft_read_float_data(costable[j * factfact]);
			fact_imag = (float)fft_read_float_data(sintable[j * factfact]);
#endif

            /* Loop through all the exchange groups */
            for(k = j; k < FFT_BUFFER_SIZE; k += exchanges << 1) 
			{
                long k1 = k + exchanges;
                /* newval[k]  := val[k] + factor * val[k1]
                 * newval[k1] := val[k] - factor * val[k1]
                 **/
                /* FIXME - potential scope for more optimization here? */
                tmp_real = fact_real * re[k1] - fact_imag * im[k1];
                tmp_imag = fact_real * im[k1] + fact_imag * re[k1];
                re[k1] = re[k] - tmp_real;
                im[k1] = im[k] - tmp_imag;
                re[k]  += tmp_real;
                im[k]  += tmp_imag;
            }
        }
        exchanges <<= 1;
        factfact >>= 1;
    }
}

long reverseBits(unsigned long initial) 
{
    unsigned int reversed = 0, loop;
    for(loop = 0; loop < FFT_BUFFER_SIZE_LOG; loop++) 
	{
        reversed <<= 1;
        reversed += (initial & 1);
        initial >>= 1;
    }
    return reversed;
}

/*
 * Do all the steps of the FFT, taking as input sound data (as described in
 * sound.h) and returning the intensities of each frequency as doubles in the
 * range 0 to ((FFT_BUFFER_SIZE / 2) * 32768) ^ 2
 *
 * FIXME - the above range assumes no frequencies present have an amplitude
 * larger than that of the sample variation.  But this is false: we could have
 * a wave such that its maximums are always between samples, and it's just
 * inside the representable range at the places samples get taken.
 * Question: what _is_ the maximum value possible.  Twice that value?  Root
 * two times that value?  Hmmm.  Think it depends on the frequency, too.
 *
 * The input array is assumed to have FFT_BUFFER_SIZE * 2 elements,
 * and the output array is assumed to have (FFT_BUFFER_SIZE / 2 + 1) elements.
 * state is a (non-NULL) pointer returned by fft_init.
 */
void fft_perform(const sound_sample *input, FFT_OUTPUT_DATA_TYPE *output, fft_state *state)
{
    /* Convert data from sound format to be ready for FFT */
    fft_prepare(input, state->real, state->imag);

    /* Do the actual FFT */
    fft_calculate(state->real, state->imag);

    /* Convert the FFT output into intensities */
    fft_output(state->real, state->imag, output);
}
