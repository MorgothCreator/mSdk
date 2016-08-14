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
 *         Simon Goldschmidt
 *
 */

#include <stdlib.h>
#include "api/lan_api.h"
#include "app/lan/httpd.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
//#include "driver/cpu.h"
//#include "fs.h"
//#include "fs.c"
#include "lib/fat_fs/inc/ff.h"
#include "api/uart_api.h"
extern new_uart* DebugCom;
//***********************************************************

#include <string.h>

#ifndef HTTPD_DEBUG
#define HTTPD_DEBUG         LWIP_DBG_OFF
#endif

/** Set this to 1 and add the next line to lwippools.h to use a memp pool
 * for allocating struct http_state instead of the heap:
 *
 * LWIP_MEMPOOL(HTTPD_STATE, 20, 100, "HTTPD_STATE")
 */
#ifndef HTTPD_USE_MEM_POOL
#define HTTPD_USE_MEM_POOL  0
#endif

/** The server port for HTTPD to use */
#define HTTPD_SERVER_PORT                   80
/** Maximum retries before the connection is aborted/closed.
 * - number of times pcb->poll is called -> default is 4*500ms = 2s;
 * - reset when pcb->sent is called
 */
#define HTTPD_MAX_RETRIES                   4
/** The poll delay is X*500ms */
#define HTTPD_POLL_INTERVAL                 4
/** An unsigned short telling us how much data to pass to tcp_write at maximum
 * Define HTTPD_USE_MAX_SEND_LIMIT to 1 to use this
 */
#define HTTPD_MAX_SEND_LIMIT(http_state)    0xffff
/** If 1 HTTPD_MAX_SEND_LIMIT() sets an upper limit to the bytes passed to tcp_writes */
#define HTTPD_USE_MAX_SEND_LIMIT            0
/** An u8_t telling us if we have to copy the file when enqueueing (1) or not (0)*/
#if HTTPD_SUPPORT_DYNAMIC_PAGES
#define HTTPD_FILE_IS_VOLATILE(http_state)  ((http_state)->file_orig != NULL)
#else
#define HTTPD_FILE_IS_VOLATILE(http_state)  0
#endif /* HTTPD_SUPPORT_DYNAMIC_PAGES */
/** Track sent bytes for debug purposes */
#define HTTPD_TRACK_SENT_BYTES              0
/** Priority for tcp pcbs created by HTTPD */
#define HTTPD_TCP_PRIO                      TCP_PRIO_NORMAL

typedef enum
{
	httpd_source_none = 0,
	httpd_source_disk = 1,
	httpd_source_ram = 2
}httpd_source_type;

struct http_state {
  unsigned long left;
#if HTTPD_TRACK_SENT_BYTES
  unsigned long file_size;
  unsigned long sent_total;
#endif /* HTTPD_TRACK_SENT_BYTES */
  unsigned long offset;
#if HTTPD_SUPPORT_DYNAMIC_PAGES
  const unsigned char *file_orig;
#endif /* HTTPD_SUPPORT_DYNAMIC_PAGES */
  unsigned char retries;
  void *fat_instance;
  httpd_source_type source;
};

static err_t http_sent(void *arg, struct tcp_pcb *pcb, unsigned short len);
static err_t http_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);

FATFS *LWIP_FS_DISK;

/** Allocate a struct http_state. */
static struct http_state*
http_state_alloc()
{
#if HTTPD_USE_MEM_POOL
  return memp_malloc(MEMP_HTTPD_STATE);
#else /* HTTPD_USE_MEM_POOL */
  return mem_malloc(sizeof(struct http_state));
#endif /* HTTPD_USE_MEM_POOL */
}

/** Free a struct http_state.
 * Also frees the file data if dynamic.
 */
