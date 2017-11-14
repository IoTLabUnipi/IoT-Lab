#include "contiki.h"
#include "lib/random.h"
#include "sys/etimer.h"
#include "net/ip/uip.h"
#include "net/ip/uip-debug.h"
#include "sys/node-id.h"
#include "net/rpl/rpl.h"
#include "dev/button-sensor.h"

#include "simple-udp.h"

#include <stdio.h>
#include <string.h>

#define UDP_PORT 1234

#define SEND_INTERVAL		(60 * CLOCK_SECOND)
#define SEND_TIME		(random_rand() % (SEND_INTERVAL))

static struct simple_udp_connection unicast_connection;

/*---------------------------------------------------------------------------*/
PROCESS(unicast_sender_process, "Unicast sender example process");
AUTOSTART_PROCESSES(&unicast_sender_process);
/*---------------------------------------------------------------------------*/
static void
receiver(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
  printf("Data received on port %d from port %d with length %d:, %s\n",
         receiver_port, sender_port, datalen, data);
}
/*---------------------------------------------------------------------------*/
static void
set_global_address(void)
{
  uip_ipaddr_t ipaddr;

  uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(unicast_sender_process, ev, data)
{
  static struct etimer periodic_timer;
  static struct etimer send_timer;
  uip_ipaddr_t addr;

  PROCESS_BEGIN();

  set_global_address();
  
  simple_udp_register(&unicast_connection, UDP_PORT,
                      NULL, UDP_PORT, receiver);

  etimer_set(&periodic_timer, SEND_INTERVAL);
  SENSORS_ACTIVATE(button_sensor);
  while(1) {

    PROCESS_WAIT_EVENT();
    if(etimer_expired(&periodic_timer)){
      etimer_reset(&periodic_timer);
      etimer_set(&send_timer, SEND_TIME);
    }
    if(etimer_expired(&send_timer)){
      uip_ip6addr(&addr,0xaaaa,0,0,0,0xc30c,0,0,0x0001);
   
      char buf[20];

      printf("Sending unicast to ");
      uip_debug_ipaddr_print(&addr);
      printf("\n");
      sprintf(buf, "READ");
      simple_udp_sendto(&unicast_connection, buf, strlen(buf) + 1, &addr);
    }
    if(ev==sensors_event && data==&button_sensor)
    {
      uip_ip6addr(&addr,0xaaaa,0,0,0,0xc30c,0,0,0x0001);
   
      static unsigned int message_number;
      char buf[20];

      printf("Sending unicast to ");
      uip_debug_ipaddr_print(&addr);
      printf("\n");
      sprintf(buf, "WRITE %d", message_number);
      message_number++;
      simple_udp_sendto(&unicast_connection, buf, strlen(buf) + 1, &addr);
    }
   
    
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
