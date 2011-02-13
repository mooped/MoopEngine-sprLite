PROJECT=bunnychickendinosaur

all: $(PROJECT)

.PHONY: all

GAME_OBJECTS := $(shell ls | grep \.cc | sed s/\.cc/\.o/ | sed s/^/temp\\//)

include ../platform.mk

ifeq ($(DEBUG),true)
DEBUG_CFLAGS=-g -DDEBUG
else
DEBUG_CFLAGS=
endif

bundle:
	$(MAKE) clean $(PROJECT) PACKAGE=true
	./package.sh

ifeq ($(PACKAGE),1)
PACKAGE_CFLAGS=-DPACKAGE
else
PACKAGE_CFLAGS=-DTESTBUILD
endif

CFLAGS=\
	$(PLATFORM_CFLAGS)\
	$(ARCH_CFLAGS)\
	$(PACKAGE_CFLAGS)\
	$(DEBUG_CFLAGS)\
	-I../engine

LDFLAGS=\
	$(PLATFORM_LDFLAGS)\
	$(ARCH_LDFLAGS)

$(PROJECT): $(GAME_OBJECTS) ../sprlite.a
	make -C source_assets
	g++ -o $(PROJECT) $(GAME_OBJECTS) ../sprlite.a $(LDFLAGS)

../sprlite.a:
	$(MAKE) -C ../ DEBUG=$(DEBUG)

temp:
	mkdir temp

temp/%.o: %.cc temp
	gcc -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(GAME_OBJECTS)
	rm -df temp
	rm -f ../sprlite.a
	$(MAKE) -C ../ clean

