#include "contiki.h"
#include "contiki-net.h"
#include "rest-engine.h"
#include "dev/leds.h"
#include <stdio.h>

static int value = 10;

void
get_handler(void* request, void* response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset){

	/* Populat the buffer with the response payload*/
	char message[30];
	int length = 30;
	unsigned char status;
	unsigned int red, blue, green;

	status = leds_get();
	red = status & 0x02;
	red = red >> 1;
	blue = status & 0x01;
	green = status & 0x04;
	green = green >> 2;
	sprintf(message, "red:%u, blue:%u, green:%u\n", red, blue, green);
	length = strlen(message);
	memcpy(buffer, message, length);

	REST.set_header_content_type(response, REST.type.TEXT_PLAIN); 
	REST.set_header_etag(response, (uint8_t *) &length, 1);
	REST.set_response_payload(response, buffer, length);
}

void
post_handler(void* request, void* response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset){


  size_t len = 0;
  const char *color = NULL;
  const char *mode = NULL;
  uint8_t led = 0;
  int success = 1;
	REST.set_response_status(response, REST.status.CHANGED);
  if((len = REST.get_query_variable(request, "color", &color))) {
    printf("color %.*s\n", len, color);
	// on the z1 leds macros are broken
	// red = LEDS_BLUE
	// green = LEDS_RED
	// blue = LEDS_GREEN
    if(strncmp(color, "r", len) == 0) {
      led = LEDS_BLUE;
    } else if(strncmp(color, "g", len) == 0) {
      led = LEDS_RED;
    } else if(strncmp(color, "b", len) == 0) {
      led = LEDS_GREEN;
    } else {
      success = 0;
    }
  } else {
    success = 0;
  } if(success && (len = REST.get_post_variable(request, "mode", &mode))) {
    printf("mode %s\n", mode);

    if(strncmp(mode, "on", len) == 0) {
      leds_on(led);
    } else if(strncmp(mode, "off", len) == 0) {
      leds_off(led);
    } else {
      success = 0;
    }
  } else {
    success = 0;
  } if(!success) {
    REST.set_response_status(response, REST.status.BAD_REQUEST);
  }
}

RESOURCE(resource_example, "title=\"Resource\";rt=\"Text\"", get_handler, post_handler, NULL, NULL);

PROCESS(server, "CoAP Server");
AUTOSTART_PROCESSES(&server);
PROCESS_THREAD(server, ev, data){
	PROCESS_BEGIN();
	rest_init_engine();
	rest_activate_resource(&resource_example, "example");
	while(1) {
   		 PROCESS_WAIT_EVENT();
	}
	PROCESS_END();
}

