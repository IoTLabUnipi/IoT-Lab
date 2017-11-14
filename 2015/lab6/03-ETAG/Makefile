CONTIKI_PROJECT = etag-resource

all: $(CONTIKI_PROJECT)

SMALL=1

CONTIKI=/home/user/contiki
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"


APPS += er-coap
APPS += rest-engine

CONTIKI_WITH_IPV6 = 1
include $(CONTIKI)/Makefile.include
