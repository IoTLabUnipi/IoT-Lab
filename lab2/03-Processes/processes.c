#include "contiki.h"
#include "dev/button-sensor.h"
#include "lib/random.h"
#include <stdio.h>
static int random;
static process_event_t new_data;
/*---------------------------------------------------------------------------*/
PROCESS(button_process, "Button process");
PROCESS(print_process, "Print process");
AUTOSTART_PROCESSES(&button_process, &print_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(button_process, ev, data)
{
  
  PROCESS_BEGIN();
 
  new_data=process_alloc_event();

  SENSORS_ACTIVATE(button_sensor);
  
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event &&
			     data == &button_sensor);
    
    random=random_rand();
    printf("Send event with value:%d\n",random);
    process_post(&print_process, new_data, &random);
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(print_process, ev, data)
{
  
  PROCESS_BEGIN();
 
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == new_data); 
    
    printf("Received value:%d\n", *(int*)data);
 }
  PROCESS_END();
}