static void
http_state_free(struct http_state *hs)
{
  if (hs != NULL) {
#if HTTPD_SUPPORT_DYNAMIC_PAGES
    if (hs->file_orig != NULL) {
      mem_free((void*)hs->file_orig);
    }
#endif /* HTTPD_SUPPORT_DYNAMIC_PAGES */
#if HTTPD_USE_MEM_POOL
    memp_free(MEMP_HTTPD_STATE, hs);
#else /* HTTPD_USE_MEM_POOL */
  	UARTprintf(DebugCom, "Remained to send = %u.\n\r" , hs->left);
  	if(hs->source == httpd_source_disk)
  	{
  		f_close((FIL *)hs->fat_instance);
  	}
  	else if(hs->source == httpd_source_ram)
  	{
  		if(hs->fat_instance) free(hs->fat_instance);
  	}
  	if(hs->fat_instance)
  		free(hs->fat_instance);
	hs->fat_instance = NULL;
    mem_free(hs);
#endif /* HTTPD_USE_MEM_POOL */
  }
}

/**
 * The connection shall be actively closed.
 * Reset the sent- and recv-callbacks.
 */
static void
http_close_conn(struct tcp_pcb *pcb, struct http_state *hs)
{
  err_t err;
  LWIP_DEBUGF(HTTPD_DEBUG, ("http_close: pcb=0x%08X hs=0x%08X left=%d\n", pcb,
    hs, hs != NULL ? hs->left : 0));
  tcp_recv(pcb, NULL);
  err = tcp_close(pcb);
  if (err != ERR_OK) {
    /* closing failed, try again later */
    LWIP_DEBUGF(HTTPD_DEBUG, ("Error %s closing pcb=0x%08X\n", lwip_strerr(err), pcb));
    tcp_recv(pcb, http_recv);
  } else {
    /* closing succeeded */
    tcp_arg(pcb, NULL);
    tcp_poll(pcb, NULL, 0);
    tcp_sent(pcb, NULL);
    if (hs != NULL) {
      http_state_free(hs);
    }
  }
}

/**
 * Try to send more data on this pcb.
 */
static void
http_send_data(struct tcp_pcb *pcb, struct http_state *hs)
{
  err_t err = ERR_OK;
  unsigned short len;
  unsigned short snd_buf;

  LWIP_DEBUGF(HTTPD_DEBUG, ("http_send_data: pcb=0x%08X hs=0x%08X left=%d\n", pcb,
    hs, hs != NULL ? hs->left : 0));

  if ((hs == NULL) || (hs->left == 0)) {
    /* Already closed or nothing more to send; be robust: close */
    http_close_conn(pcb, hs);
    return;
  }

  /* We cannot send more data than space available in the send buffer. */
  snd_buf = tcp_sndbuf(pcb);
  len = LWIP_MIN(snd_buf, hs->left);
  if (hs->left <= snd_buf) {
    LWIP_ASSERT((len == hs->left), "hs->left did not fit into unsigned short!");
  }
#if HTTPD_USE_MAX_SEND_LIMIT
  /* upper send limit */
  if (len > HTTPD_MAX_SEND_LIMIT(hs)) {
    len = HTTPD_MAX_SEND_LIMIT(hs);
  }
#endif /* HTTPD_USE_MAX_SEND_LIMIT */

  bool error_alloc_memory = false;
  if(len > 2920) len = 2920;
  unsigned short back_len = len;
  unsigned long bytes_read = 0;
  if(hs->source == httpd_source_disk)
  {
	  unsigned char *tmp_buff;
#ifdef gcc
	  tmp_buff = (unsigned char *)malloc(len);
#else
	  tmp_buff = (unsigned char *)memalign(32, len);
#endif
	  if(tmp_buff == NULL)
		  error_alloc_memory = true;
	  //UARTprintf(DebugCom, "Requested from disk = %u," , (unsigned long)len);
	  //CPUirqe();
	  f_read(hs->fat_instance, (void *)tmp_buff, len, &bytes_read);
	  //len = _FatData_ReadSection(hs->fat_instance, tmp_buff, hs->offset, len);
	  //CPUirqd();
	  if(tmp_buff != NULL)
	  {
		  do {
			err = tcp_write(pcb, tmp_buff, len, HTTPD_FILE_IS_VOLATILE(hs));
			LWIP_DEBUGF(HTTPD_DEBUG, ("http_send_data: tcp_write(%d) -> %s\n", len,
			  lwip_strerr(err)));
			if (err == ERR_MEM) {
			  len /= 2;
			}
		  } while ((err == ERR_MEM) && (len > 1));
		  free(tmp_buff);
	  } else error_alloc_memory = true;
  }
  else if(hs->source == httpd_source_ram)
  {
	  if(hs->offset != 0)
	  {
		  do {
			err = tcp_write(pcb, (void *)hs->offset, len, HTTPD_FILE_IS_VOLATILE(hs));
			LWIP_DEBUGF(HTTPD_DEBUG, ("http_send_data: tcp_write(%d) -> %s\n", len,
			  lwip_strerr(err)));
			if (err == ERR_MEM) {
			  len /= 2;
			}
		  } while ((err == ERR_MEM) && (len > 1));
	  } else error_alloc_memory = true;
  }
  if(back_len != len) error_alloc_memory = true;
  //UARTprintf(DebugCom, " Sended addr = %u\n\r" , hs->offset);
  if (err == ERR_OK) {
    hs->offset += bytes_read;
    LWIP_ASSERT((hs->left >= len), "hs->left >= len");
    hs->left -= len;
  }
  if (hs->left <= 0 || error_alloc_memory == true)
  {
    LWIP_DEBUGF(HTTPD_DEBUG, ("http_send_data: file finished, closing\n"));
    //tcp_write(pcb, (unsigned char *)"HTTP/1.0 205\r\n", sizeof("HTTP/1.0 205\r\n"), HTTPD_FILE_IS_VOLATILE(hs));
    http_close_conn(pcb, hs);
  }
}

