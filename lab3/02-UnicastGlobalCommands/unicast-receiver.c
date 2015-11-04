#include "contiki.h"
#include "lib/random.h"
#include "sys/etimer.h"
#include "net/ip/uip.h"
#include "net/ip/uip-debug.h"
#include "net/rpl/rpl.h"
 #include <string.h>

#include "simple-udp.h"

#include <stdio.h>
#include <string.h>

#define UDP_PORT 1234
#define SERVICE_ID 190

#define SEND_INTERVAL		(10 * CLOCK_SECOND)
#define SEND_TIME		(random_rand() % (SEND_INTERVAL))

static struct simple_udp_connection unicast_connection;
static unsigned int temp;
/*---------------------------------------------------------------------------*/
PROCESS(unicast_receiver_process, "Unicast receiver example process");
AUTOSTART_PROCESSES(&unicast_receiver_process);
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
  printf("Data received from ");
  uip_debug_ipaddr_print(sender_addr);
  printf(" on port %d from port %d with length %d: '%s'\n",
         receiver_port, sender_port, datalen, data);
  char *received = (char *)data;
  if(strcmp(received, "READ") == 0){
    char buf[20];
    sprintf(buf, "TEMP: %d", temp);
    simple_udp_sendto(&unicast_connection, buf, strlen(buf) + 1, sender_addr);
  }
  if(strcmp(received, "WRITE") > 0){
    received = received + 6;
    temp = atoi(received);
  }
}
/*---------------------------------------------------------------------------*/
static void
set_global_address(void)
{
  uip_ipaddr_t ipaddr;

  uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);
  
  uip_ipaddr_t prefix;
  rpl_dag_t *dag;
  rpl_set_root(RPL_DEFAULT_INSTANCE,&ipaddr);
  dag=rpl_get_any_dag();
  uip_ip6addr(&prefix,0xaaaa,0,0,0,0,0,0,0);
  rpl_set_prefix(dag, &prefix, 64);
  
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(unicast_receiver_process, ev, data)
{

  PROCESS_BEGIN();

  set_global_address();
  temp = 0;
  simple_udp_register(&unicast_connection, UDP_PORT,
                      NULL, UDP_PORT, receiver);

  while(1) {
    PROCESS_WAIT_EVENT();
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
