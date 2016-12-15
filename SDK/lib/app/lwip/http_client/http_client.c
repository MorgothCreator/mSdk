/*
 * http_client.c
 *
 *  Created on: Dec 2, 2016
 *      Author: John Smith
 */

#include "http_client.h"
#include "api/lan_api.h"
#include "interface/LwIp/src/include/lwip/opt.h"
#include "interface/LwIp/src/include/lwip/lwip_timers.h"
#include "interface/LwIp/src/include/lwip/udp.h"
#include "interface/LwIp/src/include/lwip/dns.h"
#include "interface/LwIp/src/include/lwip/ip_addr.h"
#include "interface/LwIp/src/include/lwip/pbuf.h"
#include "interface/LwIp/src/include/lwip/dhcp.h"
#include "interface/LwIp/src/include/lwip/dns.h"
#include "interface/LwIp/src/include/lwip/tcp.h"


/* ECHO protocol states */
enum echoclient_states
{
  ES_NOT_CONNECTED = 0,
  ES_CONNECTED,
  ES_RECEIVED,
  ES_CLOSING,
};

/* structure to be passed as argument to the tcp callbacks */
struct echoclient
{
  enum echoclient_states state; /* connection status */
  struct tcp_pcb *pcb;          /* pointer on the current tcp_pcb */
  struct pbuf *p_tx;            /* pointer on pbuf to be transmitted */
};
/* Private function prototypes -----------------------------------------------*/
static err_t tcp_echoclient_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void tcp_echoclient_connection_close(struct tcp_pcb *tpcb, struct echoclient * es);
static err_t tcp_echoclient_poll(void *arg, struct tcp_pcb *tpcb);
static err_t tcp_echoclient_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void tcp_echoclient_send(struct tcp_pcb *tpcb, struct echoclient * es);
static err_t tcp_echoclient_connected(void *arg, struct tcp_pcb *tpcb, err_t err);
/**
  * @brief Function called when TCP connection established
  * @param tpcb: pointer on the connection contol block
  * @param err: when connection correctly established err should be ERR_OK
  * @retval err_t: returned error
  */
static err_t tcp_echoclient_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
  struct echoclient *es = NULL;
  if (err == ERR_OK)
  {
    /* allocate structure es to maintain tcp connection informations */
    es = (struct echoclient *)mem_malloc(sizeof(struct echoclient));

    if (es != NULL)
    {
      es->state = ES_CONNECTED;
      es->pcb = tpcb;
      unsigned char *data = calloc(1, strlen(arg));
      if(!data)
    	  return ERR_MEM;
      sprintf((char*)data, arg);

      /* allocate pbuf */
      es->p_tx = pbuf_alloc(PBUF_TRANSPORT, strlen((char*)data) , PBUF_POOL);

      if (es->p_tx)
      {
        /* copy data to pbuf */
        pbuf_take(es->p_tx, (char*)data, strlen((char*)data));

        /* pass newly allocated es structure as argument to tpcb */
        tcp_arg(tpcb, es);

        /* initialize LwIP tcp_recv callback function */
        tcp_recv(tpcb, tcp_echoclient_recv);

        /* initialize LwIP tcp_sent callback function */
        tcp_sent(tpcb, tcp_echoclient_sent);

        /* initialize LwIP tcp_poll callback function */
        tcp_poll(tpcb, tcp_echoclient_poll, 1);

        /* send data */
        tcp_echoclient_send(tpcb,es);

        return err;
      }
      free(data);
    }
    else
    {
      /* close connection */
      tcp_echoclient_connection_close(tpcb, es);

      /* return memory allocation error */
      return ERR_MEM;
    }
  }
  else
  {
    /* close connection */
    tcp_echoclient_connection_close(tpcb, es);
  }
  return err;
}

/**
  * @brief tcp_receiv callback
  * @param arg: argument to be passed to receive callback
  * @param tpcb: tcp connection control block
  * @param err: receive error code
  * @retval err_t: retuned error
  */
static err_t tcp_echoclient_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
  struct echoclient *es;
  err_t ret_err;

  LWIP_ASSERT("arg != NULL",arg != NULL);

  es = (struct echoclient *)arg;

  /* if we receive an empty tcp frame from server => close connection */
  if (p == NULL)
  {
    /* remote host closed connection */
    es->state = ES_CLOSING;
    if(es->p_tx == NULL)
    {
       /* we're done sending, close connection */
       tcp_echoclient_connection_close(tpcb, es);
    }
    else
    {
      /* send remaining data*/
      tcp_echoclient_send(tpcb, es);
    }
    ret_err = ERR_OK;
  }
  /* else : a non empty frame was received from echo server but for some reason err != ERR_OK */
  else if(err != ERR_OK)
  {
    /* free received pbuf*/
    if (p != NULL)
    {
      pbuf_free(p);
    }
    ret_err = err;
  }
  else if(es->state == ES_CONNECTED)
  {

    /* Acknowledge data reception */
    tcp_recved(tpcb, p->tot_len);
    uart.puts(DebugCom, p->payload, p->len);

    pbuf_free(p);
    tcp_echoclient_connection_close(tpcb, es);
    ret_err = ERR_OK;
  }

  /* data received when connection already closed */
  else
  {
    /* Acknowledge data reception */
    tcp_recved(tpcb, p->tot_len);

    /* free pbuf and do nothing */
    pbuf_free(p);
    ret_err = ERR_OK;
  }
  return ret_err;
}

