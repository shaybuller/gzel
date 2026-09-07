CC   = gcc

CFLAGS = -Wall -Wextra -I/usr/local/include -fpic -shared

LDLIBS             = -lxcb -lxcb-xkb -lEGL -lGL -lm

TARGET     = libgzel

SRCS = gcore.c   \
	   gshapes.c \
	   gtexts.c  \
	   gmath.c   \
       g_xcb.c    \
	   g_xcb_gl.c \
	   g_gl.c    
       

all: clear clean so

so:
	$(CC) $(CFLAGS) $(LDLIBS) $(SRCS) -o ../../../lib/$(TARGET).so && \
	echo "Create libntr.so in debpkgs/project/usr/lib..."

clean:
	@if [ -f "../../../lib/libntr.so" ]; then \
		sudo rm -f /usr/lib/libntr.so && \
		echo "Removed old libntr.so from debpkgs/project/usr/lib..."; \
	fi

clear:
	clear