/**
 * When data has been received in the correct state, try to parse it
 * as a HTTP request.
 *
 * @param p the received pbuf
 * @param hs the connection state
 * @return ERR_OK if request was OK and hs has been initialized correctly
 *         another err_t otherwise
 */
static err_t
http_parse_request(struct pbuf *p, struct http_state *hs)
{
  int i;
  err_t request_supported;
  char *data;
  //struct fs_file file;
  char* filename = NULL;

  data = p->payload;

  /* default is request not supported */
  request_supported = ERR_ARG;

  /* @todo: support POST, check p->len */
  UARTPuts(DebugCom, data , p->len);
  UARTPuts(DebugCom, "\n\r" , -1);
  if (strncmp(data, "GET ", 4) == 0) {
    request_supported = ERR_OK;
    for(i = 0; i < 40; i++) {
      if (((char *)data + 4)[i] == ' ' ||
         ((char *)data + 4)[i] == '\r' ||
         ((char *)data + 4)[i] == '\n') {
        ((char *)data + 4)[i] = 0;
      }
    }
  }

  if (request_supported == ERR_OK) {
    if (*(char *)(data + 4) == '/' &&
       *(char *)(data + 5) == 0) {
      /* root -> index.html */
      /* @todo: trailing / -> /../index.html */
      filename = ":/index.html";
    } else {
      /* @todo: filter out hostname (valid request!) */
      filename = (char *)data + 3;
      filename[0] = ':';
      char *Ptr2 = strchr(filename, ' ');
      if(Ptr2) Ptr2[0] = 0;
    }
  } else {
    /* invalid request/not implemented */
    filename = ":/501.html";
  }
  LWIP_DEBUGF(HTTPD_DEBUG, ("http_recv: GET -> %s\n", filename));

  //CPUirqe();
  FIL *FsFile = calloc(1, sizeof(FIL));
  //CPUirqd();
  if(f_open(FsFile, filename, FA_READ) != FR_OK)
  {
	  hs->source = httpd_source_none;
	  hs->offset = 0;
	  hs->left = 0;
	  hs->fat_instance = NULL;
	  if(FsFile)
		  free(FsFile);
	  return ERR_OK;
  }
  else
  {
	  UARTprintf(DebugCom, "/*%s, */%u\r\n", /*FsFile->lfn , */FsFile->fsize);
	  hs->left = FsFile->fsize;
	  if(FsFile->fsize > (1 << 16))
	  {
		  hs->source = httpd_source_disk;
		  hs->offset = 0;
		  hs->fat_instance = (void *)FsFile;
	  }
	  else
	  {
		  hs->source = httpd_source_ram;
#ifdef gcc
		  hs->offset = (unsigned long)(void *)malloc(FsFile->fsize);
#else
		  hs->offset = (unsigned long)(void *)memalign(32, FsFile->fsize);
#endif
		  //CPUirqe();
		  unsigned long bytes_read = 0;
		  f_read(FsFile, (void *)hs->offset, FsFile->fsize, &bytes_read);
		  //CPUirqd();
		  hs->fat_instance = (void *)hs->offset;
	  }
	  return ERR_OK;
  }
#if HTTPD_SUPPORT_DYNAMIC_PAGES
  /* @todo: if file was created dynamically and must be freed after sending: */
  /*hs->file_orig = hs->file;*/
#endif /* HTTPD_SUPPORT_DYNAMIC_PAGES */

#if HTTPD_TRACK_SENT_BYTES
  hs->file_size = file.len;
#endif /* HTTPD_TRACK_SENT_BYTES */
  //return ERR_OK;
}

