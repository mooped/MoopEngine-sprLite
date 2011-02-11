all: sprlite.a

.PHONY: all

include platform.mk

ifeq ($(DEBUG), 1)
DEBUG_CFLAGS=-g -DDEBUG
else
DEBUG_CFLAGS=
endif

CFLAGS=\
	$(PLATFORM_CFLAGS)\
	$(ARCH_CFLAGS)\
	$(DEBUG_CFLAGS)

LDFLAGS=\
	$(PLATFORM_LDFLAGS)\
	$(ARCH_LDFLAGS)

ENGINE_OBJECTS := $(shell ls engine | grep \.cc | sed /.*_GLES\.cc/d | sed s/\.cc/\.o/ | sed s/^/temp\\//)

sprlite.a: $(ENGINE_OBJECTS)
	ar -r sprlite.a $(ENGINE_OBJECTS)

temp/%.o: engine/%.cc temp
	gcc -c $< -o $@ $(CFLAGS)

temp:
	mkdir temp

clean:
	rm -f $(ENGINE_OBJECTS)
	rm -df temp

