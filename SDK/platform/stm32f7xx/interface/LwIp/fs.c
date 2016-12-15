/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#include "interface/LwIp/src/include/lwip/apps/httpd_opts.h"
#include "interface/LwIp/src/include/lwip/def.h"
#include "interface/LwIp/src/include/lwip/apps/fs.h"
#include "lib/fat_fs/inc/ff.h"
#include <string.h>


/*-----------------------------------------------------------------------------------*/
err_t
fs_open(fs_file *file, const char *name)
{
  if ((file == NULL) || (name == NULL)) {
     return ERR_ARG;
  }
  char tmp_str[256];
  strncpy(tmp_str, HTTPD_PATH, 256);
  strncat(tmp_str, name, 256);
  if(f_open(file, tmp_str, FA_READ) == FR_OK)
	  return ERR_OK;
  /* file not found */
  return ERR_VAL;
}
/*-----------------------------------------------------------------------------------*/
void
fs_close(fs_file *file)
{
  f_close(file);
  LWIP_UNUSED_ARG(file);
}
/*-----------------------------------------------------------------------------------*/
unsigned long
fs_read(fs_file *file, char *buffer, int count)
{
  unsigned long read;
  if(f_eof(file)) {
    return FS_READ_EOF;
  }
  read = f_size(file) - f_tell(file);
  if(read > count) {
    read = count;
  }
  unsigned int _read = 0;
  f_read(file, buffer, read, &_read);
  read = _read;
  return(read);
}
/*-----------------------------------------------------------------------------------*/
unsigned long
fs_bytes_left(fs_file *file)
{
  return f_size(file) - f_tell(file);
}