/**
 * The pcb had an error and is already deallocated.
 * The argument might still be valid (if != NULL).
 */
static void
http_err(void *arg, err_t err)
{
  struct http_state *hs = arg;
  LWIP_UNUSED_ARG(err);

  LWIP_DEBUGF(HTTPD_DEBUG, ("http_err: %s", lwip_strerr(err)));

  if (hs != NULL) {
    http_state_free(hs);
  }
}

/**
 * Data has been sent and acknowledged by the remote host.
 * This means that more data can be sent.
 */
static err_t
http_sent(void *arg, struct tcp_pcb *pcb, unsigned short len)
{
  struct http_state *hs = arg;
  LWIP_DEBUGF(HTTPD_DEBUG, ("http_sent: pcb=0x%08X hs=0x%08X len=%d\n", pcb, hs, len));

  LWIP_UNUSED_ARG(len);

  if (hs == NULL) {
    /* this should not happen, but just to be robust... */
    return ERR_OK;
  }
#if HTTPD_TRACK_SENT_BYTES
  hs->sent_total += len;
  LWIP_DEBUGF(HTTPD_DEBUG, ("http_sent: sent_total=%d, file size=%d\n", hs->sent_total, hs->file_size));
  if(hs->sent_total > hs->file_size) {
    LWIP_DEBUGF(HTTPD_DEBUG, ("http_sent: sent %d bytes too much!\n", hs->sent_total - hs->file_size));
  }
#endif /* HTTPD_TRACK_SENT_BYTES */

  /* reset retry counter */
  hs->retries = 0;

  if (hs->left > 0) {
    LWIP_DEBUGF(HTTPD_DEBUG, ("http_sent: %d bytes left, calling http_send_data\n", hs->left));
    http_send_data(pcb, hs);
  } else {
    /* this should normally not happen, print to be robust */
    LWIP_DEBUGF(HTTPD_DEBUG, ("http_sent: no bytes left\n"));
    http_close_conn(pcb, hs);
  }
  return ERR_OK;
}

/**
 * The poll function is called every 2nd second.
 * If there has been no data sent (which resets the retries) in 8 seconds, close.
 * If the last portion of a file has not been sent in 2 seconds, close.
 *
 * This could be increased, but we don't want to waste resources for bad connections.
 */
static err_t
http_poll(void *arg, struct tcp_pcb *pcb)
{
  struct http_state *hs = arg;
  LWIP_DEBUGF(HTTPD_DEBUG, ("http_poll: pcb=0x%08X hs=0x%08X pcb_state=%s\n",
    pcb, hs, tcp_debug_state_str(pcb->state)));

  if (hs == NULL) {
    if (pcb->state == ESTABLISHED) {
      /* arg is null, close. */
      LWIP_DEBUGF(HTTPD_DEBUG, ("http_poll: arg is NULL, close\n"));
      http_close_conn(pcb, hs);
      return ERR_ABRT;
    }
  } else {
    hs->retries++;
    if (hs->retries == HTTPD_MAX_RETRIES) {
      LWIP_DEBUGF(HTTPD_DEBUG, ("http_poll: too many retries, close\n"));
      http_close_conn(pcb, hs);
      return ERR_ABRT;
    }
    LWIP_DEBUGF(HTTPD_DEBUG, ("http_poll: try to send more data\n"));
    http_send_data(pcb, hs);
  }

  return ERR_OK;
}