/**
  * @brief function used to send data
  * @param  tpcb: tcp control block
  * @param  es: pointer on structure of type echoclient containing info on data
  *             to be sent
  * @retval None
  */
static void tcp_echoclient_send(struct tcp_pcb *tpcb, struct echoclient * es)
{
  struct pbuf *ptr;
  err_t wr_err = ERR_OK;

  while ((wr_err == ERR_OK) &&
         (es->p_tx != NULL) &&
         (es->p_tx->len <= tcp_sndbuf(tpcb)))
  {

    /* get pointer on pbuf from es structure */
    ptr = es->p_tx;

    /* enqueue data for transmission */
    wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);

    if (wr_err == ERR_OK)
    {
      /* continue with next pbuf in chain (if any) */
      es->p_tx = ptr->next;

      if(es->p_tx != NULL)
      {
        /* increment reference count for es->p */
        pbuf_ref(es->p_tx);
      }

      /* free pbuf: will free pbufs up to es->p (because es->p has a reference count > 0) */
      pbuf_free(ptr);
   }
   else if(wr_err == ERR_MEM)
   {
      /* we are low on memory, try later, defer to poll */
     es->p_tx = ptr;
   }
   else
   {
     /* other problem ?? */
   }
  }
}

/**
  * @brief  This function implements the tcp_poll callback function
  * @param  arg: pointer on argument passed to callback
  * @param  tpcb: tcp connection control block
  * @retval err_t: error code
  */
static err_t tcp_echoclient_poll(void *arg, struct tcp_pcb *tpcb)
{
  err_t ret_err;
  struct echoclient *es;

  es = (struct echoclient*)arg;
  if (es != NULL)
  {
    if (es->p_tx != NULL)
    {
      /* there is a remaining pbuf (chain) , try to send data */
      tcp_echoclient_send(tpcb, es);
    }
    else
    {
      /* no remaining pbuf (chain)  */
      if(es->state == ES_CLOSING)
      {
        /* close tcp connection */
        tcp_echoclient_connection_close(tpcb, es);
      }
    }
    ret_err = ERR_OK;
  }
  else
  {
    /* nothing to be done */
    tcp_abort(tpcb);
    ret_err = ERR_ABRT;
  }
  return ret_err;
}

/**
  * @brief  This function implements the tcp_sent LwIP callback (called when ACK
  *         is received from remote host for sent data)
  * @param  arg: pointer on argument passed to callback
  * @param  tcp_pcb: tcp connection control block
  * @param  len: length of data sent
  * @retval err_t: returned error code
  */
static err_t tcp_echoclient_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
  struct echoclient *es;

  LWIP_UNUSED_ARG(len);

  es = (struct echoclient *)arg;

  if(es->p_tx != NULL)
  {
    /* still got pbufs to send */
    tcp_echoclient_send(tpcb, es);
  }

  return ERR_OK;
}

/**
  * @brief This function is used to close the tcp connection with server
  * @param tpcb: tcp connection control block
  * @param es: pointer on echoclient structure
  * @retval None
  */
static void tcp_echoclient_connection_close(struct tcp_pcb *tpcb, struct echoclient * es )
{
  /* remove callbacks */
  tcp_recv(tpcb, NULL);
  tcp_sent(tpcb, NULL);
  tcp_poll(tpcb, NULL,0);

  if (es != NULL)
  {
    mem_free(es);
  }

  /* close tcp connection */
  tcp_close(tpcb);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/**
 * DNS found callback when using DNS names as server address.
 */
static void
tcp_dns_found(const char* hostname, const ip_addr_t *ipaddr, void *arg)
{
  LWIP_UNUSED_ARG(hostname);
  LWIP_UNUSED_ARG(arg);

  if (ipaddr != NULL) {
    /* Address resolved, send request */
  	uart.printf(DebugCom, "tcp_dns_found: %u.%u.%u.%u\n", ((ipaddr->addr >> 0) & 0xFF), ((ipaddr->addr >> 8) & 0xFF), ((ipaddr->addr >> 16) & 0xFF), ((ipaddr->addr >> 24) & 0xFF));
  } else {
    /* DNS resolving failed -> try another server */
  	uart.printf(DebugCom,  "tcp_dns_found: Error getting IP for this DNS\n");
  }
}

void tcp_setup(char *ip_addr, char *mesage)
{
    ip_addr_t ip;
    //IP4_ADDR(&ip, 110,777,888,999);    //IP of my PHP server
    err_t err;
    err = dns_gethostbyname(ip_addr, &ip, tcp_dns_found, NULL);
    //if(err)
    	//uart.printf(DebugCom,  "tcp_dns_found: Error getting IP for this DNS\n");
    //else
    	//uart.printf(DebugCom, "tcp_dns_found: %u.%u.%u.%u\n", ((ip.addr >> 0) & 0xFF), ((ip.addr >> 8) & 0xFF), ((ip.addr >> 16) & 0xFF), ((ip.addr >> 24) & 0xFF));

    struct tcp_pcb *pcb;
    /* create the control block */
    pcb = tcp_new();    //testpcb is a global struct tcp_pcb
                            // as defined by lwIP
    if (pcb != NULL)
    {
		/* dummy data to pass to callbacks*/
		tcp_arg(pcb, mesage);
		/* register callbacks with the pcb */
		//tcp_err(pcb, tcpErrorHandler);
		//tcp_recv(pcb, tcpRecvCallback);
		//tcp_sent(pcb, tcpSendCallback);

		/* now connect */
		tcp_connect(pcb, &ip, 80, tcp_echoclient_connected);
    }
}
