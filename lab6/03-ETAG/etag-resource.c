#include <stdio.h>
#include "contiki.h"
#include "contiki-net.h"
#include "rest-engine.h"
#include "dev/button-sensor.h"
//#include "dev/leds.h"

static int value = 10;
static const uint8_t *bytes = NULL;
static const uint8_t *etag = NULL;
static int etag_len = 0;

void res_event_get_handler(void* request, void* response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);
void res_event_post_handler(void* request, void* response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);
static void event_handler();


EVENT_RESOURCE(resource_example, "title=\"Resource\";rt=\"Text\"", res_event_get_handler, res_event_post_handler, NULL, NULL, event_handler);

void
res_event_get_handler(void* request, void* response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset){

	/* Populat the buffer with the response payload*/
	char message[20];
	int length = 20;
	unsigned int accept = -1;
	int len = 0;
	

	len = coap_get_header_etag(request, &bytes);
  	REST.get_header_accept(request, &accept);
	
	if(accept == -1 || accept == REST.type.TEXT_PLAIN) {
		printf("%u\n", bytes);
		printf("%u\n", etag);
		
		if(len > 0 && len == etag_len && memcmp(etag, bytes, len) == 0){
			REST.set_response_status(response, REST.status.NOT_MODIFIED);
			REST.set_header_etag(response, etag, etag_len);
		} else {
			sprintf(message, "VALUE:%03u", value);
			length = strlen(message);
			memcpy(buffer, message, length);

			REST.set_header_content_type(response, REST.type.TEXT_PLAIN); 
			REST.set_response_payload(response, buffer, length);
		}
	} else if(accept == REST.type.APPLICATION_XML) {
		sprintf(message, "<value>%03u</value>", value);
		length = strlen(message);
		memcpy(buffer, message, length);

		REST.set_header_content_type(response, REST.type.APPLICATION_XML);
		REST.set_response_payload(response, buffer, length);
	} else{
		REST.set_response_status(response, REST.status.NOT_ACCEPTABLE);
		    const char *msg = "Supporting content-types text/plain, application/xml";
		    REST.set_response_payload(response, msg, strlen(msg));
	}
}

void
res_event_post_handler(void* request, void* response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset){


  int new_value, len;
  const char *val = NULL;
     
  len=REST.get_post_variable(request, "value", &val);

  etag_len = coap_get_header_etag(request, &etag);
  if( len > 0 ){
     new_value = atoi(val);	
     value = new_value;
     REST.set_response_status(response, REST.status.CREATED);
     REST.set_header_etag(response, etag, etag_len);
  } else {
     REST.set_response_status(response, REST.status.BAD_REQUEST);
  }
}

static void
event_handler()
{
  /* Do the update triggered by the event here, e.g., sampling a sensor. */
  ++value;

    /* Notify the registered observers which will trigger the tget_handler to create the response. */
    REST.notify_subscribers(&resource_example);
  
}

/*---------------------------------------------------------------------------*/
PROCESS(server, "Server process");
AUTOSTART_PROCESSES(&server);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(server, ev, data)
{
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(button_sensor);
  rest_init_engine();
  rest_activate_resource(&resource_example, "example");
  while(1) {
    PROCESS_WAIT_EVENT();
    if(ev == sensors_event && data == &button_sensor){
    	//leds_toggle(LEDS_ALL);
    	printf("Button pressed\n");
	resource_example.trigger();
    }
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