/**
 * Data has been received on this pcb.
 * For HTTP 1.0, this should normally only happen once (if the request fits in one packet).
 */
static err_t
http_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
  err_t parsed = ERR_ABRT;
  struct http_state *hs = arg;
  LWIP_DEBUGF(HTTPD_DEBUG, ("http_recv: pcb=0x%08X pbuf=0x%08X err=%s\n", pcb, p,
    lwip_strerr(err)));

  if (p != NULL) {
    /* Inform TCP that we have taken the data. */
    tcp_recved(pcb, p->tot_len);
  }

  if (hs == NULL) {
    /* be robust */
    LWIP_DEBUGF(HTTPD_DEBUG, ("Error, http_recv: hs is NULL, abort\n"));
    http_close_conn(pcb, hs);
    return ERR_OK;
  }

  if ((err != ERR_OK) || (p == NULL)) {
    /* error or closed by other side */
    if (p != NULL) {
      pbuf_free(p);
    }
    http_close_conn(pcb, hs);
    return ERR_OK;
  }

  if (hs->source == httpd_source_none) {
    parsed = http_parse_request(p, hs);
  } else {
    LWIP_DEBUGF(HTTPD_DEBUG, ("http_recv: already sending data\n"));
  }
  pbuf_free(p);
  if (parsed == ERR_OK) {
    LWIP_DEBUGF(HTTPD_DEBUG, ("http_recv: data %p len %ld\n", hs->offset, hs->left));
    http_send_data(pcb, hs);
  } else if (parsed == ERR_ABRT) {
    http_close_conn(pcb, hs);
    return ERR_OK;
  }
  return ERR_OK;
}

/**
 * A new incoming connection has been accepted.
 */
static err_t
http_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
  struct http_state *hs;

  LWIP_UNUSED_ARG(err);
  LWIP_DEBUGF(HTTPD_DEBUG, ("http_accept(%p)\n", arg));

  /* Decrease the listen backlog counter */
  tcp_accepted(((struct tcp_pcb_listen*)arg));

  tcp_setprio(pcb, HTTPD_TCP_PRIO);

  /* Allocate memory for the structure that holds the state of the
     connection. */
  hs = http_state_alloc();
  if (hs == NULL) {
    LWIP_DEBUGF(HTTPD_DEBUG, ("http_accept: Out of memory, RST\n"));
    return ERR_MEM;
  }

  /* Initialize the structure. */
  hs->offset = 0;
  hs->source = httpd_source_none;
#if HTTPD_SUPPORT_DYNAMIC_PAGES
  hs->file_orig = NULL;
#endif /* HTTPD_SUPPORT_DYNAMIC_PAGES */
  hs->left = 0;
  hs->retries = 0;
  hs->fat_instance = NULL;
#if HTTPD_TRACK_SENT_BYTES
  hs->sent_total = 0;
#endif /* HTTPD_TRACK_SENT_BYTES */

  /* Tell TCP that this is the structure we wish to be passed for our
     callbacks. */
  tcp_arg(pcb, hs);

  /* Set up the various callback functions */
  tcp_recv(pcb, http_recv);
  tcp_err(pcb, http_err);
  tcp_poll(pcb, http_poll, HTTPD_POLL_INTERVAL);
  tcp_sent(pcb, http_sent);

  return ERR_OK;
}
/**
 * Initialize the httpd: set up a listening PCB and bind it to the defined port
 */
void
httpd_init(unsigned char instance, unsigned long ip)
{
	lan_init(instance, ip);
  struct tcp_pcb *pcb;
  err_t err;

  pcb = tcp_new();
  LWIP_ASSERT(("httpd_init: tcp_new failed"), pcb != NULL);
  err = tcp_bind(pcb, IP_ADDR_ANY, HTTPD_SERVER_PORT);
  LWIP_ASSERT(("httpd_init: tcp_bind failed: %s", lwip_strerr(err)), err == ERR_OK);
  pcb = tcp_listen(pcb);
  LWIP_ASSERT(("httpd_init: tcp_listen failed"), pcb != NULL);
  /* initialize callback arg and accept callback */
  tcp_arg(pcb, pcb);
  tcp_accept(pcb, http_accept);
}

