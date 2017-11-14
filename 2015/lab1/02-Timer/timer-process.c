#include <stdio.h>
#include "contiki.h"
#include "dev/leds.h"

/*---------------------------------------------------------------------------*/
PROCESS(timer_process, "Timer process");
AUTOSTART_PROCESSES(&timer_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(timer_process, ev, data)
{
	static struct etimer et;
  PROCESS_BEGIN();
  etimer_set(&et,CLOCK_SECOND*100);

  while(1) {
    PROCESS_WAIT_EVENT();
		if(etimer_expired(&et)){
		  leds_toggle(LEDS_ALL);
		  printf("Timer\n");
			etimer_reset(&et);
		}
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
