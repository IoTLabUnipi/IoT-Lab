CONTIKI_PROJECT = unicast-sender unicast-receiver 

all: $(CONTIKI_PROJECT)

UIP_CONF_IPV6=1
WITH_UIP6=1
CFLAGS += -DUIP_CONF_IPV6_RPL

CONTIKI = /home/user/contiki

include $(CONTIKI)/Makefile.include
