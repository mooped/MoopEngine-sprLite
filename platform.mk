UNAME_S := $(shell uname -s)
UNAME_P := $(shell uname -p)

ifeq ($(UNAME_S), Darwin)
PLATFORM=MACOSX
PLATFORM_CFLAGS=-DMACOSX -DUNIX -I/opt/local/include/
PLATFORM_LDFLAGS=-framework GLUT -framework OpenGL -lobjc
endif

ifeq ($(UNAME_S), FreeBSD)
PLATFORM=FREEBSD
PLATFORM_CFLAGS=-DFREEBSD -DUNIX -I/usr/local/include/
PLATFORM_LDFLAGS=-L/usr/local/lib/ -lglut -lGL -lcompat -pthread
endif

ifeq ($(UNAME_S), Linux)
PLATFORM=LINUX
PLATFORM_CFLAGS=-DLINUX -DUNIX
PLATFORM_LDFLAGS=-lglut -lGL
endif

ifeq ($(UNAME_S), MINGW32_NT-6.1)
PLATFORM=WIN32
PLATFORM_CFLAGS=-DWIN32 -DUNIX -I/c/Users/moop/devel/freeglut/include
PLATFORM_LDFLAGS=-lopengl32 -lfreeglut -L/c/Users/moop/devel/freeglut/lib -L/c/MinGW/lib/lib/
endif

ifeq ($(UNAME_P), i386)
ARCH=i386
ARCH_CFLAGS=-DI386 -D_32BIT -DLITTLEENDIAN
ARCH_LDFLAGS=
endif

ifeq ($(UNAME_P), i686)
ARCH=i686
ARCH_CFLAGS=-DI686 -D_32BIT -DLITTLEENDIAN
ARCH_LDFLAGS=
endif

ifeq ($(UNAME_P), x86_64)
ARCH=x86_64
ARCH_CFLAGS=-DX86_64 -D_64BIT -DLITTLEENDIAN
ARCH_LDFLAGS=
endif

ifeq ($(UNAME_P), powerpc)
ARCH=powerpc
ARCH_CFLAGS=-DPPC -D_32BIT -DBIGENDIAN
ARCH_LDFLAGS=
endif

