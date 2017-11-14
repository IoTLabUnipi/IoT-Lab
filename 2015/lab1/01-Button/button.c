#include <stdio.h>
#include "contiki.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"

/*---------------------------------------------------------------------------*/
PROCESS(button_process, "Button process");
AUTOSTART_PROCESSES(&button_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(button_process, ev, data)
{
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(button_sensor);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event &&
			     data == &button_sensor);
    leds_toggle(LEDS_ALL);
    printf("Button pressed\n");
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
