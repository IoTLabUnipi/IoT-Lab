#include <stdio.h>
#include "contiki.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"

/*---------------------------------------------------------------------------*/
PROCESS(buttontimer_process, "ButtonTimer process");
AUTOSTART_PROCESSES(&buttontimer_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(buttontimer_process, ev, data)
{
	static struct etimer et;
  PROCESS_BEGIN();
	SENSORS_ACTIVATE(button_sensor);
  etimer_set(&et,CLOCK_SECOND*300);

  while(1) {
    PROCESS_WAIT_EVENT();
		if(ev == sensors_event && data == &button_sensor)
		{
			leds_toggle(LEDS_ALL);
			printf("Button pressed\n");
		}
		if(etimer_expired(&et)){
		  leds_toggle(LEDS_ALL);
		  printf("Timer\n");
			etimer_reset(&et);
		}
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
