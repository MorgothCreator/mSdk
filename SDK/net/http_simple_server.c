/*
 * http_simple_server.c
 *
 *  Created on: Nov 7, 2012
 *      Author: XxXx
 */

#include "http_simple_server.h"
#include "lwip_api.h"
#ifdef UseLwip

/* This is the callback function that is called
when a TCP segment has arrived in the connection. */
void http_simple_close_conn(struct tcp_pcb *pcb)
{
  tcp_recv(pcb, NULL);
  tcp_close(pcb);
  /* closing succeeded */
  tcp_arg(pcb, NULL);
  tcp_sent(pcb, NULL);
}

/**
 * Data has been received on this pcb.
 */
err_t http_simple_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
	  if (p != NULL){
	    /* Inform TCP that we have taken the data. */
	    tcp_recved(pcb, p->tot_len);
	   }
	if (err != ERR_OK)
	{
	    /* error or closed by other side */
	    if (p != NULL) {
	      pbuf_free(p);
	    }
		http_simple_close_conn(pcb);
	    return ERR_OK;
	}
	char *data;
	int i;
	/* If we got a NULL pbuf in p, the remote end has closed
	the connection. */
	  err_t request_supported = ERR_ARG;
	if(p != NULL) {
		/* The payload pointer in the pbuf contains the data
		in the TCP segment. */
		data = p->payload;
		  /* @todo: support POST, check p->len */
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

		 if (request_supported == ERR_OK) tcp_write(pcb, indexdata, sizeof(indexdata), 0);
		/* Free the pbuf. */
		pbuf_free(p);
	}
	http_simple_close_conn(pcb);
	return ERR_OK;
}





/* This is the callback function that is called when
a connection has been accepted. */
//static err_t
//http_accept(void *arg, struct tcp_pcb *pcb, err_t err)
//{
	/* Set up the function http_recv() to be called when data
	arrives. */
//	tcp_recv(pcb, http_recv);
//	return ERR_OK;
//}

/**
 * A new incoming connection has been accepted.
 */
err_t http_simple_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
  LWIP_UNUSED_ARG(err);

  /* Decrease the listen backlog counter */
  tcp_accepted(((struct tcp_pcb_listen*)arg));

  tcp_setprio(pcb, ECHO_TCP_PRIO);
  /* Set up the various callback functions */
  tcp_recv(pcb, http_simple_recv);
  tcp_err(pcb,  NULL);
  tcp_poll(pcb, NULL, ECHO_POLL_INTERVAL);
  tcp_sent(pcb, NULL);

  return ERR_OK;
}

/* The initialization function. */
void http_simple_init(unsigned long ip)
{
	lan_interface_init(ip);
	struct tcp_pcb *pcb;
	pcb = tcp_new();
	tcp_bind(pcb, IP_ADDR_ANY, ECHO_SERVER_PORT);
	pcb = tcp_listen(pcb);
	/* initialize callback arg and accept callback */
	tcp_arg(pcb, pcb);
	tcp_accept(pcb, http_simple_accept);
}
#endif

