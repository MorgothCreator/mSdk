/*
 * i2s_api.c
 *
 *  Created on: Nov 15, 2016
 *      Author: John Smith
 */

#include <stdbool.h>
#include "i2s_def.h"
#include "i2s_api.h"

bool i2s_init(I2S_t *param)
{
	param->in = &i2s_interface_in;
	param->out = &i2s_interface_out;
	return true;
}
