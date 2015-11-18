#include "contiki.h"
#include "contiki-net.h"
#include "rest-engine.h"

void
get_handler(void* request, void* response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset){

/* Populat the buffer with the response payload*/
char const * const message = "Hello World!";
int length = 12;
memcpy(buffer, message, length);

REST.set_header_content_type(response, REST.type.TEXT_PLAIN); 
REST.set_header_etag(response, (uint8_t *) &length, 1);
REST.set_response_payload(response, buffer, length);
}

RESOURCE(resource_example, "title=\"Resource\";rt=\"Text\"", get_handler, NULL, NULL, NULL);

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

